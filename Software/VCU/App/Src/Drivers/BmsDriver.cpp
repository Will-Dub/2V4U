#include "BmsDriver.hpp"

#include <string.h>

namespace App::Drivers {

BmsDriver::BmsDriver(UART_HandleTypeDef& huart) : m_huart(huart) {}

void BmsDriver::init()
{
    startReceive();
}

// Reception

void BmsDriver::startReceive()
{
    HAL_UARTEx_ReceiveToIdle_DMA(&m_huart, m_bmsRxBuf, BUFFER_SIZE);

    // No callback at half buffer, only when the line goes idle
    __HAL_DMA_DISABLE_IT(m_huart.hdmarx, DMA_IT_HT);
}

// Interrupt: copy the frame out, then listen again
void BmsDriver::onRxEvent(uint16_t size)
{
    if (size > BUFFER_SIZE)
        size = BUFFER_SIZE;

    memcpy(m_frame, m_bmsRxBuf, size);
    m_frameSize = size;
    m_frameReady = true;

    startReceive();
}

// Interrupt: only flag it, the recovery happens in process()
void BmsDriver::onUartError()
{
    m_uartErrors = m_uartErrors + 1;
    m_restartPending = true;
}

// Main loop

void BmsDriver::process(uint32_t nowMs)
{
    // Recover after a UART error
    if (m_restartPending) {
        m_restartPending = false;
        HAL_UART_AbortReceive(&m_huart);
        startReceive();
    }

    // Take the frame with interrupts masked for a moment
    if (m_frameReady) {
        uint8_t frame[BUFFER_SIZE];
        uint16_t size;

        __disable_irq();
        size = m_frameSize;
        memcpy(frame, m_frame, size);
        m_frameReady = false;
        __enable_irq();

        handleFrame(frame, size, nowMs);
    }

    // Ask for the next message
    if (nowMs - m_lastRequestMs >= POLL_PERIOD_MS) {
        m_lastRequestMs = nowMs;
        sendRequest();
    }

    // Health
    m_data.isOk = m_data.isValid && (nowMs - m_data.lastUpdateMs) <= STALE_TIMEOUT_MS;
    m_data.rxErrorCount = m_frameErrors + m_uartErrors;
}

void BmsDriver::sendRequest()
{
    const uint8_t* req = m_requestCells ? REQ_CELLS : REQ_BASIC;
    m_requestCells = !m_requestCells;

    // Skipped if the previous request is still going out
    HAL_UART_Transmit_IT(&m_huart, const_cast<uint8_t*>(req), sizeof(REQ_BASIC));
}

// Parsing

uint16_t BmsDriver::be16(const uint8_t* p)
{
    return static_cast<uint16_t>((p[0] << 8) | p[1]);
}

// Frame: DD | command | status | length | data | checksum (2) | 77
bool BmsDriver::checkFrame(const uint8_t* f, uint16_t size) const
{
    if (size < HEADER_LEN + TRAILER_LEN)
        return false;
    if (f[0] != START_BYTE)
        return false;

    // Status 0 means the BMS accepted the request
    if (f[2] != 0x00)
        return false;

    const uint8_t len = f[3];
    if (size < static_cast<uint16_t>(len + HEADER_LEN + TRAILER_LEN))
        return false;
    if (f[len + HEADER_LEN + 2] != END_BYTE)
        return false;

    // Checksum is 0x10000 minus the sum of status, length and data
    uint16_t sum = 0;
    for (uint16_t i = 2; i < static_cast<uint16_t>(len + HEADER_LEN); i++) {
        sum += f[i];
    }
    const uint16_t expected = static_cast<uint16_t>(0x10000u - sum);

    return be16(&f[len + HEADER_LEN]) == expected;
}

void BmsDriver::handleFrame(const uint8_t* data, uint16_t size, uint32_t nowMs)
{
    if (!checkFrame(data, size)) {
        m_frameErrors++;
        return;
    }

    bool ok = false;
    switch (data[1]) {
        case CMD_BASIC:
            ok = parseBasic(data);
            if (ok) {
                m_data.lastUpdateMs = nowMs;
                m_data.isValid = true;
            }
            break;
        case CMD_CELLS:
            ok = parseCells(data);
            break;
        default:
            break;
    }

    if (!ok)
        m_frameErrors++;
}

// Pack voltage, current, soc, protection flags and temperatures
bool BmsDriver::parseBasic(const uint8_t* f)
{
    const uint8_t len = f[3];
    const uint8_t* d = &f[HEADER_LEN];

    if (len < BASIC_MIN_LEN)
        return false;

    // Conversion to volts and amps (10 mV and 10 mA per count)
    const float voltage = be16(&d[0]) / 100.0f;
    const float current = static_cast<int16_t>(be16(&d[2])) / 100.0f;
    const uint16_t cycles = be16(&d[8]);
    const uint16_t protection = be16(&d[16]);
    const uint8_t soc = d[19];
    const uint8_t cells = d[21];
    const uint8_t ntcs = d[22];

    // A corrupted frame can pass the checksum, so reject impossible values
    if (voltage > MAX_PACK_VOLTAGE || soc > 100)
        return false;

    // Temperatures are in 0.1 K
    uint8_t usable = ntcs;
    if (usable > (len - BASIC_MIN_LEN) / 2)
        usable = (len - BASIC_MIN_LEN) / 2;
    if (usable > App::Logic::BmsData::MAX_TEMPS)
        usable = App::Logic::BmsData::MAX_TEMPS;

    float temps[App::Logic::BmsData::MAX_TEMPS]{};
    for (uint8_t i = 0; i < usable; i++) {
        temps[i] = (be16(&d[BASIC_MIN_LEN + i * 2]) - 2731) / 10.0f;
        if (temps[i] < MIN_TEMP_C || temps[i] > MAX_TEMP_C)
            return false;
    }

    // Only publish once everything checked out
    m_data.packVoltageV = voltage;
    m_data.packCurrentA = current;
    m_data.socPercent = soc;
    m_data.cycles = cycles;
    m_data.protectionFlags = protection;
    m_data.cellCount = cells;
    m_data.tempCount = usable;
    for (uint8_t i = 0; i < usable; i++) {
        m_data.tempC[i] = temps[i];
    }

    return true;
}

// Cell voltages, one 16 bit value per cell
bool BmsDriver::parseCells(const uint8_t* f)
{
    const uint8_t* d = &f[HEADER_LEN];
    uint8_t n = f[3] / 2;

    if (n == 0)
        return false;
    if (n > App::Logic::BmsData::MAX_CELLS)
        n = App::Logic::BmsData::MAX_CELLS;

    uint16_t mv[App::Logic::BmsData::MAX_CELLS]{};
    uint16_t lo = 0xFFFF;
    uint16_t hi = 0;

    for (uint8_t i = 0; i < n; i++) {
        mv[i] = be16(&d[i * 2]); // 1 mV per count
        if (mv[i] > MAX_CELL_MV)
            return false;
        if (mv[i] < lo)
            lo = mv[i];
        if (mv[i] > hi)
            hi = mv[i];
    }

    // Conversion to volts for the min and max
    m_data.cellMinV = lo / 1000.0f;
    m_data.cellMaxV = hi / 1000.0f;
    m_data.cellCount = n;
    for (uint8_t i = 0; i < n; i++) {
        m_data.cellMv[i] = mv[i];
    }

    return true;
}

} // namespace App::Drivers