#pragma once

#include <stdint.h>

#define DEBOUNCE_TIME_MS 20

namespace App::Drivers {

class DebounceButton
{
  public:
    DebounceButton();
    void update(uint32_t currentMs, bool isPressed);
    bool isPressed() const;
    bool isRisingEdge() const;

  private:
    bool m_state = false;
    bool m_lastRawState = false;
    uint32_t m_lastChangeTimeMs = 0;
    bool m_isRisingEdge = false;
};

}; // namespace App::Drivers