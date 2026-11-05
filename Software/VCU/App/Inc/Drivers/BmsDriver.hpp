#pragma once

#include "VcuStruct.hpp"
#include "main.h"

#include <stdint.h>

#define BMS_BUFFER_SIZE 100

namespace App::Drivers {

class BmsDriver
{
  public:
    BmsDriver(UART_HandleTypeDef& huart);

    void init();
    void process(uint32_t currentTickMs);
    void onRxEvent(uint16_t size);
    void onUartError();

  private:
    UART_HandleTypeDef& m_huart;
    uint8_t m_bmsRxBuf[BMS_BUFFER_SIZE];

    void startReceive();
};

}; // namespace App::Drivers