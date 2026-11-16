#pragma once

#include "VcuStruct.hpp"
#include "main.h"

#include <stdint.h>

namespace App::Drivers {

class BmsDriver
{
  public:
    explicit BmsDriver(UART_HandleTypeDef& huart);

    void init();
    void process(uint32_t nowMs);  // Main loop
    void onRxEvent(uint16_t size); // Interrupt context
    void onUartError();            // Interrupt context

    const App::Logic::BmsData& getData() const
    {
        return m_data;
    }

  private:
    // Buffers
    static constexpr uint16_t BUFFER_SIZE = 100;

    // JBD protocol
    static constexpr uint8_t START_BYTE = 0xDD;
    static constexpr uint8_t END_BYTE = 0x77;
    static constexpr uint8_t CMD_BASIC = 0x03;
    static constexpr uint8_t CMD_CELLS = 0x04;
    static constexpr uint8_t HEADER_LEN = 4;  // Start, command, status, length
    static constexpr uint8_t TRAILER_LEN = 3; // Checksum (2) and end byte
    static constexpr uint8_t BASIC_MIN_LEN = 23;

    static constexpr uint8_t REQ_BASIC[7] = {0xDD, 0xA5, 0x03, 0x00, 0xFF, 0xFD, 0x77};
    static constexpr uint8_t REQ_CELLS[7] = {0xDD, 0xA5, 0x04, 0x00, 0xFF, 0xFC, 0x77};

    // Timing
    static constexpr uint32_t POLL_PERIOD_MS = 250;    // Alternates basic and cells
    static constexpr uint32_t STALE_TIMEOUT_MS = 2000; // No good basic frame means lost

    // Sanity limits
    static constexpr float MAX_PACK_VOLTAGE = 80.0f;
    static constexpr float MIN_TEMP_C = -50.0f;
    static constexpr float MAX_TEMP_C = 150.0f;
    static constexpr uint16_t MAX_CELL_MV = 5000;

    UART_HandleTypeDef& m_huart;
    App::Logic::BmsData m_data{};

    // Shared with the interrupt
    uint8_t m_bmsRxBuf[BUFFER_SIZE]{}; // DMA target
    uint8_t m_frame[BUFFER_SIZE]{};    // Copy for the main loop
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

    // Big-endian 16 bit value
    static uint16_t be16(const uint8_t* p);
};

} // namespace App::Drivers