#include "VcuStateShutdown.hpp"

using namespace App::Logic;

void VcuStateShutdown::onEnter(const VcuInputs& inputs)
{
    m_logger.debug("OnEnter: Shutdown");
}

VcuStateId VcuStateShutdown::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    outputs.vesc.isMotorEnabled = false;
    outputs.vesc.targetAmp = 0.0f;

    if (inputs.vesc.motorCurrent > 1.0f) {
        outputs.power.contactor = true;
        outputs.power.prechargeRelay = false;
        return VcuStateId::SAME;
    }

    outputs.power.contactor = false;
    outputs.power.prechargeRelay = false;

    bool isRxDead = (inputs.tickMs - inputs.vesc.lastRxMs) > 250;
    bool isTxDead = (inputs.vesc.txFailCount > 10);
    bool isVescDead = isRxDead || isTxDead;

    if (isVescDead) {
        return VcuStateId::STANDBY;
    }

    return VcuStateId::SAME;
}

VcuStateId VcuStateShutdown::getId()
{
    return VcuStateId::SHUTDOWN;
}
