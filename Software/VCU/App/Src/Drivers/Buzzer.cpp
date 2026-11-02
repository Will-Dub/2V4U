#include "Buzzer.hpp"

namespace App::Drivers {
Buzzer::Buzzer(GPIO_TypeDef* buzzerPort, uint16_t buzzerPin)
    : m_buzzerPort(buzzerPort), m_buzzerPin(buzzerPin)
{
}

void Buzzer::set(Logic::BuzzerMode mode, uint32_t beepMs, uint32_t pauseMs)
{
    if (m_mode == Logic::BuzzerMode::BEEP_ONCE) {
        return;
    }

    if (m_mode != mode || m_beepMs != beepMs || m_pauseMs != pauseMs) {
        m_mode = mode;
        m_beepMs = beepMs;
        m_pauseMs = pauseMs;
        m_lastBeepMs = 0;
    }
}

void Buzzer::update(uint32_t currentMs)
{
    switch (m_mode) {
        case Logic::BuzzerMode::OFF:
            m_state = false;
            break;
        case Logic::BuzzerMode::ON:
            m_state = true;
            break;
        case Logic::BuzzerMode::BEEP_ONCE:
            if (m_lastBeepMs == 0) {
                m_state = true;
                m_lastBeepMs = currentMs;
            } else if ((currentMs - m_lastBeepMs) >= m_beepMs) {
                m_state = false;
                m_mode = Logic::BuzzerMode::OFF;
            }
            break;
        case Logic::BuzzerMode::BEEP_LOOP:
            if (m_lastBeepMs == 0) {
                m_state = true;
                m_lastBeepMs = currentMs;
            }

            uint32_t elapsedTime = (currentMs - m_lastBeepMs);
            if ((m_state && elapsedTime >= m_beepMs) || (!m_state && elapsedTime >= m_pauseMs)) {
                m_state = !m_state;
                m_lastBeepMs = currentMs;
            }
            break;
    }

    HAL_GPIO_WritePin(m_buzzerPort, m_buzzerPin, m_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

} // namespace App::Drivers