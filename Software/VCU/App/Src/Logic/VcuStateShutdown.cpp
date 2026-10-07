#include "VcuStateShutdown.hpp"

using namespace App::Logic;

void VcuStateShutdown::onEnter(const VcuInputs& inputs)
{
    m_logger.debug("OnEnter: Shutdown");
}

VcuStateId VcuStateShutdown::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    outputs.power.contactor = true;
    outputs.power.prechargeRelay = false;

    outputs.motor.enableMotor = false;
    outputs.motor.targetAmp = 0.0f;

    bool isCurrentZero = (inputs.vesc.motorCurrent < 1.0f);

    // TODO: Add a timeout?

    if (isCurrentZero) {
        return VcuStateId::STANDBY;
    }

    return VcuStateId::SAME;
}
