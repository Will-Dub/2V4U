#include "VcuStatePark.hpp"

using namespace App::Logic;

void VcuStatePark::onEnter() {}

VcuStateId VcuStatePark::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    return VcuStateId::SAME;
}
