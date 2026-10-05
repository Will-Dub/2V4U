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

struct VcuInputs
{
    struct
    {
        bool isBtnBoardPressed = false;
        bool isStartPressed = false;
        bool isEStopPressed = true;
        bool isBrakePressed = false;
        bool isForwardDirection = false;
        float throttlePercent = 0.0f;
    } pilot;

    struct
    {
        bool isAlive = false;
        int32_t erpm = 0;
        float motorCurrent = 0.0f;
        float inputCurrent = 0.0f;
        float dutyCycle = 0.0f;
        float inputVoltage = 0.0f;
        float mosfetTemp = 0.0f;
        float motorTemp = 0.0f;
        uint8_t faultCode = 0;

        float ampHours = 0.0f;
        float ampHoursCharged = 0.0f;
        float wattHours = 0.0f;
        float wattHoursCharged = 0.0f;
    } vesc;

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
        bool enableMotor = false;
        float targetAmp = 0.0f;
    } motor;

    struct
    {
        bool boardLed = false;
        bool errorLed = false;
        bool startLed = false;
    } lights;
};

}; // namespace App::Logic