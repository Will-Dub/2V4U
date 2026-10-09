#include "VcuStatePrecharge.hpp"

using namespace App::Logic;

void VcuStatePrecharge::onEnter(const VcuInputs& inputs)
{
    m_logger.debug("OnEnter: Precharge");
    m_prechargeStartTimeMs = inputs.tickMs;
}

VcuStateId VcuStatePrecharge::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    outputs.power.prechargeRelay = true;
    outputs.power.contactor = false;

    if (inputs.vesc.inputVoltage > 46.0f) {
        return VcuStateId::DRIVE;
    }

    uint32_t currentTimeMs = inputs.tickMs;
    if (currentTimeMs - m_prechargeStartTimeMs >= 3000) {
        return VcuStateId::FAULT;
    }

    return VcuStateId::SAME;
}

VcuStateId VcuStatePrecharge::getId()
{
    return VcuStateId::PRECHARGE;
}
