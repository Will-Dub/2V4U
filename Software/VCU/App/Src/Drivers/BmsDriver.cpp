#include "BmsDriver.hpp"

namespace App::Drivers {

    BmsDriver::BmsDriver(UART_HandleTypeDef& huart): m_huart(huart) {}

    void BmsDriver::init() {}

    void BmsDriver::process(uint32_t currentTickMs) {}
    void BmsDriver::onRxEvent(uint16_t size) {}
    void BmsDriver::onUartError() {}

    void BmsDriver::startReceive() {
        HAL_UARTEx_ReceiveToIdle_DMA(&m_huart, m_bmsRxBuf, BMS_BUFFER_SIZE);
        __HAL_DMA_DISABLE_IT(m_huart.hdmarx, DMA_IT_HT);
    }

} // namespace App::Drivers