#include "VcuStateStandby.hpp"

using namespace App::Logic;

void VcuStateStandby::onEnter() {}

VcuStateId VcuStateStandby::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    if(inputs.pilot.isStartPressed && !inputs.pilot.isEStopPressed) {
        return VcuStateId::PRECHARGE;
    }

    return VcuStateId::SAME;
}
