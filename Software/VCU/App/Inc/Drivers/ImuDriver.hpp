#pragma once

#include "main.h"

#include <stdint.h>

#define SPI_READ_BIT 0x80
#define SPI_TIMEOUT_MS 5
#define REG_WHO_AM_I 0x00
#define REG_PWR_MGMT_1 0x06
#define REG_INT_ENABLE 0x11
#define REG_ACCEL_XOUT_H 0x2D

namespace App::Drivers {

class ImuDriver
{
  public:
    ImuDriver(SPI_HandleTypeDef& hspi, GPIO_TypeDef* csPort, uint16_t csPin);
    bool init();
    bool readSensorData(float& ax, float& ay, float& az, float& gx, float& gy, float& gz,
                        float& tempC);

  private:
    SPI_HandleTypeDef& m_hspi;
    GPIO_TypeDef* m_csPort;
    uint16_t m_csPin;
    bool m_isInit = false;

    bool checkWhoAmiI();
    void writeRegister(uint8_t reg, uint8_t value);
    bool readRegisters(uint8_t reg, uint8_t* rxBuffer, uint16_t size);
    void select();
    void deselect();
};

}; // namespace App::Drivers