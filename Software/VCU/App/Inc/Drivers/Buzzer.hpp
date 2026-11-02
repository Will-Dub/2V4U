#pragma once

#include "VcuStruct.hpp"
#include "main.h"

#include <stdint.h>

#define DEBOUNCE_TIME_MS 20

namespace App::Drivers {

class Buzzer
{
  public:
    Buzzer(GPIO_TypeDef* buzzerPort, uint16_t buzzerPin);
    void set(Logic::BuzzerMode mode, uint32_t beepMs = 0, uint32_t pauseMs = 0);
    void update(uint32_t currentMs);

  private:
    GPIO_TypeDef* m_buzzerPort;
    uint16_t m_buzzerPin;
    Logic::BuzzerMode m_mode = Logic::BuzzerMode::OFF;
    bool m_state = false;
    uint32_t m_beepMs = 0;
    uint32_t m_pauseMs = 0;
    uint32_t m_lastBeepMs = 0;
};

}; // namespace App::Drivers