#include "DashTransmitter.hpp"

#include "VcuStruct.hpp"

void App::Infrastructure::DashTransmitter::process(App::Logic::VcuStateId currentState,
                                                   const App::Logic::VcuInputs& inputs,
                                                   const App::Logic::VcuOutputs& outputs)
{
    if (1 == 1) {
        m_lastTxMs = inputs.tickMs;

        Shared::VcuStatusPacket packet;

        packet.tickMs = inputs.tickMs;
        packet.currentState = static_cast<uint8_t>(currentState);
        packet.rpm = inputs.vesc.rpm;
        packet.motorTemp = inputs.vesc.motorTemp;
        packet.mosfetTemp = inputs.vesc.mosfetTemp;
        packet.accelX = inputs.imu.Ax;
        packet.accelY = inputs.imu.Ay;
        packet.accelZ = inputs.imu.Az;
        packet.throttlePercent = inputs.pilot.throttlePercent;
        packet.statusFlags = 0;

        if (inputs.pilot.isBrakePressed)
            packet.statusFlags |= (1 << 0); // Bit 0
        if (inputs.pilot.isStartPressed)
            packet.statusFlags |= (1 << 1); // Bit 1
        if (inputs.pilot.isEStopPressed)
            packet.statusFlags |= (1 << 2); // Bit 2
        if (outputs.vesc.isMotorEnabled)
            packet.statusFlags |= (1 << 3); // Bit 3
        if (outputs.power.prechargeRelay)
            packet.statusFlags |= (1 << 4); // Bit 4
        if (outputs.power.contactor)
            packet.statusFlags |= (1 << 5); // Bit 5

        // TODO: Send as bytes
        printf("$VCU,%lu,%u,%ld,%.2f,%.2f,%.2f,%.2f,%.2f,%.1f,%u\n", packet.tickMs,
               packet.currentState, packet.rpm, packet.motorTemp, packet.mosfetTemp, packet.accelX,
               packet.accelY, packet.accelZ, packet.throttlePercent, packet.statusFlags);
    }
}
