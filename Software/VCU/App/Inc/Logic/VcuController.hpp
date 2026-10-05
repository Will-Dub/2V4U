#pragma once
#include "VcuStateDrive.hpp"
#include "VcuStateFault.hpp"
#include "VcuStateInit.hpp"
#include "VcuStateStandby.hpp"
#include "VcuStatePrecharge.hpp"
#include "VcuStateShutdown.hpp"

namespace App::Logic {
class VcuController
{
  private:
    VcuStateInit m_stateInit;
    VcuStateStandby m_stateStandby;
    VcuStatePrecharge m_statePrecharge;
    VcuStateDrive m_stateDrive;
    VcuStateFault m_stateFault;
    VcuStateShutdown m_stateShutdown;
    VcuState* m_currentState;

    VcuState* getStateInstance(VcuStateId id);
    void transitionToState(VcuStateId nextStateId, const VcuInputs& inputs);

  public:
    VcuController();
    void run(const VcuInputs& inputs, VcuOutputs& outputs);
    void init(const VcuInputs& inputs);
};
} // namespace App::Logic