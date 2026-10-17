#include "App.h"

#include "DebounceButton.hpp"
#include "ImuDriver.hpp"
#include "StmLogger.hpp"
#include "ThrottleHallSensor.hpp"
#include "VcuController.hpp"
#include "VcuStateInit.hpp"
#include "VcuStruct.hpp"
#include "VescCanDriver.hpp"
#include "main.h"

#include <math.h>
#include <stdio.h>

extern SPI_HandleTypeDef hspi2;

static App::Infrastructure::StmLogger stmLogger;
static App::Logic::VcuController vcuController(stmLogger);
static App::Drivers::ThrottleHallSensor throttleSensor;
static App::Drivers::DebounceButton brakeButton;
static App::Drivers::DebounceButton boardButton;
static App::Drivers::DebounceButton eStopButton;
static App::Drivers::DebounceButton forwardButton;
static App::Drivers::DebounceButton startButton;
static App::Drivers::ImuDriver imuDriver(hspi2, SPI2_IMU_NCS_GPIO_Port, SPI2_IMU_NCS_Pin);
volatile bool tick = false;
volatile bool imuDataReady = false;

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == IMU_INT_Pin) {
        imuDataReady = true;
    }
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM6) {
        tick = true;
    }
}

extern "C" void App_Init(volatile uint16_t* throttleDmaPtr)
{
    App::Drivers::vescDriver.init();
    throttleSensor.init(throttleDmaPtr);
    imuDriver.init();

    App::Logic::VcuInputs inputs;
    inputs.tickMs = HAL_GetTick();
    vcuController.init(inputs);
}

extern "C" void App_Run(void)
{
    App::Drivers::vescDriver.processRb();

    if (!tick)
        return;

    tick = false;

    static float imuAx = 0.0f, imuAy = 0.0f, imuAz = 1.0f;
    static float imuGx = 0.0f, imuGy = 0.0f, imuGz = 0.0f;
    static float imuTemp = 0.0f;
    static bool imuIsDataValid = false;

    if (imuDataReady) {
        imuDataReady = false;

        imuIsDataValid = imuDriver.readSensorData(imuAx, imuAy, imuAz, imuGx, imuGy, imuGz,
                                                  imuTemp);

        if (!imuIsDataValid) {
            imuAx = 0.0f;
            imuAy = 0.0f;
            imuAz = 1.0f;
            imuGx = 0.0f;
            imuGy = 0.0f;
            imuGz = 0.0f;
        }
    }

    static uint32_t lastLogMs = 0;

    uint32_t currentTickMs = HAL_GetTick();
    if (currentTickMs - lastLogMs >= 500) {
        lastLogMs = currentTickMs;

        if (imuIsDataValid) {
            stmLogger.debugf("\n-----IMU----\nAx: %5.2f | Ay: %5.2f | Az: %5.2f\n"
                             "Gx: %5.2f | Gy: %5.2f | Gz: %5.2f\nTemp: %.1f C\n",
                             imuAx, imuAy, imuAz, imuGx, imuGy, imuGz, imuTemp);
        } else {
            stmLogger.errorf("INVALID IMU DATA - SENSOR LOST");
        }
    }

    /*

    // Reading
    App::Logic::VcuOutputs outputs{};
    App::Logic::VcuInputs inputs{};

    uint32_t currentTickMs = HAL_GetTick();
    inputs.tickMs = currentTickMs;

    bool rawBrake = (HAL_GPIO_ReadPin(SW_BRAKE_GPIO_Port, SW_BRAKE_Pin) == GPIO_PIN_RESET);
    bool rawBoard = (HAL_GPIO_ReadPin(BTN_BOARD_GPIO_Port, BTN_BOARD_Pin) == GPIO_PIN_SET);
    bool rawEStop = (HAL_GPIO_ReadPin(SW_PUSH_EMERGENCY_GPIO_Port, SW_PUSH_EMERGENCY_Pin) ==
                     GPIO_PIN_RESET);
    bool rawForward = (HAL_GPIO_ReadPin(SW_DIRECTION_GPIO_Port, SW_DIRECTION_Pin) ==
                       GPIO_PIN_RESET);
    bool rawStart = (HAL_GPIO_ReadPin(SW_START_GPIO_Port, SW_START_Pin) == GPIO_PIN_RESET);

    brakeButton.update(currentTickMs, rawBrake);
    boardButton.update(currentTickMs, rawBoard);
    eStopButton.update(currentTickMs, rawEStop);
    forwardButton.update(currentTickMs, rawForward);
    startButton.update(currentTickMs, rawStart);

    inputs.pilot.isBrakePressed = brakeButton.isPressed();
    inputs.pilot.isBtnBoardPressed = boardButton.isPressed();
    inputs.pilot.isEStopPressed = eStopButton.isPressed();
    inputs.pilot.isForwardDirection = forwardButton.isPressed();
    inputs.pilot.isStartPressed = startButton.isPressed();
    inputs.pilot.isStartRisingEdge = startButton.isRisingEdge();

    throttleSensor.update();
    inputs.pilot.throttlePercent = throttleSensor.getPercent();

    // Business logic
    vcuController.run(inputs, outputs);

    // Writing
    HAL_GPIO_WritePin(LED_BOARD_GPIO_Port, LED_BOARD_Pin,
                      outputs.lights.boardLed ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOSFET_CONTACTOR_GATE_GPIO_Port, MOSFET_CONTACTOR_GATE_Pin,
                      outputs.power.contactor ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOSFET_PRECHARGE_GATE_GPIO_Port, MOSFET_PRECHARGE_GATE_Pin,
                      outputs.power.prechargeRelay ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_SW_START_GPIO_Port, LED_SW_START_Pin,
                      outputs.lights.startLed ? GPIO_PIN_SET : GPIO_PIN_RESET);

    if (outputs.vesc.isMotorEnabled) {
        App::Drivers::vescDriver.sendAmpCommand(outputs.vesc.targetAmp);
    } else {
        App::Drivers::vescDriver.sendAmpCommand(0.0f);
    }*/
}