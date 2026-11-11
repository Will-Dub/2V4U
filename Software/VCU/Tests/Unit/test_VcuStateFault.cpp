#include "../../App/Inc/Logic/VcuStateFault.hpp"
#include "FakeLogger.hpp"

#include <gtest/gtest.h>

using namespace App::Logic;

class VcuStateFaultTest : public ::testing::Test
{
  protected:
    FakeLogger logger;
    VcuStateFault state{nullptr, logger};
    VcuInputs in{};
    VcuOutputs out{};

    void SetUp() override
    {
        state.onEnter(in);
    }
};

TEST_F(VcuStateFaultTest, GivenFaultState_WhenGetId_ThenReturnsFault)
{
    EXPECT_EQ(state.getId(), VcuStateId::FAULT);
}

TEST_F(VcuStateFaultTest, GivenUnsafeOutputs_WhenUpdate_ThenAllOutputsForcedSafe)
{
    out.vesc.isMotorEnabled = true;
    out.vesc.targetAmp = 15.0f;
    out.power.contactor = true;
    out.power.prechargeRelay = true;

    state.update(in, out);

    EXPECT_FALSE(out.vesc.isMotorEnabled);
    EXPECT_FLOAT_EQ(out.vesc.targetAmp, 0.0f);
    EXPECT_FALSE(out.power.contactor);
    EXPECT_FALSE(out.power.prechargeRelay);
    EXPECT_TRUE(out.lights.errorLed);
}

TEST_F(VcuStateFaultTest, GivenAllInputsHealthy_WhenUpdated100Times_ThenStateStaysSame)
{
    in.pilot.isEStopPressed = false;
    in.pilot.isThrottleValid = true;
    in.vesc.inputVoltage = 50.0f;
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(state.update(in, out), VcuStateId::SAME);
    }
}

TEST_F(VcuStateFaultTest, GivenFaultState_WhenUpdate_ThenBuzzerAlarmLoops)
{
    state.update(in, out);
    EXPECT_EQ(out.buzzer.mode, BuzzerMode::BEEP_LOOP);
    EXPECT_EQ(out.buzzer.beepMs, 50);
    EXPECT_EQ(out.buzzer.pauseMs, 2000);
}

TEST_F(VcuStateFaultTest, Given50Updates_WhenUpdateOnceMore_ThenBoardLedToggles)
{
    for (int i = 0; i < 50; i++)
        state.update(in, out);
    const bool before = out.lights.boardLed;
    state.update(in, out);
    EXPECT_NE(out.lights.boardLed, before);
}