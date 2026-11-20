#pragma once
#include "FaultCode.h"

#include <stdint.h>

namespace App::Logic {

enum class BuzzerMode : uint8_t
{
    OFF = 0,
    ON,
    BEEP_ONCE,
    BEEP_LOOP
};

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

struct VcuStatus
{
    VcuStateId stateId = VcuStateId::INIT;
    Shared::FaultCode faultCode = Shared::FaultCode::NONE;
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
    uint32_t followedTxFailCount = 0;
    uint32_t txFailCount = 0;
    uint32_t rxCount = 0;
    uint32_t txCount = 0;
    bool isOk = false;
    bool isTxDead = true;
    bool isRxDead = true;
};

struct BmsData
{
    static constexpr uint8_t MAX_CELLS = 16;
    static constexpr uint8_t MAX_TEMPS = 3;

    float packVoltageV = 0;
    float packCurrentA = 0;
    uint8_t socPercent = 0;

    float tempC[MAX_TEMPS]{};
    uint8_t tempCount = 0;

    uint16_t cellMv[MAX_CELLS]{};
    uint8_t cellCount = 0;
    float cellMinV = 0;
    float cellMaxV = 0;

    uint16_t protectionFlags = 0;
    uint16_t cycles = 0;

    uint32_t lastUpdateMs = 0;
    uint32_t rxErrorCount = 0;
    bool isValid = false;
    bool isOk = false;
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
        float throttleRatio = 0.0f;
        bool isThrottleValid = false;
    } pilot;

    struct
    {
        float Ax = 0.0f, Ay = 0.0f, Az = 1.0f;
        float Gx = 0.0f, Gy = 0.0f, Gz = 0.0f;
        float temp = 0.0f;
        bool isValid = false;
    } imu;

    VescData vesc;
    BmsData bms;

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

    struct
    {
        BuzzerMode mode = BuzzerMode::OFF;
        uint32_t beepMs = 0;
        uint32_t pauseMs = 0;
    } buzzer;
};

}; // namespace App::Logic