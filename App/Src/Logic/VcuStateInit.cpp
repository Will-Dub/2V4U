#include "VcuStateInit.hpp"

using namespace App::Logic;

void VcuStateInit::onEnter() {}

VcuStateId VcuStateInit::update(const VcuInputs& inputs, VcuOutputs& outputs)
{
    outputs.lights.boardLed = inputs.pilot.isBtnBoardPressed;
    outputs.power.contactor = inputs.pilot.isBtnBoardPressed;
    return VcuStateId::SAME;
}
