#include "VcuStateDrive.hpp"

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

    return VcuStateId::SAME;
}

VcuStateId VcuStateDrive::getId()
{
    return VcuStateId::DRIVE;
}
