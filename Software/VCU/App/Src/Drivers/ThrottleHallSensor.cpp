#include "ThrottleHallSensor.hpp"

#include "stdio.h"
namespace App::Drivers {

ThrottleHallSensor::ThrottleHallSensor() : m_dmaPtr(nullptr), m_throttleValue(0) {}

void ThrottleHallSensor::init(volatile uint16_t* dmaPtr, uint16_t dmaSize)
{
    m_dmaPtr = dmaPtr;
    m_dmaSize = dmaSize;
}

void ThrottleHallSensor::update()
{
    if (m_dmaPtr == nullptr){
        m_throttleValue = 0.0f;
        m_isValid = false;
        return;
    }

    uint32_t sum = 0;
    for (uint16_t i = 0; i < m_dmaSize; i++) {
        sum += m_dmaPtr[i];
    }
    uint16_t average = sum / m_dmaSize;

    if (average < FAULT_MIN || average > FAULT_MAX) {
        m_throttleValue = 0.0f;
        m_isValid = false;
        return;
    }

    m_throttleValue = average;
    m_isValid = true;
}

float ThrottleHallSensor::mapRange(uint16_t x, uint16_t in_min, uint16_t in_max, float out_min,
                                   float out_max)
{
    return (x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

float ThrottleHallSensor::getPercent()
{
    if(m_isValid == false){
        return 0.0f;
    }

    float percent = (m_throttleValue - PEDAL_MIN) / (float)(PEDAL_MAX - PEDAL_MIN);

    if (percent < 0.0f)
        percent = 0.0f;
    if (percent > 1.0f)
        percent = 1.0f;

    return percent;
}

} // namespace App::Drivers