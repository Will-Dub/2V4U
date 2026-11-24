#include "VcuController.hpp"

#include "Calibration.hpp"
#include "FaultCode.h"
#include "VcuStruct.hpp"

using namespace App::Logic;

VcuController::VcuController(ILogger& logger)
    : m_logger(logger), m_stateStandby(this, logger), m_statePrecharge(this, logger),
      m_stateDrive(this, logger), m_stateFault(this, logger), m_stateShutdown(this, logger),
      m_stateInit(this, logger)
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

void App::Logic::VcuController::requestFault(Shared::FaultCode code)
{
    if (m_faultCode == Shared::FaultCode::NONE) {
        m_faultCode = code;
    }
}

VcuStatus App::Logic::VcuController::getStatus() const
{
    return {getCurrentVcuStateId(), m_faultCode};
}

Shared::FaultCode App::Logic::VcuController::getFaultCode() const
{
    return m_faultCode;
}

bool App::Logic::VcuController::getIsInReverse() const
{
    return m_isInReverse;
}

bool App::Logic::VcuController::getIsBrakeThrottleLatched() const
{
    return m_isBrakeThrottleLatched;
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
    const bool isInDrive = (m_currentState->getId() == VcuStateId::DRIVE);
    const bool isInFault = m_currentState->getId() == VcuStateId::FAULT;
    Shared::FaultCode fault = Shared::FaultCode::NONE;

    // TODO: Go into fault directly for ESTOP

    // Check for fault
    if (!isInFault) {
        if (inputs.pilot.isEStopPressed)
            fault = Shared::FaultCode::ESTOP;
        else if (!inputs.pilot.isThrottleValid)
            fault = Shared::FaultCode::THROTTLE_INVALID;
        else if (!inputs.bms.isOk)
            fault = Shared::FaultCode::BMS_LOST;
        else if (inputs.bms.packCurrentA > Calibration::BMS_MAX_CURRENT_A)
            fault = Shared::FaultCode::BMS_HIGH_CURRENT;
        else if (inputs.bms.packVoltageV < Calibration::BMS_MIN_VOLTAGE_V)
            fault = Shared::FaultCode::BMS_LOW_VOLTAGE;
        else if(inputs.bms.protectionFlags)
            fault = Shared::FaultCode::BMS_PROTECTION;
        else if (isBmsTooHot(inputs.bms))
            fault = Shared::FaultCode::BMS_HIGH_TEMPERATURE;
        else if ((inputs.bms.cellMaxV - inputs.bms.cellMinV) > Calibration::BMS_MAX_CELL_DELTA_V)
            fault = Shared::FaultCode::BMS_HIGH_CELL_DELTA;
    }

    if (isInDrive && fault == Shared::FaultCode::NONE) {
        if (!inputs.vesc.isOk)
            fault = Shared::FaultCode::VESC_LOST;
        else if (inputs.vesc.inputVoltage < Calibration::VESC_MIN_VOLTAGE_V)
            fault = Shared::FaultCode::VESC_LOW_VOLTAGE;
        else if (inputs.vesc.inputCurrent > Calibration::VESC_MAX_CURRENT_A)
            fault = Shared::FaultCode::VESC_HIGH_CURRENT;
        else if (inputs.vesc.mosfetTemp > Calibration::VESC_MAX_MOSFET_TEMP_C || inputs.vesc.motorTemp > Calibration::VESC_MAX_MOTOR_TEMP_C)
            fault = Shared::FaultCode::VESC_HIGH_TEMPERATURE;
    }

    if (fault != Shared::FaultCode::NONE) {
        requestFault(fault);
        transitionToState(VcuStateId::SHUTDOWN, inputs);
    }

    // Pressing brake and throttle
    if (!inputs.pilot.isBrakePressed &&
        inputs.pilot.throttleRatio < Calibration::THROTTLE_RELEASE_THRESHOLD)
        m_isBrakeThrottleLatched = false;
    else if (inputs.pilot.isBrakePressed)
        m_isBrakeThrottleLatched = true;

    // Reverse and forward change
    if (inputs.vesc.rpm == 0.0f && inputs.pilot.isBrakePressed) {
        if (inputs.pilot.isForwardDirection) {
            m_isInReverse = false;
        } else {
            m_isInReverse = true;
        }
    }

    VcuStateId nextStateId = m_currentState->update(inputs, outputs);
    transitionToState(nextStateId, inputs);
}

bool VcuController::isBmsTooHot(const App::Logic::BmsData& bms)
{
    for (uint8_t i = 0; i < bms.tempCount; i++) {
        if (bms.tempC[i] > Calibration::BMS_MAX_TEMP_C)
            return true;
    }
    return false;
}
