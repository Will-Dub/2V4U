#pragma once
#include "ILogger.hpp"
#include "VcuStateDrive.hpp"
#include "VcuStateFault.hpp"
#include "VcuStateInit.hpp"
#include "VcuStatePrecharge.hpp"
#include "VcuStateShutdown.hpp"
#include "VcuStateStandby.hpp"

namespace App::Logic {
class VcuController
{
  private:
    ILogger& m_logger;
    VcuStateStandby m_stateStandby;
    VcuStatePrecharge m_statePrecharge;
    VcuStateDrive m_stateDrive;
    VcuStateFault m_stateFault;
    VcuStateShutdown m_stateShutdown;
    VcuStateInit m_stateInit;
    VcuState* m_currentState;

    VcuState* getStateInstance(VcuStateId id);
    void transitionToState(VcuStateId nextStateId, const VcuInputs& inputs);

  public:
    VcuController(ILogger& logger);
    void run(const VcuInputs& inputs, VcuOutputs& outputs);
    void init(const VcuInputs& inputs);
};
} // namespace App::Logic