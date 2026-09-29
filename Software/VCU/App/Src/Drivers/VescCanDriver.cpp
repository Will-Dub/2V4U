#include "VescCanDriver.hpp"

extern FDCAN_HandleTypeDef hfdcan1;

namespace App::Drivers {

VescCanDriver vescDriver;

VescCanDriver::VescCanDriver()
{
    m_data = {0};
}

void VescCanDriver::init()
{
    HAL_FDCAN_Start(&hfdcan1);

    HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}

VescData VescCanDriver::getData() const
{
    return m_data;
}

void VescCanDriver::sendAmpCommand(float amp)
{
    // TODO: Implement
}

void VescCanDriver::processRb()
{
    // TODO: Implement
}

void VescCanDriver::pushToRb(uint32_t id, uint8_t* data)
{
    // Copy data
    rxRingBuffer[rbHead].identifier = id;
    for (int i = 0; i < 8; i++) {
        rxRingBuffer[rbHead].data[i] = data[i];
    }

    // Add to ring buffer
    rbHead = (rbHead + 1) % RING_BUFFER_SIZE;
}

} // namespace App::Drivers

extern "C" {
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs)
{
    if (hfdcan->Instance == FDCAN1) {
        FDCAN_RxHeaderTypeDef rxHeader;
        uint8_t rxData[8];

        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, rxData) == HAL_OK) {
            App::Drivers::vescDriver.pushToRb(rxHeader.Identifier, rxData);
        }
    }
}
}