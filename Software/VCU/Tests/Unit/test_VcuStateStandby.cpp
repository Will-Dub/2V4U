#include "../../App/Inc/Logic/VcuStateStandby.hpp"
#include "FakeLogger.hpp"

#include <gtest/gtest.h>

using namespace App::Logic;

class VcuStateStandbyTest : public ::testing::Test
{
  protected:
    FakeLogger logger;
    VcuStateStandby state{nullptr, logger};
    VcuInputs inputs{};
    VcuOutputs outputs{};

    void SetUp() override
    {
        inputs = VcuInputs{};
        outputs = VcuOutputs{};
    }
};

TEST_F(VcuStateStandbyTest, GivenStandbyState_WhenGetId_ThenReturnsStandby)
{
    EXPECT_EQ(state.getId(), VcuStateId::STANDBY);
}

TEST_F(VcuStateStandbyTest, GivenStartNotPressed_WhenUpdate_ThenStateSame)
{
    inputs.pilot.isStartPressed = false;
    EXPECT_EQ(state.update(inputs, outputs), VcuStateId::SAME);
}

TEST_F(VcuStateStandbyTest, GivenStartPressed_WhenUpdate_ThenGoesToPrecharge)
{
    inputs.pilot.isStartPressed = true;
    EXPECT_EQ(state.update(inputs, outputs), VcuStateId::PRECHARGE);
}

TEST_F(VcuStateStandbyTest, GivenFullThrottle_WhenUpdate_ThenNoTorqueAndRelaysOpen)
{
    inputs.pilot.throttlePercent = 1.0f;
    state.update(inputs, outputs);

    EXPECT_FLOAT_EQ(outputs.vesc.targetAmp, 0.0f);
    EXPECT_FALSE(outputs.power.contactor);
    EXPECT_FALSE(outputs.power.prechargeRelay);
}