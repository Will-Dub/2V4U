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
    SAME
};

struct VcuInputs
{
    struct
    {
        bool isBtnBoardPressed;
        bool isStartPressed;
        bool isEStopPressed;
        bool isBrakePressed;
        bool isForwardDirection;
        float throttlePercent;
    } pilot;

    struct
    {
        bool isAlive;
        int32_t erpm;
        float motorCurrent;
        float inputCurrent;
        float dutyCycle;
        float inputVoltage;
        float mosfetTemp;
        float motorTemp;
        uint8_t faultCode;

        float ampHours;
        float ampHoursCharged;
        float wattHours;
        float wattHoursCharged;
    } vesc;
};

struct VcuOutputs
{
    struct
    {
        bool prechargeRelay;
        bool contactor;
    } power;

    struct
    {
        bool enableMotor;
        float targetAmp;
    } motor;

    struct
    {
        bool boardLed;
        bool errorLed;
        bool startLed;
    } lights;
};

}; // namespace App::Logic