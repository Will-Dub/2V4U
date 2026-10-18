#include "App.h"

#include "DashTransmitter.hpp"
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

static App::Infrastructure::DashTransmitter dashTransmitter;
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

static void gatherInputs(App::Logic::VcuInputs& inputs)
{

    uint32_t currentTickMs = HAL_GetTick();
    inputs.tickMs = currentTickMs;

    // Buttons/Switch
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

    // Throttle
    throttleSensor.update();
    inputs.pilot.throttlePercent = throttleSensor.getPercent();
    inputs.pilot.isThrottleValid = throttleSensor.getIsValid();

    // IMU
    static float imuAx = 0.0f, imuAy = 0.0f, imuAz = 1.0f;
    static float imuGx = 0.0f, imuGy = 0.0f, imuGz = 0.0f;
    static float imuTemp = 0.0f;
    static bool imuIsDataValid = false;

    if (imuDataReady) {
        imuDataReady = false;

        imuIsDataValid = imuDriver.readSensorData(imuAx, imuAy, imuAz, imuGx, imuGy, imuGz,
                                                  imuTemp);
    }

    inputs.imu.isValid = imuIsDataValid;

    if (imuIsDataValid) {
        inputs.imu.Ax = imuAx;
        inputs.imu.Ay = imuAy;
        inputs.imu.Az = imuAz;
        inputs.imu.Gx = imuGx;
        inputs.imu.Gy = imuGy;
        inputs.imu.Gz = imuGz;
        inputs.imu.temp = imuTemp;
    }

    // Log
    /*

    if (currentTickMs - lastLogMs >= 1000) {
        lastLogMs = currentTickMs;

        if (imuIsDataValid) {
            stmLogger.debugf("PLT | Brk:%d Btn:%d Est:%d Fwd:%d Srt:%d(R:%d) Thr:%5.1f%% (V:%d)",
                             inputs.pilot.isBrakePressed, inputs.pilot.isBtnBoardPressed,
                             inputs.pilot.isEStopPressed, inputs.pilot.isForwardDirection,
                             inputs.pilot.isStartPressed, inputs.pilot.isStartRisingEdge,
                             inputs.pilot.throttlePercent, inputs.pilot.isThrottleValid);

            stmLogger.debugf("IMU | Val:%d | A:%5.2f %5.2f %5.2f | G:%6.1f %6.1f %6.1f | T:%.1fC",
                             inputs.imu.isValid, inputs.imu.Ax, inputs.imu.Ay, inputs.imu.Az,
                             inputs.imu.Gx, inputs.imu.Gy, inputs.imu.Gz, inputs.imu.temp);

            stmLogger.debugf("VSC | %6ld RPM | Mot:%5.1fA In:%5.1fA %4.1fV | TMot:%.1fC TMos:%.1fC",
                             inputs.vesc.rpm, inputs.vesc.motorCurrent, inputs.vesc.inputCurrent,
                             inputs.vesc.inputVoltage, inputs.vesc.motorTemp,
                             inputs.vesc.mosfetTemp);
        } else {
            stmLogger.errorf("INVALID IMU DATA - SENSOR LOST");
        }
    }*/
}

static void applyOutputs(App::Logic::VcuOutputs& outputs)
{
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

    App::Logic::VcuOutputs outputs{};
    App::Logic::VcuInputs inputs{};

    gatherInputs(inputs);

    vcuController.run(inputs, outputs);

    applyOutputs(outputs);

    dashTransmitter.process(vcuController.getCurrentVcuStateId(), inputs, outputs);
}