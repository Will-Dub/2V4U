#pragma once
#include <stdint.h>

namespace App::Logic {

enum class VcuStateId
{
    INIT,
    STANDBY,
    PRECHARGE,
    DRIVE,
    FAULT,
    SHUTDOWN,
    SAME,
};

struct VescData
{
    int32_t rpm;
    float motorCurrent = 0;
    float inputCurrent = 0;
    float inputVoltage = 0;
    float mosfetTemp = 0;
    float motorTemp = 0;
    uint32_t lastRxMs = 0;
    uint32_t txFailCount = 0;
};

struct VcuInputs
{
    struct
    {
        bool isBrakePressed = false;
        bool isBtnBoardPressed = false;
        bool isEStopPressed = true;
        bool isForwardDirection = false;
        bool isStartPressed = false;
        bool isStartRisingEdge = false;
        float throttlePercent = 0.0f;
        bool isThrottleValid = false;
    } pilot;

    VescData vesc;

    uint32_t tickMs = 0;
};

struct VcuOutputs
{
    struct
    {
        bool prechargeRelay = false;
        bool contactor = false;
    } power;

    struct
    {
        bool isMotorEnabled = false;
        float targetAmp = 0.0f;
    } vesc;

    struct
    {
        bool boardLed = false;
        bool errorLed = false;
        bool startLed = false;
    } lights;
};

}; // namespace App::Logic