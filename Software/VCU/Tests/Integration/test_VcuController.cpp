#include "../../App/Inc/Logic/Calibration.hpp"
#include "../../App/Inc/Logic/VcuController.hpp"
#include "FakeLogger.hpp"

#include <gtest/gtest.h>

using namespace App::Logic;

class VcuControllerTest : public ::testing::Test
{
  protected:
    FakeLogger logger;
    VcuController vcu{logger};
    VcuInputs in{};
    VcuOutputs out{};

    void SetUp() override
    {
        in.pilot.isEStopPressed = false;
        in.pilot.isThrottleValid = true;
        in.pilot.isForwardDirection = true;
        in.vesc.isOk = true;
        in.bms.isOk = true;
        vcu.init(in);
    }

    void tick(uint32_t dtMs = 10)
    {
        in.tickMs += dtMs;
        out = VcuOutputs{};
        vcu.run(in, out);
    }

    void holdThrottle(float throttle, int ticks = 150)
    {
        in.pilot.throttlePercent = throttle;
        for (int i = 0; i < ticks; i++)
            tick();
    }

    VcuStateId state() const
    {
        return vcu.getCurrentVcuStateId();
    }

    void goToDrive()
    {
        tick();
        in.pilot.isStartPressed = true;
        tick();
        in.vesc.inputVoltage = 50.0f;
        tick();
        ASSERT_EQ(state(), VcuStateId::DRIVE);
    }
};

// ---- Startup sequence
TEST_F(VcuControllerTest, GivenJustInitialized_WhenNoTickYet_ThenStateIsInit)
{
    EXPECT_EQ(state(), VcuStateId::INIT);
}

TEST_F(VcuControllerTest, GivenInitState_WhenFirstTick_ThenMovesToStandby)
{
    tick();
    EXPECT_EQ(state(), VcuStateId::STANDBY);
}

TEST_F(VcuControllerTest, GivenStandby_WhenStartPressed_ThenMovesToPrecharge)
{
    tick();
    in.pilot.isStartPressed = true;
    tick();
    EXPECT_EQ(state(), VcuStateId::PRECHARGE);
}

TEST_F(VcuControllerTest, GivenPrecharge_WhenBusVoltageReached_ThenEntersDrive)
{
    goToDrive();
}

TEST_F(VcuControllerTest, GivenDriveState_WhenTick_ThenMotorEnabled)
{
    goToDrive();
    in.pilot.throttlePercent = 0.5f;
    tick();
    EXPECT_TRUE(out.vesc.isMotorEnabled);
}

// ---- Safety: E-stop, throttle, VESC
TEST_F(VcuControllerTest, GivenStandby_WhenEStopPressed_ThenGoesToFault)
{
    tick();
    in.pilot.isEStopPressed = true;
    tick();
    EXPECT_EQ(state(), VcuStateId::FAULT);
}

TEST_F(VcuControllerTest, GivenDrivingWithThrottle_WhenEStopPressed_ThenCurrentZeroAndContactorOpen)
{
    goToDrive();
    in.pilot.throttlePercent = 0.5f;
    tick();

    in.pilot.isEStopPressed = true;
    tick();

    EXPECT_EQ(state(), VcuStateId::FAULT);
    EXPECT_FLOAT_EQ(out.vesc.targetAmp, 0.0f);
    EXPECT_FALSE(out.power.contactor);
}

TEST_F(VcuControllerTest, GivenFault_WhenEStopReleased_ThenStaysInFault)
{
    tick();
    in.pilot.isEStopPressed = true;
    tick();
    in.pilot.isEStopPressed = false;
    tick();
    EXPECT_EQ(state(), VcuStateId::FAULT);
}

TEST_F(VcuControllerTest, GivenStandby_WhenThrottleBecomesInvalid_ThenGoesToFault)
{
    tick();
    in.pilot.isThrottleValid = false;
    tick();
    EXPECT_EQ(state(), VcuStateId::FAULT);
}

TEST_F(VcuControllerTest, GivenDrive_WhenVescNotOk_ThenGoesToFault)
{
    goToDrive();
    in.vesc.isOk = false;
    tick(300);
    EXPECT_EQ(state(), VcuStateId::FAULT);
}

TEST_F(VcuControllerTest, GivenStandby_WhenVescNotOk_ThenStaysInStandby)
{
    tick();
    in.vesc.isOk = false;
    tick(300);
    EXPECT_EQ(state(), VcuStateId::STANDBY);
}

TEST_F(VcuControllerTest, GivenDrive_WhenBmsNotOk_ThenGoesToFault)
{
    goToDrive();
    in.bms.isOk = false;
    tick();
    EXPECT_EQ(state(), VcuStateId::FAULT);
}

TEST_F(VcuControllerTest, GivenStandby_WhenBmsNotOk_ThenStaysInStandby)
{
    tick();
    in.bms.isOk = false;
    tick();
    EXPECT_EQ(state(), VcuStateId::STANDBY);
}

TEST_F(VcuControllerTest, GivenDrive_WhenBmsOkAndVescOk_ThenStaysInDrive)
{
    goToDrive();
    tick();
    EXPECT_EQ(state(), VcuStateId::DRIVE);
}

TEST_F(VcuControllerTest, GivenDriveWithThrottle_WhenBmsNotOk_ThenCurrentZeroAndContactorOpen)
{
    goToDrive();
    in.pilot.throttlePercent = 0.5f;
    tick();

    in.bms.isOk = false;
    tick();

    EXPECT_EQ(state(), VcuStateId::FAULT);
    EXPECT_FLOAT_EQ(out.vesc.targetAmp, 0.0f);
    EXPECT_FALSE(out.power.contactor);
}

