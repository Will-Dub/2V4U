#include "VcuController.hpp"

using namespace App::Logic;

VcuController::VcuController()
{
    m_currentState = &m_stateInit;
    m_currentState->onEnter();
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
        default:
            return nullptr;
    }
}

void VcuController::run(const VcuInputs& inputs, VcuOutputs& outputs)
{
    // TODO: Add fault check
    
    VcuStateId nextStateId = m_currentState->update(inputs, outputs);
    VcuState* nextState = getStateInstance(nextStateId);

    if (nextState != m_currentState) {
        m_currentState->onExit();
        m_currentState = nextState;
        m_currentState->onEnter();
    }
}
