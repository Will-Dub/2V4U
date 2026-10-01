#include "App.h"

#include "ThrottleHallSensor.hpp"
#include "VcuController.hpp"
#include "VcuStateInit.hpp"
#include "VcuStruct.hpp"
#include "VescCanDriver.hpp"
#include "main.h"

#include <math.h>
#include <stdio.h>

static App::Logic::VcuController vcuController;
static App::Drivers::ThrottleHallSensor throttleSensor;
volatile bool tick = false;

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
}

extern "C" void App_Run(void)
{
    if (tick) {
        tick = false;
        // Reading
        App::Logic::VcuOutputs outputs{};
        App::Logic::VcuInputs inputs{};

        inputs.pilot.isBtnBoardPressed = HAL_GPIO_ReadPin(BTN_BOARD_GPIO_Port, BTN_BOARD_Pin);
        inputs.pilot.isStartPressed = HAL_GPIO_ReadPin(SW_START_GPIO_Port, SW_START_Pin);
        inputs.pilot.isEStopPressed = HAL_GPIO_ReadPin(SW_PUSH_EMERGENCY_GPIO_Port,
                                                       SW_PUSH_EMERGENCY_Pin);
        inputs.pilot.isBrakePressed = HAL_GPIO_ReadPin(SW_BRAKE_GPIO_Port, SW_BRAKE_Pin);
        inputs.pilot.isForwardDirection = HAL_GPIO_ReadPin(SW_DIRECTION_GPIO_Port,
                                                           SW_DIRECTION_Pin);

        throttleSensor.update();
        inputs.pilot.throttlePercent = throttleSensor.getPercent();

        int whole = (int)inputs.pilot.throttlePercent;
        int fraction = (int)(abs(inputs.pilot.throttlePercent) * 100) % 100;

        printf("Throttle2: %d.%02d\r\n", whole, fraction);

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
    }
}