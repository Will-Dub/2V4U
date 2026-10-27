#include "VcuStateDrive.hpp"
#include "VcuController.hpp"

using namespace App::Logic;

void VcuStateDrive::onEnter(const VcuInputs& inputs)
{
    m_logger.debugf("OnEnter: Drive");
}

VcuStateId VcuStateDrive::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    outputs.power.contactor = true;
    outputs.power.prechargeRelay = false;

    if (inputs.vesc.inputVoltage < 35.0f) {
        return VcuStateId::FAULT;
    }

    // Shutdown state logic
    if (!inputs.pilot.isStartPressed && inputs.vesc.rpm == 0.0f) {
        return VcuStateId::SHUTDOWN;
    }

    outputs.vesc.targetAmp = getTargetAmps(inputs);

    return VcuStateId::SAME;
}

VcuStateId VcuStateDrive::getId()
{
    return VcuStateId::DRIVE;
}

float VcuStateDrive::getTargetAmps(const VcuInputs& inputs)
{
    float targetAmps = inputs.pilot.throttlePercent * MAX_AMPERAGE;

    if(m_context->getIsInReverse()) {
        targetAmps *= -1.0f;
    }

    return targetAmps;
}