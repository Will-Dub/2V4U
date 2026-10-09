#include "VcuStateStandby.hpp"

using namespace App::Logic;

void VcuStateStandby::onEnter(const VcuInputs& inputs)
{
    m_logger.debug("OnEnter: Standby");
}

VcuStateId VcuStateStandby::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    if (inputs.pilot.isStartPressed) {
        return VcuStateId::PRECHARGE;
    }

    return VcuStateId::SAME;
}

VcuStateId VcuStateStandby::getId()
{
    return VcuStateId::STANDBY;
}
