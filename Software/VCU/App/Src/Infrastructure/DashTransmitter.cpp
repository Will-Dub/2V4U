#include "DashTransmitter.hpp"

#include "VcuStruct.hpp"

namespace App::Infrastructure {

void DashTransmitter::process(App::Logic::VcuStateId currentState,
                              const App::Logic::VcuInputs& inputs,
                              const App::Logic::VcuOutputs& outputs)
{
    m_lastVcuTxMs = inputs.tickMs;
    m_lastBmsTxMs = inputs.tickMs;

    sendVcu(currentState, inputs, outputs);
    sendBms(inputs);
}

void DashTransmitter::sendVcu(App::Logic::VcuStateId currentState,
                              const App::Logic::VcuInputs& inputs,
                              const App::Logic::VcuOutputs& outputs)
{
    if ((inputs.tickMs - m_lastVcuTxMs) < INTERVAL_VCU_MS)
        return;

    m_lastVcuTxMs = inputs.tickMs;

    Shared::VcuStatusPacket packet;

    packet.tickMs = inputs.tickMs;
    packet.currentState = static_cast<uint8_t>(currentState);
    packet.rpm = inputs.vesc.rpm;
    packet.motorTemp = inputs.vesc.motorTemp;
    packet.mosfetTemp = inputs.vesc.mosfetTemp;
    packet.motorCurrent = inputs.vesc.motorCurrent;
    packet.inputCurrent = inputs.vesc.inputCurrent;
    packet.inputVoltage = inputs.vesc.inputVoltage;
    packet.accelX = inputs.imu.Ax;
    packet.accelY = inputs.imu.Ay;
    packet.accelZ = inputs.imu.Az;
    packet.throttlePercent = inputs.pilot.throttlePercent;
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

    printf("$VCU,%lu,%u,%ld,%.2f,%.2f,%.2f,%.2f,%.2f,%.1f,%u,%u,%.2f,%.2f,%.2f,%lu,%lu,%lu\n",
           packet.tickMs, packet.currentState, packet.rpm, packet.motorTemp, packet.mosfetTemp,
           packet.accelX, packet.accelY, packet.accelZ, packet.throttlePercent,
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

    float temp = 0.0f;
    if (bms.tempCount > 0)
        temp = bms.tempC[0];

    printf("$BMS,%.2f,%.2f,%.1f", bms.packVoltageV, bms.packCurrentA, temp);

    // Cell voltages in mV
    for (uint8_t i = 0; i < App::Logic::BmsData::kMaxCells; i++) {
        printf(",%u", bms.cellMv[i]);
    }

    printf("\n");
}
} // namespace App::Infrastructure
