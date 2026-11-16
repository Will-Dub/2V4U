#include "VcuStateShutdown.hpp"

#include "Calibration.hpp"

using namespace App::Logic;

void VcuStateShutdown::onEnter(const VcuInputs& inputs)
{
    m_logger.debugf("OnEnter: Shutdown");
}

VcuStateId VcuStateShutdown::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    outputs.vesc.isMotorEnabled = false;
    outputs.vesc.targetAmp = 0.0f;
    outputs.power.prechargeRelay = false;

    if (inputs.vesc.isOk && (inputs.vesc.motorCurrent > Calibration::SHUTDOWN_CURRENT_A ||
                             inputs.vesc.motorCurrent < -Calibration::SHUTDOWN_CURRENT_A)) {
        outputs.power.contactor = true;
        return VcuStateId::SAME;
    }

    outputs.power.contactor = false;

    if (!inputs.vesc.isOk) {
        return VcuStateId::STANDBY;
    }

    return VcuStateId::SAME;
}

VcuStateId VcuStateShutdown::getId()
{
    return VcuStateId::SHUTDOWN;
}
