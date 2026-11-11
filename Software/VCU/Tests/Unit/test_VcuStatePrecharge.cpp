#include "../../App/Inc/Logic/VcuStatePrecharge.hpp"
#include "FakeLogger.hpp"

#include <gtest/gtest.h>

using namespace App::Logic;

class VcuStatePrechargeTest : public ::testing::Test
{
  protected:
    FakeLogger logger;
    VcuStatePrecharge state{nullptr, logger};
    VcuInputs in{};
    VcuOutputs out{};

    void SetUp() override
    {
        in.tickMs = 10000;
        state.onEnter(in);
    }
};

TEST_F(VcuStatePrechargeTest, GivenPrechargeState_WhenGetId_ThenReturnsPrecharge)
{
    EXPECT_EQ(state.getId(), VcuStateId::PRECHARGE);
}

TEST_F(VcuStatePrechargeTest,
       GivenPrechargeStarted_WhenUpdate_ThenPrechargeRelayClosedAndContactorOpen)
{
    state.update(in, out);
    EXPECT_TRUE(out.power.prechargeRelay);
    EXPECT_FALSE(out.power.contactor);
}

TEST_F(VcuStatePrechargeTest, GivenBusVoltageAbove46V_WhenUpdate_ThenGoesToDrive)
{
    in.vesc.inputVoltage = 46.1f;
    EXPECT_EQ(state.update(in, out), VcuStateId::DRIVE);
}

TEST_F(VcuStatePrechargeTest, GivenBusVoltageExactly46V_WhenUpdate_ThenStateSame)
{
    in.vesc.inputVoltage = 46.0f;
    EXPECT_EQ(state.update(in, out), VcuStateId::SAME);
}

TEST_F(VcuStatePrechargeTest, GivenLowVoltageAndUnder3s_WhenUpdate_ThenStateSame)
{
    in.vesc.inputVoltage = 20.0f;
    in.tickMs = 10000 + 2999;
    EXPECT_EQ(state.update(in, out), VcuStateId::SAME);
}

TEST_F(VcuStatePrechargeTest, GivenLowVoltageAfter3s_WhenUpdate_ThenGoesToFault)
{
    in.vesc.inputVoltage = 20.0f;
    in.tickMs = 10000 + 3000;
    EXPECT_EQ(state.update(in, out), VcuStateId::FAULT);
}

TEST_F(VcuStatePrechargeTest, GivenVoltageReachedOnTimeoutTick_WhenUpdate_ThenGoesToDrive)
{
    in.vesc.inputVoltage = 50.0f;
    in.tickMs = 10000 + 3000;
    EXPECT_EQ(state.update(in, out), VcuStateId::DRIVE);
}

TEST_F(VcuStatePrechargeTest, GivenReenteredPrecharge_WhenUpdate_ThenTimeoutRestartsFromEntry)
{
    in.tickMs = 50000;
    state.onEnter(in);
    in.tickMs = 50000 + 2999;
    EXPECT_EQ(state.update(in, out), VcuStateId::SAME);
}