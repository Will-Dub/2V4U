#pragma once
#include "VcuStateDrive.hpp"
#include "VcuStateFault.hpp"
#include "VcuStateInit.hpp"
#include "VcuStateStandby.hpp"

namespace App::Logic {
class VcuController
{
  private:
    VcuStateInit m_stateInit;
    VcuStateStandby m_stateStandby;
    VcuStateDrive m_stateDrive;
    VcuStateFault m_stateFault;
    VcuState* m_currentState;

    VcuState* getStateInstance(VcuStateId id);

  public:
    VcuController();
    void run(const VcuInputs& inputs, VcuOutputs& outputs);
};
} // namespace App::Logic