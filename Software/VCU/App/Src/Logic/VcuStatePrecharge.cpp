#include "VcuStatePrecharge.hpp"

using namespace App::Logic;

void VcuStatePrecharge::onEnter() {
    m_prechargeStartTimeMs = HAL_GetTick();
}

VcuStateId VcuStatePrecharge::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    uint32_t currentTimeMs = HAL_GetTick();

    // TODO: Add can message check
    if (currentTimeMs - m_prechargeStartTimeMs >= 2000) {
        return VcuStateId::DRIVE;
    }

    return VcuStateId::SAME;
}
