#include "App.h"
#include "Drivers/VescCanDriver.hpp"
#include "Logic/VcuStateInit.hpp"
#include "VcuController.hpp"
#include "VcuStruct.hpp"
#include "main.h"

static App::Logic::VcuController vcuController;

extern "C" void App_Init(void) { App::Drivers::vescDriver.init(); }

extern "C" void App_Run(void) {
  App::Logic::VcuOutputs outputs{};
  App::Logic::VcuInputs inputs{};

  inputs.pilot.boardButtonPressed =
      HAL_GPIO_ReadPin(BTN_BOARD_GPIO_Port, BTN_BOARD_Pin);

  vcuController.run(inputs, outputs);

  HAL_GPIO_WritePin(LED_BOARD_GPIO_Port, LED_BOARD_Pin,
                    outputs.lights.boardLed ? GPIO_PIN_SET : GPIO_PIN_RESET);
}