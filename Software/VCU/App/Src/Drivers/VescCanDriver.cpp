#include "VescCanDriver.hpp"

#include <string.h>

namespace App::Drivers {

VescCanDriver::VescCanDriver(FDCAN_HandleTypeDef& hfdcan) : m_hfdcan(hfdcan)
{
    m_data = {0};
}

void VescCanDriver::init()
{
    HAL_FDCAN_ConfigGlobalFilter(&m_hfdcan, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_ACCEPT_IN_RX_FIFO0,
                                 FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);

    if (HAL_FDCAN_Start(&m_hfdcan) != HAL_OK)
        Error_Handler();

    if (HAL_FDCAN_ActivateNotification(&m_hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
        Error_Handler();
}

App::Logic::VescData VescCanDriver::getData() const
{
    return m_data;
}

void VescCanDriver::sendAmpCommand(float amp)
{
    uint8_t txData[8]{0};
    vesc_vesc_command_abs_current_v1_t src;

    // Milli amp so *1000
    src.command_current_v1 = static_cast<int32_t>(amp * 1000.0f);
    vesc_vesc_command_abs_current_v1_pack(txData, &src, 8);

    FDCAN_TxHeaderTypeDef txHeader{};

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

    if (HAL_FDCAN_AddMessageToTxFifoQ(&m_hfdcan, &txHeader, txData) != HAL_OK) {
        m_data.txFailCount++;
        m_data.followedTxFailCount++;
    } else {
        m_data.followedTxFailCount = 0;
        m_data.txCount++;
    }

    // TODO: Add regenerative braking (vesc_vesc_command_abs_brake_current_v1_pack)
}

void VescCanDriver::processSlots()
{
    for (uint8_t i = 0; i < SLOT_COUNT; i++) {
        const Slot slot = static_cast<Slot>(i);
        uint8_t data[8];
        uint32_t lastRxMs = 0;

        if (!takeSlot(slot, data, lastRxMs))
            continue;

        m_data.rxCount++;
        m_data.lastRxMs = lastRxMs;

        switch (slot) {
            case SLOT_STATUS1:
                decodeStatus1(data);
                break;
            case SLOT_STATUS4:
                decodeStatus4(data);
                break;
            case SLOT_STATUS5:
                decodeStatus5(data);
                break;
            default:
                break;
        }
    }
}

void VescCanDriver::onRxFifo0()
{
    FDCAN_RxHeaderTypeDef rxHeader;
    uint8_t rxData[8];

    while (HAL_FDCAN_GetRxFifoFillLevel(&m_hfdcan, FDCAN_RX_FIFO0) > 0) {
        // Get can message
        if (HAL_FDCAN_GetRxMessage(&m_hfdcan, FDCAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK)
            break;

        // Check the format
        if (rxHeader.IdType != FDCAN_EXTENDED_ID)
            continue;
        if (rxHeader.DataLength != FDCAN_DLC_BYTES_8)
            continue;

        // Check the id and command
        const uint8_t vescId = rxHeader.Identifier & 0xFF;
        const uint8_t commandId = (rxHeader.Identifier >> 8) & 0xFF;
        if (vescId != kVescId)
            continue;

        // Find the slot
        Slot slot = SLOT_STATUS1;
        if (!slotForCommand(commandId, slot))
            continue;

        // Store the data in the slot
        memcpy(m_slots[slot].data, rxData, sizeof(rxData));
        m_slotNew[slot] = true;
        m_lastRxMs = HAL_GetTick();
    }
}

bool VescCanDriver::takeSlot(Slot slot, uint8_t (&data)[8], uint32_t& lastRxMs)
{
    // Disable interupt so no overwrite
    const uint32_t primask = __get_PRIMASK();
    __disable_irq();

    const bool isNew = m_slotNew[slot];
    if (isNew) {
        memcpy(data, m_slots[slot].data, sizeof(data));
        m_slotNew[slot] = false;
        lastRxMs = m_lastRxMs;
    }

    __set_PRIMASK(primask);
    return isNew;
}

void VescCanDriver::decodeStatus1(const uint8_t* data)
{
    vesc_vesc_status1_v1_t st;
    vesc_vesc_status1_v1_unpack(&st, data, 8);
    m_data.rpm = st.status_rpm_v1;
    m_data.motorCurrent = st.status_total_current_v1 / 10.0f;
}

void VescCanDriver::decodeStatus4(const uint8_t* data)
{
    vesc_vesc_status4_v1_t st;
    vesc_vesc_status4_v1_unpack(&st, data, 8);
    m_data.mosfetTemp = st.staus_mosfet_temp_v1 / 10.0f;
    m_data.motorTemp = st.staus_motor_temp_v1 / 10.0f;
    m_data.inputCurrent = st.status_total_input_current_v1 / 10.0f;
}

void VescCanDriver::decodeStatus5(const uint8_t* data)
{
    vesc_vesc_status5_v1_t st;
    vesc_vesc_status5_v1_unpack(&st, data, 8);
    m_data.inputVoltage = st.status_input_voltage_v1 / 10.0f;
}

bool VescCanDriver::slotForCommand(uint8_t commandId, Slot& slot)
{
    switch (commandId) {
        case CMD_STATUS_1:
            slot = SLOT_STATUS1;
            return true;
        case CMD_STATUS_4:
            slot = SLOT_STATUS4;
            return true;
        case CMD_STATUS_5:
            slot = SLOT_STATUS5;
            return true;
        default:
            return false;
    }
}

} // namespace App::Drivers