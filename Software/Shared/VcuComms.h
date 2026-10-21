#pragma once
#include <stdint.h>

namespace Shared
{

#pragma pack(push, 1)
    struct VcuStatusPacket
    {
        uint32_t tickMs;
        uint8_t currentState;

        // VESC
        int32_t rpm;
        float motorTemp;
        float mosfetTemp;

        // IMU
        float accelX;
        float accelY;
        float accelZ;

        // Pilot
        float throttlePercent;

        uint8_t statusFlags;
    };
#pragma pack(pop)
}