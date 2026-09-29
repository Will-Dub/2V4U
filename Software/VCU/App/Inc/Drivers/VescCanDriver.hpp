#pragma once
#include "fdcan.h"

namespace App::Drivers {
struct CanFrame
{
    uint32_t identifier;
    uint8_t data[8];
};

struct VescData
{
    int32_t erpm;
    float motorCurrent;
    float inputCurrent;
    float dutyCycle;
    float inputVoltage;
    float mosfetTemp;
    float motorTemp;
    uint8_t faultCode;

    float ampHours;
    float ampHoursCharged;
    float wattHours;
    float wattHoursCharged;
};

class VescCanDriver
{
  public:
    VescCanDriver();

    void init();
    void sendAmpCommand(float amp);

    VescData getData() const;

    void pushToRb(uint32_t id, uint8_t* data);
    void processRb();

  private:
    VescData m_data;

    static const int RING_BUFFER_SIZE = 16;
    CanFrame rxRingBuffer[RING_BUFFER_SIZE];

    volatile int rbHead;
    int rbTail;
};

extern VescCanDriver vescDriver;
} // namespace App::Drivers