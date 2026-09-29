#pragma once
#include "VcuStateDrive.hpp"
#include "VcuStateFault.hpp"
#include "VcuStateInit.hpp"
#include "VcuStatePark.hpp"

namespace App::Logic {
class VcuController
{
  private:
    VcuStateInit m_stateInit;
    VcuStatePark m_statePark;
    VcuStateDrive m_stateDrive;
    VcuStateFault m_stateFault;
    VcuState* m_currentState;

    VcuState* getStateInstance(VcuStateId id);

  public:
    VcuController();
    void run(const VcuInputs& inputs, VcuOutputs& outputs);
};
} // namespace App::Logic