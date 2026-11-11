#pragma once
#include "VcuStruct.hpp"
#include "fdcan.h"
#include "vesc.h"

namespace App::Drivers {
struct CanFrame
{
    uint32_t identifier;
    uint8_t data[8];
};

class VescCanDriver
{
  public:
    VescCanDriver(FDCAN_HandleTypeDef& hfdcan);

    void init();
    void sendAmpCommand(float amp);

    App::Logic::VescData getData() const;

    void processSlots();
    void onRxFifo0();

  private:
    static constexpr uint8_t kVescId = 1;
    static constexpr float kMaxCommandAmp = 100.0f;

    enum CommandId : uint8_t
    {
        CMD_SET_CURRENT = 1,
        CMD_STATUS_1 = 9,
        CMD_STATUS_4 = 16,
        CMD_STATUS_5 = 27
    };

    enum Slot : uint8_t
    {
        SLOT_STATUS1,
        SLOT_STATUS4,
        SLOT_STATUS5,
        SLOT_COUNT
    };

    struct RxSlot
    {
        uint8_t data[8];
    };

    FDCAN_HandleTypeDef& m_hfdcan;
    App::Logic::VescData m_data{};

    CanFrame m_slots[SLOT_COUNT]{};
    volatile bool m_slotNew[SLOT_COUNT]{};
    volatile uint32_t m_lastRxMs;

    static bool slotForCommand(uint8_t commandId, Slot& slot);
    bool takeSlot(Slot slot, uint8_t (&data)[8], uint32_t& lastRxMs);

    void decodeStatus1(const uint8_t* data);
    void decodeStatus4(const uint8_t* data);
    void decodeStatus5(const uint8_t* data);
};

} // namespace App::Drivers