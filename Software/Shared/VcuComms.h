#pragma once
#include <stdint.h>

namespace Shared
{
    constexpr uint8_t BMS_CELL_COUNT = 16;
    constexpr uint8_t BMS_TEMP_COUNT = 3;
    constexpr float BMS_TEMP_MISSING = -99.0f;

#pragma pack(push, 1)
    struct VcuStatusPacket
    {
        uint32_t tickMs;
        uint8_t currentState;
        uint8_t faultCode;

        // VESC
        int32_t rpm;
        float motorTemp;
        float mosfetTemp;
        float motorCurrent;
        float inputCurrent;
        float inputVoltage;

        // IMU
        float accelX;
        float accelY;
        float accelZ;

        // Pilot
        float throttlePercent;

        // CAN
        uint32_t canRxCount;
        uint32_t canTxCount;
        uint32_t canTxFailCount;

        uint8_t inputStatusFlags;
        uint8_t outputStatusFlags;
    };
#pragma pack(pop)
}