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
    VescCanDriver();

    void init();
    void sendAmpCommand(float amp);

    App::Logic::VescData getData() const;

    void pushToRb(uint32_t id, uint8_t* data);
    void processRb();

  private:
    App::Logic::VescData m_data;

    static const int RING_BUFFER_SIZE = 16;
    CanFrame m_rxRingBuffer[RING_BUFFER_SIZE];
    uint32_t m_lastRxMs;

    volatile int m_rbHead = 0;
    int m_rbTail = 0;
};

extern VescCanDriver vescDriver;
} // namespace App::Drivers