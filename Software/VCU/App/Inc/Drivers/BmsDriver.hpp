#pragma once

#include "VcuStruct.hpp"
#include "main.h"

#include <stdint.h>

#define BMS_BUFFER_SIZE 100

namespace App::Drivers {

class BmsDriver
{
  public:
    explicit BmsDriver(UART_HandleTypeDef& huart);

    void init();
    void process(uint32_t currentTickMs); // Main loop
    void onRxEvent(uint16_t size);        // Interrupt context
    void onUartError();                   // Interrupt context

    const App::Logic::BmsData& getData() const
    {
        return m_data;
    }
    bool isStale(uint32_t nowMs, uint32_t timeoutMs) const;

    uint8_t dbgBytes[12]{};
    uint16_t dbgSize = 0;

  private:
    UART_HandleTypeDef& m_huart;
    App::Logic::BmsData m_data{};

    // Shared with the interrupt
    uint8_t m_bmsRxBuf[BMS_BUFFER_SIZE]{}; // DMA target
    uint8_t m_frame[BMS_BUFFER_SIZE]{};    // Copy for the main loop
    volatile uint16_t m_frameSize = 0;
    volatile bool m_frameReady = false;
    volatile bool m_restartPending = false;
    volatile uint32_t m_uartErrors = 0;

    // Main loop only
    uint32_t m_frameErrors = 0;
    uint32_t m_lastRequestMs = 0;
    bool m_requestCells = false; // Alternates basic info / cell voltages

    void startReceive();
    void sendRequest();
    void handleFrame(const uint8_t* data, uint16_t size, uint32_t nowMs);
    bool checkFrame(const uint8_t* f, uint16_t size) const;
    bool parseBasic(const uint8_t* f);
    bool parseCells(const uint8_t* f);
};

} // namespace App::Drivers