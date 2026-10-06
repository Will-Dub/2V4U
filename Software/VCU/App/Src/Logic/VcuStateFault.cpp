#include "VcuStateFault.hpp"

using namespace App::Logic;

void VcuStateFault::onEnter(const VcuInputs& inputs)
{
    m_logger.debug("OnEnter: Fault");
    m_blinkTimer = 0;
}

VcuStateId VcuStateFault::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    outputs.motor.enableMotor = false;
    outputs.motor.targetAmp = 0.0f;
    outputs.power.contactor = false;
    outputs.power.prechargeRelay = false;
    outputs.lights.errorLed = true;
    outputs.lights.boardLed = m_isBoardLedOn;

    m_blinkTimer++;
    if (m_blinkTimer % 50 == 0) {
        m_isBoardLedOn = !m_isBoardLedOn;
    }

    return VcuStateId::SAME;
}
