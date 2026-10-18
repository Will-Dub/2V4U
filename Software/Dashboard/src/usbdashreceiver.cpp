#include "usbdashreceiver.h"

UsbDashReceiver::UsbDashReceiver(QObject *parent)
    : QObject{parent}
{
    connect(&m_serial, &QSerialPort::errorOccurred, this, &UsbDashReceiver::handleError);
}

bool UsbDashReceiver::init(const QString &portName)
{
    if (m_serial.isOpen()) {
        m_serial.close();
    }

    m_serial.setPortName(portName);
    m_serial.setBaudRate(QSerialPort::Baud115200);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serial.open(QIODevice::ReadWrite)) {
        qWarning() << "Failed to open port";
        emit connectionStatusChanged();
        return false;
    }

    connect(&m_serial, &QSerialPort::readyRead, this, &UsbDashReceiver::handleRead);

    qInfo() << "Successfully opened port:" << portName;
    emit connectionStatusChanged();
    return true;
}

DashData UsbDashReceiver::dashData() const { return m_data; }

bool UsbDashReceiver::isConnected() const { return m_serial.isOpen(); }

void UsbDashReceiver::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qWarning() << "Serial port error " << m_serial.errorString();
        m_serial.close();
        emit connectionStatusChanged();
    }
}

void UsbDashReceiver::handleRead()
{
    while (m_serial.canReadLine()) {
        QString line = QString::fromUtf8(m_serial.readLine().trimmed());
        if (line.isEmpty()) continue;

        if (line.startsWith("$VCU,")) {
            parseVcuPacket(line);
        } else {
            emit debugLogReceived(line);
        }
    }
}

void UsbDashReceiver::parseVcuPacket(const QString& line)
{
    QStringList parts = line.split(',');

    if (parts.size() != 11) return;

    m_data.tickMs = parts[1].toInt();
    m_data.stateStr = stateToString(parts[2].toUInt());
    m_data.rpm = parts[3].toInt();
    m_data.motorTemp = parts[4].toFloat();
    m_data.mosfetTemp = parts[5].toFloat();
    m_data.accelX = parts[6].toFloat();
    m_data.accelY = parts[7].toFloat();
    m_data.accelZ = parts[8].toFloat();
    m_data.throttlePercent = parts[9].toFloat();

    uint8_t flags = parts[10].toUInt();

    m_data.isBrakePressed = (flags & (1 << 0)) != 0;
    m_data.isStartPressed = (flags & (1 << 1)) != 0;
    m_data.isEStopPressed = (flags & (1 << 2)) != 0;
    m_data.isMotorEnabled = (flags & (1 << 3)) != 0;
    m_data.isPrechargeOn  = (flags & (1 << 4)) != 0;
    m_data.isContactorOn  = (flags & (1 << 5)) != 0;
    emit dashDataChanged();
}

QString UsbDashReceiver::stateToString(uint8_t stateId)
{
    switch(stateId) {
    case 0: return "INIT";
    case 1: return "STANDBY";
    case 2: return "PRECHARGE";
    case 3: return "DRIVE";
    case 4: return "FAULT";
    case 5: return "SHUTDOWN";
    default: return "UNKNOWN";
    }
}
