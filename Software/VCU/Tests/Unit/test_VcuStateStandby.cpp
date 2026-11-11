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

TEST_F(VcuStateStandbyTest, ReturnsCorrectStateId)
{
    EXPECT_EQ(state.getId(), VcuStateId::STANDBY);
}

TEST_F(VcuStateStandbyTest, RemainsInStandbyWhenStartNotPressed)
{
    inputs.pilot.isStartPressed = false;
    EXPECT_EQ(state.update(inputs, outputs), VcuStateId::SAME);
}

TEST_F(VcuStateStandbyTest, TransitionsToPrechargeWhenStartPressed)
{
    inputs.pilot.isStartPressed = true;
    EXPECT_EQ(state.update(inputs, outputs), VcuStateId::PRECHARGE);
}

TEST_F(VcuStateStandbyTest, NoTorqueAndContactorOpenInStandby)
{
    inputs.pilot.throttlePercent = 1.0f;
    state.update(inputs, outputs);

    EXPECT_FLOAT_EQ(outputs.vesc.targetAmp, 0.0f);
    EXPECT_FALSE(outputs.power.contactor);
    EXPECT_FALSE(outputs.power.prechargeRelay);
}