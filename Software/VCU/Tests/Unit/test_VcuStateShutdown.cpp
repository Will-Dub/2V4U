#include "../../App/Inc/Logic/VcuStateShutdown.hpp"
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

TEST_F(VcuStateShutdownTest, GivenMotorCurrentExactly1A_WhenUpdate_ThenContactorOpens)
{
    in.vesc.motorCurrent = 1.0f;
    state.update(in, out);
    EXPECT_FALSE(out.power.contactor);
    EXPECT_FALSE(out.power.prechargeRelay);
}

TEST_F(VcuStateShutdownTest, GivenVescStillTalking_WhenUpdate_ThenStateSame)
{
    in.tickMs = 100;
    in.vesc.lastRxMs = 0;
    EXPECT_EQ(state.update(in, out), VcuStateId::SAME);
}

TEST_F(VcuStateShutdownTest, GivenVescSilentOver250ms_WhenUpdate_ThenGoesToStandby)
{
    in.tickMs = 251;
    in.vesc.lastRxMs = 0;
    EXPECT_EQ(state.update(in, out), VcuStateId::STANDBY);
}

TEST_F(VcuStateShutdownTest, GivenVescSilentExactly250ms_WhenUpdateThenStateSame)
{
    in.tickMs = 250;
    in.vesc.lastRxMs = 0;
    EXPECT_EQ(state.update(in, out), VcuStateId::SAME);
}

TEST_F(VcuStateShutdownTest, GivenMoreThan10TxFailures_WhenUpdate_ThenGoesToStandby)
{
    in.vesc.followedTxFailCount = 11;
    EXPECT_EQ(state.update(in, out), VcuStateId::STANDBY);
}

TEST_F(VcuStateShutdownTest, GivenExactly10TxFailures_WhenUpdate_ThenStateSame)
{
    in.vesc.followedTxFailCount = 10;
    EXPECT_EQ(state.update(in, out), VcuStateId::SAME);
}

TEST_F(VcuStateShutdownTest,
       GivenCurrentFlowingAndVescDead_WhenUpdate_ThenStateSameAndContactorClosed)
{
    in.vesc.motorCurrent = 5.0f;
    in.tickMs = 1000;
    in.vesc.lastRxMs = 0;
    EXPECT_EQ(state.update(in, out), VcuStateId::SAME);
    EXPECT_TRUE(out.power.contactor);
}