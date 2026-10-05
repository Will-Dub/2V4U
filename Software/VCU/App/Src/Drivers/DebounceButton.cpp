#include "DebounceButton.hpp"

DebounceButton::DebounceButton() {}

void DebounceButton::update(uint32_t currentMs, bool isPressed) {
    if (isPressed != m_lastState) {
        m_lastChangeTimeMs = currentMs;
    }
    m_lastState = isPressed;
}

bool DebounceButton::isPressed() const {
    return m_lastState;
}