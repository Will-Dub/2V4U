#include "VcuController.hpp"

#include "VcuStruct.hpp"

using namespace App::Logic;

VcuController::VcuController(ILogger& logger)
    : m_logger(logger), m_stateStandby(this, logger), m_statePrecharge(this, logger), m_stateDrive(this, logger),
      m_stateFault(this, logger), m_stateShutdown(this, logger), m_stateInit(this, logger)
{
    m_currentState = &m_stateInit;
}

void VcuController::init(const VcuInputs& inputs)
{
    m_currentState->onEnter(inputs);
}

VcuStateId App::Logic::VcuController::getCurrentVcuStateId() const
{
    return m_currentState->getId();
}

VcuState* VcuController::getStateInstance(VcuStateId id)
{
    switch (id) {
        case VcuStateId::INIT:
            return &m_stateInit;
        case VcuStateId::STANDBY:
            return &m_stateStandby;
        case VcuStateId::PRECHARGE:
            return &m_statePrecharge;
        case VcuStateId::DRIVE:
            return &m_stateDrive;
        case VcuStateId::FAULT:
            return &m_stateFault;
        case VcuStateId::SAME:
            return m_currentState;
        case VcuStateId::SHUTDOWN:
            return &m_stateShutdown;
        default:
            return nullptr;
    }
}

void VcuController::transitionToState(VcuStateId nextStateId, const VcuInputs& inputs)
{
    VcuState* nextState = getStateInstance(nextStateId);

    if (nextState != m_currentState) {
        m_currentState->onExit();
        m_currentState = nextState;
        m_currentState->onEnter(inputs);
    }
}

void VcuController::run(const VcuInputs& inputs, VcuOutputs& outputs)
{
    bool isRxDead = (inputs.tickMs - inputs.vesc.lastRxMs) > 250;
    bool isTxDead = (inputs.vesc.followedTxFailCount > 10);
    bool isVescDead = isRxDead || isTxDead;
    bool requiresVesc = (m_currentState->getId() == VcuStateId::DRIVE);
    bool isInFault = m_currentState->getId() == VcuStateId::FAULT;

    if (inputs.pilot.isEStopPressed && !isInFault) {
        transitionToState(VcuStateId::FAULT, inputs);
    }

    if (isVescDead && requiresVesc) {
        transitionToState(VcuStateId::FAULT, inputs);
    }

    if (!inputs.pilot.isThrottleValid && !isInFault) {
        transitionToState(VcuStateId::FAULT, inputs);
    }

    if (inputs.pilot.isThrottleValid == false) {
        transitionToState(VcuStateId::FAULT, inputs);
    }

    // Reverse and forward logic
    if(inputs.vesc.rpm == 0.0f && inputs.pilot.isBrakePressed){
        if(inputs.pilot.isForwardDirection){
            m_isInReverse = false;
        } else {
            m_isInReverse = true;
        }   
    }

    VcuStateId nextStateId = m_currentState->update(inputs, outputs);
    transitionToState(nextStateId, inputs);
}
