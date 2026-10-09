#include "VcuStateInit.hpp"

using namespace App::Logic;

void VcuStateInit::onEnter(const VcuInputs& inputs)
{
    m_logger.debug("OnEnter: Init");
}

VcuStateId VcuStateInit::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    return VcuStateId::STANDBY;
}

VcuStateId VcuStateInit::getId()
{
    return VcuStateId::INIT;
}