// ---- Throttle and brake latch
TEST_F(VcuControllerTest, GivenDrive_WhenFullThrottleHeld_ThenCurrentReachesMax)
{
    goToDrive();
    in.pilot.throttlePercent = 0.0f;
    tick();
    holdThrottle(1.0f);
    EXPECT_NEAR(out.vesc.targetAmp, Calibration::MAX_AMPS, 0.1f);
}

TEST_F(VcuControllerTest, GivenDrive_WhenThrottleHalfHeld_ThenCurrentIsPositiveAndBelowMax)
{
    goToDrive();
    in.pilot.throttlePercent = 0.0f;
    tick();
    holdThrottle(0.5f);
    EXPECT_GT(out.vesc.targetAmp, 0.0f);
    EXPECT_LT(out.vesc.targetAmp, Calibration::MAX_AMPS);
}

TEST_F(VcuControllerTest, GivenDrive_WhenThrottleJumpsToFull_ThenCurrentRisesGradually)
{
    goToDrive();
    in.pilot.throttlePercent = 0.0f;
    tick();
    in.pilot.throttlePercent = 1.0f;
    tick();
    EXPECT_LT(out.vesc.targetAmp, Calibration::MAX_AMPS * 0.1f);
}

TEST_F(VcuControllerTest,
       GivenShutdownWithCurrentFlowing_WhenVescLinkDies_ThenContactorOpensAndStandby)
{
    goToDrive();
    in.vesc.motorCurrent = 5.0f;
    in.pilot.isStartPressed = false;
    tick();
    EXPECT_EQ(state(), VcuStateId::SHUTDOWN);
    EXPECT_TRUE(out.power.contactor);

    in.vesc.isOk = false;
    tick(300);
    EXPECT_EQ(state(), VcuStateId::STANDBY);
    EXPECT_FALSE(out.power.contactor);
}

TEST_F(VcuControllerTest, GivenDrive_WhenBrakeAndThrottleTogether_ThenCurrentZero)
{
    goToDrive();
    in.pilot.throttlePercent = 0.5f;
    in.pilot.isBrakePressed = true;
    tick();
    EXPECT_FLOAT_EQ(out.vesc.targetAmp, 0.0f);
}

TEST_F(VcuControllerTest, GivenBrakeLatched_WhenBrakeReleasedButThrottleHeld_ThenCurrentStaysZero)
{
    goToDrive();
    in.pilot.throttlePercent = 0.5f;
    in.pilot.isBrakePressed = true;
    tick();

    in.pilot.isBrakePressed = false;
    tick();
    EXPECT_FLOAT_EQ(out.vesc.targetAmp, 0.0f);
}

TEST_F(VcuControllerTest, GivenThrottleHeldSincePowerOn_WhenEnteringDrive_ThenCurrentZero)
{
    in.pilot.throttlePercent = 0.8f;
    goToDrive();
    tick();
    EXPECT_FLOAT_EQ(out.vesc.targetAmp, 0.0f);
}

// ---- Reverse
TEST_F(VcuControllerTest, GivenStoppedWithBrake_WhenDirectionSwitchedBackward_ThenReverseSelected)
{
    tick();
    in.vesc.rpm = 0.0f;
    in.pilot.isBrakePressed = true;
    in.pilot.isForwardDirection = false;
    tick();
    EXPECT_TRUE(vcu.getIsInReverse());
}

TEST_F(VcuControllerTest, GivenNoBrake_WhenDirectionSwitchedBackward_ThenReverseNotSelected)
{
    tick();
    in.pilot.isForwardDirection = false;
    tick();
    EXPECT_FALSE(vcu.getIsInReverse());
}

TEST_F(VcuControllerTest,
       GivenVehicleMoving_WhenDirectionSwitchedBackwardWithBrake_ThenReverseNotSelected)
{
    tick();
    in.vesc.rpm = 500.0f;
    in.pilot.isBrakePressed = true;
    in.pilot.isForwardDirection = false;
    tick();
    EXPECT_FALSE(vcu.getIsInReverse());
}

TEST_F(VcuControllerTest, GivenReverseSelectedInDrive_WhenThrottlePressed_ThenCurrentIsNegative)
{
    tick();
    in.pilot.isBrakePressed = true;
    in.pilot.isForwardDirection = false;
    tick();
    goToDrive();
    EXPECT_TRUE(vcu.getIsInReverse());

    in.pilot.isBrakePressed = false;
    in.pilot.throttlePercent = 0.0f;
    tick();

    holdThrottle(0.5f);
    EXPECT_LT(out.vesc.targetAmp, 0.0f);
    EXPECT_GE(out.vesc.targetAmp, -Calibration::MAX_AMPS * 0.5f);
}

// ---- Full scenario
TEST_F(
    VcuControllerTest,
    GivenDriveAtStandstill_WhenStartReleasedAndVescPowersOff_ThenShutdownThenStandbyWithContactorOpen)
{
    goToDrive();

    in.pilot.isStartPressed = false;
    tick();
    EXPECT_EQ(state(), VcuStateId::SHUTDOWN);

    in.vesc.isOk = false;
    tick(300);
    EXPECT_EQ(state(), VcuStateId::STANDBY);
    EXPECT_FALSE(out.power.contactor);
}