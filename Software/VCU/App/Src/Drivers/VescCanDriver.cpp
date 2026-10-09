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

App::Logic::VescData VescCanDriver::getData() const
{
    return m_data;
}

void VescCanDriver::sendAmpCommand(float amp)
{
    uint8_t txData[8]{0};
    vesc_vesc_command_abs_current_v1_t src;

    src.command_current_v1 = static_cast<int32_t>(amp * 1000.0f);
    vesc_vesc_command_abs_current_v1_pack(txData, &src, 8);

    FDCAN_TxHeaderTypeDef txHeader;

    // Command id 0x01, vesc id 0x01
    txHeader.Identifier = (1 << 8) | 1;
    txHeader.IdType = FDCAN_EXTENDED_ID;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;
    txHeader.DataLength = FDCAN_DLC_BYTES_8;
    txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txHeader.BitRateSwitch = FDCAN_BRS_OFF;
    txHeader.FDFormat = FDCAN_CLASSIC_CAN;
    txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txHeader.MessageMarker = 0;

    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeader, txData) != HAL_OK) {
        m_data.txFailCount++;
    } else {
        m_data.txFailCount = 0;
    }

    // TODO: Add regenerative braking
    // vesc_vesc_command_abs_brake_current_v1_pack
}

void VescCanDriver::processRb()
{
    while (m_rbTail != m_rbHead) {
        // Copy to avoid race condition
        CanFrame canFrame = m_rxRingBuffer[m_rbTail];
        m_rbTail = (m_rbTail + 1) % RING_BUFFER_SIZE;

        uint8_t vescId = canFrame.identifier & 0xFF;
        uint8_t commandId = (canFrame.identifier >> 8) & 0xFF;

        if (vescId != 1) {
            continue;
        }

        m_lastRxMs = HAL_GetTick();

        switch (commandId) {
            // Status 1 (rpm, current, duty)
            case 9:
                vesc_vesc_status1_v1_t status1;
                vesc_vesc_status1_v1_unpack(&status1, canFrame.data, 8);

                m_data.rpm = status1.status_rpm_v1;
                m_data.motorCurrent = status1.status_total_current_v1 / 10.0f;

                break;

            // Status 4 (temp)
            case 16:
                vesc_vesc_status4_v1_t status4;
                vesc_vesc_status4_v1_unpack(&status4, canFrame.data, 8);

                m_data.mosfetTemp = status4.staus_mosfet_temp_v1 / 10.0f;
                m_data.motorTemp = status4.staus_motor_temp_v1 / 10.0f;
                m_data.inputCurrent = status4.status_total_input_current_v1 / 10.0f;

                break;

            // Status 5 (voltage)
            case 27:
                vesc_vesc_status5_v1_t status5;
                vesc_vesc_status5_v1_unpack(&status5, canFrame.data, 8);

                m_data.inputVoltage = status5.status_input_voltage_v1 / 10.0f;

                break;

            default:
                break;
        }
    }
}

void VescCanDriver::pushToRb(uint32_t id, uint8_t* data)
{
    // Copy data
    m_rxRingBuffer[m_rbHead].identifier = id;
    for (int i = 0; i < 8; i++) {
        m_rxRingBuffer[m_rbHead].data[i] = data[i];
    }

    // Add to ring buffer
    m_rbHead = (m_rbHead + 1) % RING_BUFFER_SIZE;
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