#include "../../App/Inc/Logic/VcuController.hpp"
#include "FakeLogger.hpp"

#include <gtest/gtest.h>

using namespace App::Logic;

class VcuStateDriveTest : public ::testing::Test
{
  protected:
    FakeLogger logger;
    VcuController controller{logger};
    VcuStateDrive state{&controller, logger};
    VcuInputs in{};
    VcuOutputs out{};

    void SetUp() override
    {
        in.vesc.inputVoltage = 50.0f;
        in.pilot.isStartPressed = true;
    }
};

TEST_F(VcuStateDriveTest, GivenDriveState_WhenGetId_ThenReturnsDrive)
{
    EXPECT_EQ(state.getId(), VcuStateId::DRIVE);
}

TEST_F(VcuStateDriveTest, GivenHealthyBus_WhenUpdate_ThenContactorClosedAndPrechargeOpen)
{
    state.update(in, out);
    EXPECT_TRUE(out.power.contactor);
    EXPECT_FALSE(out.power.prechargeRelay);
}

TEST_F(VcuStateDriveTest, GivenBusVoltageBelow35V_WhenUpdate_ThenGoesToFault)
{
    in.vesc.inputVoltage = 34.9f;
    EXPECT_EQ(state.update(in, out), VcuStateId::FAULT);
}

TEST_F(VcuStateDriveTest, GivenBusVoltageExactly35V_WhenUpdate_ThenStateSame)
{
    in.vesc.inputVoltage = 35.0f;
    EXPECT_EQ(state.update(in, out), VcuStateId::SAME);
}

TEST_F(VcuStateDriveTest, GivenStartReleasedAtStandstill_WhenUpdate_ThenGoesToShutdown)
{
    in.pilot.isStartPressed = false;
    in.vesc.rpm = 0.0f;
    EXPECT_EQ(state.update(in, out), VcuStateId::SHUTDOWN);
}

TEST_F(VcuStateDriveTest, GivenStartReleasedWhileMoving_WhenUpdate_ThenStateSame)
{
    in.pilot.isStartPressed = false;
    in.vesc.rpm = 500.0f;
    EXPECT_EQ(state.update(in, out), VcuStateId::SAME);
}