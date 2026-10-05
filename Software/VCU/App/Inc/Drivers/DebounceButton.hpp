#pragma once

#include <stdint.h>

#define DEBOUNCE_TIME_MS 50

class DebounceButton
{
  public:
    DebounceButton();
    void update(uint32_t currentMs, bool isPressed);
    bool isPressed() const;

  private:
    bool m_lastState = false;
    uint32_t m_lastChangeTimeMs = 0;
};