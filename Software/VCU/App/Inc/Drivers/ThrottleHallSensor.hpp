#pragma once
#include <stdint.h>

namespace App::Drivers {

class ThrottleHallSensor
{
  public:
    ThrottleHallSensor();
    void init(volatile uint16_t* dmaPtr, uint16_t dmaSize = 100);
    float getRatio() const;
    bool getIsValid() const;
    void update();

  private:
    volatile uint16_t* m_dmaPtr = nullptr;
    uint16_t m_dmaSize;
    float m_value = 0;
    bool m_isReadingOk = false;
    uint8_t m_badSamples = 0;

    static constexpr uint16_t FAULT_MIN = 700;
    static constexpr uint16_t FAULT_MAX = 4500;
    static constexpr uint16_t PEDAL_MIN = 960;
    static constexpr uint16_t PEDAL_MAX = 4100;
    static constexpr uint8_t COUNT_BAD_READ_TO_FAULT = 10;
};

}; // namespace App::Drivers