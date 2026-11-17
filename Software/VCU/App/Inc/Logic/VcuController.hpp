#pragma once
#include "ILogger.hpp"
#include "VcuStateDrive.hpp"
#include "VcuStateFault.hpp"
#include "VcuStateInit.hpp"
#include "VcuStatePrecharge.hpp"
#include "VcuStateShutdown.hpp"
#include "VcuStateStandby.hpp"
#include "VcuStruct.hpp"

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
    bool m_isInReverse = false;
    bool m_isBrakeThrottleLatched = true;
    Shared::FaultCode m_faultCode = Shared::FaultCode::NONE;

    VcuState* getStateInstance(VcuStateId id);
    void transitionToState(VcuStateId nextStateId, const VcuInputs& inputs);

  public:
    VcuController(ILogger& logger);
    void run(const VcuInputs& inputs, VcuOutputs& outputs);
    void init(const VcuInputs& inputs);
    VcuStateId getCurrentVcuStateId() const;
    void requestFault(Shared::FaultCode code);
    VcuStatus getStatus() const;
    Shared::FaultCode getFaultCode() const;
    bool getIsInReverse() const;
    bool getIsBrakeThrottleLatched() const;
};
} // namespace App::Logic