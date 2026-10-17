#include "ImuDriver.hpp"

using namespace App::Drivers;

ImuDriver::ImuDriver(SPI_HandleTypeDef& hspi, GPIO_TypeDef* csPort, uint16_t csPin)
    : m_hspi(hspi), m_csPort(csPort), m_csPin(csPin)
{
}

bool ImuDriver::checkWhoAmiI()
{
    uint8_t result = 0x00;

    if (!readRegisters(REG_WHO_AM_I, &result, 1)) {
        return false;
    }

    return (result == 0xEA);
}

bool ImuDriver::init()
{
    if (!checkWhoAmiI())
        return false;

    // Wake up
    writeRegister(REG_PWR_MGMT_1, 0x01);
    HAL_Delay(10);

    // Enable interupt
    writeRegister(REG_INT_ENABLE, 0x01);

    m_isInit = true;
    return true;
}

void ImuDriver::writeRegister(uint8_t reg, uint8_t value)
{
    uint8_t txData[2] = {reg, value};
    select();
    HAL_SPI_Transmit(&m_hspi, txData, 2, 5);
    deselect();
}

bool ImuDriver::readRegisters(uint8_t reg, uint8_t* rxBuffer, uint16_t length)
{
    // Apply read bit
    uint8_t txReg = reg | 0x80;

    select();

    // Send address
    if (HAL_SPI_Transmit(&m_hspi, &txReg, 1, 5) != HAL_OK) {
        deselect();
        return false;
    }

    // Wait for all the data
    if (HAL_SPI_Receive(&m_hspi, rxBuffer, length, 5) != HAL_OK) {
        deselect();
        return false;
    }

    deselect();
    return true;
}

void ImuDriver::select()
{
    HAL_GPIO_WritePin(m_csPort, m_csPin, GPIO_PIN_RESET);
}

void ImuDriver::deselect()
{
    HAL_GPIO_WritePin(m_csPort, m_csPin, GPIO_PIN_SET);
}

bool ImuDriver::readSensorData(float& ax, float& ay, float& az, float& gx, float& gy, float& gz,
                               float& tempC)
{
    if (!m_isInit)
        return false;

    uint8_t buffer[14];

    // Read accel + gyro
    if (!readRegisters(0x2D, buffer, 14)) {
        return false;
    }

    // Accel is bytes 0-5. Temp is 6-7. Gyro is 8-13.
    int16_t rawAx = (buffer[0] << 8) | buffer[1];
    int16_t rawAy = (buffer[2] << 8) | buffer[3];
    int16_t rawAz = (buffer[4] << 8) | buffer[5];

    int16_t rawGx = (buffer[6] << 8) | buffer[7];
    int16_t rawGy = (buffer[8] << 8) | buffer[9];
    int16_t rawGz = (buffer[10] << 8) | buffer[11];

    int16_t rawTemp = (buffer[12] << 8) | buffer[13];

    // Convert to G force
    ax = rawAx / 16384.0f;
    ay = rawAy / 16384.0f;
    az = rawAz / 16384.0f;

    // Convert temp
    tempC = (rawTemp / 333.87f) + 21.0f;

    // Convert to dps
    gx = rawGx / 131.0f;
    gy = rawGy / 131.0f;
    gz = rawGz / 131.0f;

    return true;
}