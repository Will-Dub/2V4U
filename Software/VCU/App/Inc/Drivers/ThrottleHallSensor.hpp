#pragma once
#include <stdint.h>

namespace App::Drivers {

class ThrottleHallSensor
{
  public:
    ThrottleHallSensor();
    void init(volatile uint16_t* dmaPtr, uint16_t dmaSize = 100);
    float getPercent();
    bool isValid();
    void update();

  private:
    static float mapRange(uint16_t x, uint16_t in_min, uint16_t in_max, float out_min,
                          float out_max);

    volatile uint16_t* m_dmaPtr;
    uint16_t m_dmaSize;
    float m_throttleValue;
    bool m_isValid;

    const uint16_t FAULT_MIN = 300;
    const uint16_t FAULT_MAX = 3500;
    const uint16_t PEDAL_MIN = 570;
    const uint16_t PEDAL_MAX = 3180;
};

}; // namespace App::Drivers