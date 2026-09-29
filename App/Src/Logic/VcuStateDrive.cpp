#include "VcuStateDrive.hpp"

using namespace App::Logic;

void VcuStateDrive::onEnter() {}

VcuStateId VcuStateDrive::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    return VcuStateId::SAME;
}
