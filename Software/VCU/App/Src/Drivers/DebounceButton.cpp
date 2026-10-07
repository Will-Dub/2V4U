#include "DebounceButton.hpp"

namespace App::Drivers {

DebounceButton::DebounceButton() {}

void DebounceButton::update(uint32_t currentMs, bool isPressed)
{
    m_isRisingEdge = false;

    if (isPressed != m_lastRawState) {
        m_lastChangeTimeMs = currentMs;
    }

    if ((currentMs - m_lastChangeTimeMs) > DEBOUNCE_TIME_MS) {
        if (m_lastRawState != m_state) {
            m_state = m_lastRawState;

            if (m_state == true)
                m_isRisingEdge = true;
        }
    }
    m_lastRawState = isPressed;
}

bool DebounceButton::isPressed() const
{
    return m_state;
}

bool DebounceButton::isRisingEdge() const
{
    return m_isRisingEdge;
}

} // namespace App::Drivers