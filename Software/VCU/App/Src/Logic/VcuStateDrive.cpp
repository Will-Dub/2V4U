#include "VcuStateDrive.hpp"

using namespace App::Logic;

void VcuStateDrive::onEnter(const VcuInputs& inputs)
{
    m_logger.debug("OnEnter: Drive");
}

VcuStateId VcuStateDrive::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    outputs.power.contactor = true;
    outputs.power.prechargeRelay = false;

    // TODO: Check user speed
    if (!inputs.pilot.isStartPressed) {
        return VcuStateId::SHUTDOWN;
    }

    if (inputs.vesc.inputVoltage < 35.0f) {
        return VcuStateId::FAULT;
    }

    return VcuStateId::SAME;
}

VcuStateId VcuStateDrive::getId()
{
    return VcuStateId::DRIVE;
}
