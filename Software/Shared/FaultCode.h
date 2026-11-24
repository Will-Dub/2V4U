#pragma once
#include "stdint.h"

namespace Shared
{
    enum class FaultCode : uint8_t
    {
        NONE = 0,
        ESTOP,
        THROTTLE_INVALID,
        PRECHARGE_TIMEOUT,
        VESC_LOST,
        VESC_LOW_VOLTAGE,
        VESC_HIGH_CURRENT,
        VESC_HIGH_TEMPERATURE,
        BMS_LOST,
        BMS_HIGH_CELL_DELTA,
        BMS_HIGH_CURRENT,
        BMS_PROTECTION,
        BMS_LOW_VOLTAGE,
        BMS_HIGH_TEMPERATURE,
        INVALID,
    };
}