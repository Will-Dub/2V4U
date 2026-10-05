#include "VcuStateDrive.hpp"

using namespace App::Logic;

void VcuStateDrive::onEnter(const VcuInputs& inputs) {}

VcuStateId VcuStateDrive::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    if(!inputs.pilot.isStartPressed){
        return VcuStateId::SHUTDOWN;
    }

    return VcuStateId::SAME;
}
