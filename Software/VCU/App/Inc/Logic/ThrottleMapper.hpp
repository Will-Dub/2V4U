#pragma once

#define THROTTLE_RELEASE_THRESHOLD 0.05f

namespace App::Logic {

struct ThrottleMapConfig
{
    // Ratio ignored at the bottom
    float deadzone = 0.04f;

    float curveExponent = 1.5f;
    float maxAmps = 10.0f;

    // Ratio of maxAmps per sec
    float riseRatePerS = 1.5f;
    float fallRatePerS = 4.0f
};

class ThrottleMapper
{
  public:
    explicit ThrottleMapper(const ThrottleMapConfig& config = {}) : m_config(config) {}

    float update(float throttle, float dtS);

    void reset() { m_out = 0.0f; }

    float getOutput() const { return m_out; }

  private:
    ThrottleMapConfig m_config;
    float m_out = 0.0f;
};
} // namespace App::Logic