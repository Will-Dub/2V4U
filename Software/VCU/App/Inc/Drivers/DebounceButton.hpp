#pragma once

#include <stdint.h>

namespace App::Drivers {

class DebounceButton
{
  public:
    DebounceButton(bool isNormallyClosed = false);
    void update(uint32_t currentMs, bool isPressed);
    bool isPressed() const;
    bool isRisingEdge() const;

  private:
    static constexpr int DEBOUNCE_TIME_MS = 20;

    bool m_isNormallyClosed = false;
    bool m_state = false;
    bool m_lastRawState = false;
    uint32_t m_lastChangeTimeMs = 0;
    bool m_isRisingEdge = false;
};

}; // namespace App::Drivers