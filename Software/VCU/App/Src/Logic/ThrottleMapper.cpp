#include "ThrottleMapper.hpp"

#include <cmath>

namespace App::Logic {

float ThrottleMapper::update(float throttle, float dt)
{
    // Check throttle is valid and between 0-1
    if (std::isnan(throttle)) throttle = 0.0f;
    if (throttle < 0.0f) throttle = 0.0f;
    if (throttle > 1.0f) throttle = 1.0f;
    if (dt < 0.0f) dt = 0.0f;

    // Deadzone
    float x = 0.0f;
    if (throttle > m_config.deadzone) {
        x = (throttle - m_config.deadzone) / (1.0f - m_config.deadzone);
    }

    x = std::pow(x, m_config.curveExponent);

    // Put in amps
    const float target = x * m_config.maxAmps;

    // Calculate max change
    const float maxRise = m_config.riseRatePerS * m_config.maxAmps * dt;
    const float maxFall = m_config.fallRatePerS * m_config.maxAmps * dt;

    // Apply max change
    if (target > m_out + maxRise)      m_out += maxRise;
    else if (target < m_out - maxFall) m_out -= maxFall;
    else                               m_out = target;

    return m_out;
}

} // namespace App::Logic