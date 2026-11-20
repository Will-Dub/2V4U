#include "DashTransmitter.hpp"

#include <stdio.h>

static_assert(App::Logic::BmsData::MAX_CELLS == Shared::BMS_CELL_COUNT,
              "BMS cell count differs from the dash packet");
static_assert(App::Logic::BmsData::MAX_TEMPS == Shared::BMS_TEMP_COUNT,
              "BMS temp count differs from the dash packet");

namespace App::Infrastructure {

void DashTransmitter::process(const App::Logic::VcuStatus& status,
                              const App::Logic::VcuInputs& inputs,
                              const App::Logic::VcuOutputs& outputs)
{
    sendVcu(status, inputs, outputs);
    sendBms(inputs);
}

void DashTransmitter::sendVcu(const App::Logic::VcuStatus& status,
                              const App::Logic::VcuInputs& inputs,
                              const App::Logic::VcuOutputs& outputs)
{
    if ((inputs.tickMs - m_lastVcuTxMs) < INTERVAL_VCU_MS)
        return;

    m_lastVcuTxMs = inputs.tickMs;

    Shared::VcuStatusPacket packet;

    packet.tickMs = inputs.tickMs;
    packet.currentState = static_cast<uint8_t>(status.stateId);
    packet.faultCode = static_cast<uint8_t>(status.faultCode);
    packet.rpm = inputs.vesc.rpm;
    packet.motorTemp = inputs.vesc.motorTemp;
    packet.mosfetTemp = inputs.vesc.mosfetTemp;
    packet.motorCurrent = inputs.vesc.motorCurrent;
    packet.inputCurrent = inputs.vesc.inputCurrent;
    packet.inputVoltage = inputs.vesc.inputVoltage;
    packet.accelX = inputs.imu.Ax;
    packet.accelY = inputs.imu.Ay;
    packet.accelZ = inputs.imu.Az;
    packet.throttleRatio = inputs.pilot.throttleRatio;
    packet.canRxCount = inputs.vesc.rxCount;
    packet.canTxCount = inputs.vesc.txCount;
    packet.canTxFailCount = inputs.vesc.txFailCount;
    packet.inputStatusFlags = 0;
    packet.outputStatusFlags = 0;

    // Input flags
    if (inputs.pilot.isBrakePressed)
        packet.inputStatusFlags |= (1 << 0);
    if (inputs.pilot.isStartPressed)
        packet.inputStatusFlags |= (1 << 1);
    if (inputs.pilot.isEStopPressed)
        packet.inputStatusFlags |= (1 << 2);
    if (inputs.pilot.isForwardDirection)
        packet.inputStatusFlags |= (1 << 3);
    if (inputs.pilot.isThrottleValid)
        packet.inputStatusFlags |= (1 << 4);

    // Output flags
    if (outputs.vesc.isMotorEnabled)
        packet.outputStatusFlags |= (1 << 0);
    if (outputs.power.prechargeRelay)
        packet.outputStatusFlags |= (1 << 1);
    if (outputs.power.contactor)
        packet.outputStatusFlags |= (1 << 2);

    printf("$VCU,%lu,%u,%u,%ld,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%u,%u,%.2f,%.2f,%.2f,%lu,%lu,%lu\n",
           packet.tickMs, packet.currentState, packet.faultCode, packet.rpm, packet.motorTemp,
           packet.mosfetTemp, packet.accelX, packet.accelY, packet.accelZ, packet.throttleRatio,
           packet.inputStatusFlags, packet.outputStatusFlags, packet.motorCurrent,
           packet.inputCurrent, packet.inputVoltage, packet.canRxCount, packet.canTxCount,
           packet.canTxFailCount);
}

void DashTransmitter::sendBms(const App::Logic::VcuInputs& inputs)
{
    if ((inputs.tickMs - m_lastBmsTxMs) < INTERVAL_BMS_MS)
        return;

    m_lastBmsTxMs = inputs.tickMs;

    const App::Logic::BmsData& bms = inputs.bms;

    printf("$BMS,%u,%.2f,%.2f", bms.isOk ? 1 : 0, bms.packVoltageV, bms.packCurrentA);

    // Temperature
    for (uint8_t i = 0; i < Shared::BMS_TEMP_COUNT; i++) {
        const float temp = (i < bms.tempCount) ? bms.tempC[i] : Shared::BMS_TEMP_MISSING;
        printf(",%.1f", temp);
    }

    // Cell voltages in mV
    for (uint8_t i = 0; i < App::Logic::BmsData::MAX_CELLS; i++) {
        printf(",%u", bms.cellMv[i]);
    }

    printf("\n");
}
} // namespace App::Infrastructure
