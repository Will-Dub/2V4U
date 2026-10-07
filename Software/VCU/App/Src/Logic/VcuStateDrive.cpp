#include "VcuStateDrive.hpp"

using namespace App::Logic;

void VcuStateDrive::onEnter(const VcuInputs& inputs)
{
    m_logger.debug("OnEnter: Drive");
}

VcuStateId VcuStateDrive::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    if (!inputs.pilot.isStartPressed) {
        return VcuStateId::SHUTDOWN;
    }

    return VcuStateId::SAME;
}
