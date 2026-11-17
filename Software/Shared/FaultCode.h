#pragma once
#include "stdint.h"

namespace Shared
{
    enum class FaultCode : uint8_t
    {
        NONE = 0,
        ESTOP,
        THROTTLE_INVALID,
        VESC_LOST,
        BMS_LOST,
        PRECHARGE_TIMEOUT,
        LOW_BUS_VOLTAGE,
    };
}