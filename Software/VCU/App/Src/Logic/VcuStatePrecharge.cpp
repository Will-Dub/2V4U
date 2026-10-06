#include "VcuStatePrecharge.hpp"

using namespace App::Logic;

void VcuStatePrecharge::onEnter(const VcuInputs& inputs)
{
    m_logger.debug("OnEnter: Precharge");
    m_prechargeStartTimeMs = inputs.tickMs;
}

VcuStateId VcuStatePrecharge::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    // TODO: Add can message check

    outputs.power.prechargeRelay = true;

    uint32_t currentTimeMs = inputs.tickMs;
    if (currentTimeMs - m_prechargeStartTimeMs >= 2000) {
        return VcuStateId::DRIVE;
    }

    return VcuStateId::SAME;
}
