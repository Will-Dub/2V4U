#include "../../App/Inc/Logic/VcuStateShutdown.hpp"
#include "Calibration.hpp"
#include "FakeLogger.hpp"

#include <gtest/gtest.h>

using namespace App::Logic;

class VcuStateShutdownTest : public ::testing::Test
{
  protected:
    FakeLogger logger;
    VcuStateShutdown state{nullptr, logger};
    VcuInputs in{};
    VcuOutputs out{};

    void SetUp() override
    {
        in.vesc.isOk = true;
        in.bms.isOk = true;
    }
};

TEST_F(VcuStateShutdownTest, GivenShutdownState_WhenGetId_ThenReturnsShutdown)
{
    EXPECT_EQ(state.getId(), VcuStateId::SHUTDOWN);
}

TEST_F(VcuStateShutdownTest,
       GivenMotorEnabledWithCurrent_WhenUpdate_ThenMotorDisabledAndCurrentZero)
{
    out.vesc.isMotorEnabled = true;
    out.vesc.targetAmp = 12.0f;
    state.update(in, out);
    EXPECT_FALSE(out.vesc.isMotorEnabled);
    EXPECT_FLOAT_EQ(out.vesc.targetAmp, 0.0f);
}

TEST_F(VcuStateShutdownTest,
       GivenMotorCurrentAbove1A_WhenUpdate_ThenContactorStaysClosedAndStateSame)
{
    in.vesc.motorCurrent = 1.5f;
    EXPECT_EQ(state.update(in, out), VcuStateId::SAME);
    EXPECT_TRUE(out.power.contactor);
    EXPECT_FALSE(out.power.prechargeRelay);
}

TEST_F(VcuStateShutdownTest, GivenRegenCurrentFlowing_WhenUpdate_ThenContactorStaysClosed)
{
    in.vesc.motorCurrent = -5.0f;
    state.update(in, out);
    EXPECT_TRUE(out.power.contactor);
}
TEST_F(VcuStateShutdownTest, GivenMotorCurrentExactlyAtThreshold_WhenUpdate_ThenContactorOpens)
{
    in.vesc.motorCurrent = Calibration::SHUTDOWN_CURRENT_A;
    state.update(in, out);
    EXPECT_FALSE(out.power.contactor);
    EXPECT_FALSE(out.power.prechargeRelay);
}

TEST_F(VcuStateShutdownTest, GivenVescOk_WhenUpdate_ThenStateSame)
{
    EXPECT_EQ(state.update(in, out), VcuStateId::SAME);
}

TEST_F(VcuStateShutdownTest, GivenVescNotOk_WhenUpdate_ThenGoesToStandby)
{
    in.vesc.isOk = false;
    EXPECT_EQ(state.update(in, out), VcuStateId::STANDBY);
}

TEST_F(VcuStateShutdownTest,
       GivenCurrentFlowingAndVescNotOk_WhenUpdate_ThenContactorOpensAndStandby)
{
    in.vesc.motorCurrent = 5.0f;
    in.vesc.isOk = false;
    EXPECT_EQ(state.update(in, out), VcuStateId::STANDBY);
    EXPECT_FALSE(out.power.contactor);
}