#include "VcuStateFault.hpp"

using namespace App::Logic;

void VcuStateFault::onEnter(const VcuInputs& inputs)
{
    m_logger.debugf("OnEnter: Fault");
    m_blinkTimer = 0;
}

VcuStateId VcuStateFault::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    outputs.vesc.isMotorEnabled = false;
    outputs.vesc.targetAmp = 0.0f;
    outputs.power.contactor = false;
    outputs.power.prechargeRelay = false;
    outputs.lights.errorLed = true;
    outputs.lights.boardLed = m_isBoardLedOn;
    outputs.buzzer.mode = Logic::BuzzerMode::BEEP_LOOP;
    outputs.buzzer.beepMs = 50;
    outputs.buzzer.pauseMs = 2000;

    m_blinkTimer++;
    if (m_blinkTimer % 50 == 0) {
        m_isBoardLedOn = !m_isBoardLedOn;
    }

    return VcuStateId::SAME;
}

VcuStateId VcuStateFault::getId()
{
    return VcuStateId::FAULT;
}
