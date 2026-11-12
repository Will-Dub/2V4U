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

    outputs.vesc.isMotorEnabled = true;
    outputs.vesc.targetAmp = getTargetAmps(inputs);

    return VcuStateId::SAME;
}

VcuStateId VcuStateDrive::getId()
{
    return VcuStateId::DRIVE;
}

float VcuStateDrive::getTargetAmps(const VcuInputs& inputs)
{
    const float dt = (inputs.tickMs - m_lastTickMs) / 1000.0f;
    m_lastTickMs = inputs.tickMs;

    if (m_context->getIsBrakeThrottleLatched()) {
        m_forwardMap.reset();
        m_reverseMap.reset();
        return 0.0f;
    }

    if (m_context->getIsInReverse()) {
        m_forwardMap.reset();
        return -m_reverseMap.update(inputs.pilot.throttlePercent, dtS);
    }

    m_reverseMap.reset();
    return m_forwardMap.update(inputs.pilot.throttlePercent, dtS);
}