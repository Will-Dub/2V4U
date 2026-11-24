#include "ThrottleHallSensor.hpp"

#include <stdio.h>

namespace App::Drivers {

ThrottleHallSensor::ThrottleHallSensor() {}

void ThrottleHallSensor::init(volatile uint16_t* dmaPtr, uint16_t dmaSize)
{
    m_dmaPtr = dmaPtr;
    m_dmaSize = dmaSize;
}

void ThrottleHallSensor::update()
{
    if (m_dmaPtr == nullptr || m_dmaSize == 0) {
        m_badSamples = COUNT_BAD_READ_TO_FAULT;
        m_isReadingOk = false;
        return;
    }

    uint32_t sum = 0;
    for (uint16_t i = 0; i < m_dmaSize; i++) {
        sum += m_dmaPtr[i];
    }
    uint16_t average = sum / m_dmaSize;

    m_isReadingOk = (average >= FAULT_MIN && average <= FAULT_MAX);

    if (m_isReadingOk) {
        m_badSamples = 0;
        m_value = average;
    } else if (m_badSamples < COUNT_BAD_READ_TO_FAULT) {
        m_badSamples++;
    }
}

float ThrottleHallSensor::getRatio() const
{
    if (!m_isReadingOk || !getIsValid())
        return 0.0f;

    float ratio = (m_value - PEDAL_MIN) / (float)(PEDAL_MAX - PEDAL_MIN);

    if (ratio < 0.0f)
        ratio = 0.0f;
    if (ratio > 1.0f)
        ratio = 1.0f;

    return ratio;
}

bool ThrottleHallSensor::getIsValid() const
{
    return m_badSamples < COUNT_BAD_READ_TO_FAULT;
}

} // namespace App::Drivers