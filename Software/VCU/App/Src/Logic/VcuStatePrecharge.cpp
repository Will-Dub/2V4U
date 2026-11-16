#include "VcuStatePrecharge.hpp"

#include "Calibration.hpp"

using namespace App::Logic;

void VcuStatePrecharge::onEnter(const VcuInputs& inputs)
{
    m_logger.debugf("OnEnter: Precharge");
    m_prechargeStartTimeMs = inputs.tickMs;
}

VcuStateId VcuStatePrecharge::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    outputs.power.prechargeRelay = true;
    outputs.power.contactor = false;

    if (inputs.vesc.inputVoltage > Calibration::PRECHARGE_DONE_VOLTAGE) {
        return VcuStateId::DRIVE;
    }

    uint32_t currentTimeMs = inputs.tickMs;
    if (currentTimeMs - m_prechargeStartTimeMs >= Calibration::PRECHARGE_TIMEOUT_MS) {
        return VcuStateId::FAULT;
    }

    return VcuStateId::SAME;
}

VcuStateId VcuStatePrecharge::getId()
{
    return VcuStateId::PRECHARGE;
}
