#include "VcuController.hpp"

using namespace App::Logic;

VcuController::VcuController()
{
    m_currentState = &m_stateInit;
}

void VcuController::init(const VcuInputs& inputs) {
    m_currentState->onEnter(inputs);
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

void VcuController::transitionToState(VcuStateId nextStateId, const VcuInputs& inputs){
    VcuState* nextState = getStateInstance(nextStateId);

    if (nextState != m_currentState) {
        m_currentState->onExit();
        m_currentState = nextState;
        m_currentState->onEnter(inputs);
    }
}

void VcuController::run(const VcuInputs& inputs, VcuOutputs& outputs)
{
    if(inputs.pilot.isEStopPressed) {
        transitionToState(VcuStateId::FAULT, inputs);
    }
    
    VcuStateId nextStateId = m_currentState->update(inputs, outputs);
    transitionToState(nextStateId, inputs);
}
