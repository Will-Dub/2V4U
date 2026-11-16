#include "VcuStateFault.hpp"

#include "Calibration.hpp"

using namespace App::Logic;

void VcuStateFault::onEnter(const VcuInputs& inputs)
{
    m_logger.debugf("OnEnter: Fault");
}

VcuStateId VcuStateFault::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    outputs.vesc.isMotorEnabled = false;
    outputs.vesc.targetAmp = 0.0f;
    outputs.power.contactor = false;
    outputs.power.prechargeRelay = false;

    const bool isLedOn = ((inputs.tickMs / Calibration::FAULT_BLINK_MS) % 2) == 0;
    outputs.lights.errorLed = true;
    outputs.lights.boardLed = isLedOn;
    outputs.lights.startLed = isLedOn;

    outputs.buzzer.mode = BuzzerMode::BEEP_LOOP;
    outputs.buzzer.beepMs = Calibration::FAULT_BEEP_MS;
    outputs.buzzer.pauseMs = Calibration::FAULT_BEEP_PAUSE_MS;

    return VcuStateId::SAME;
}

VcuStateId VcuStateFault::getId()
{
    return VcuStateId::FAULT;
}
