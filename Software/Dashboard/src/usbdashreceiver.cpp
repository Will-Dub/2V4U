#include "usbdashreceiver.h"

UsbDashReceiver::UsbDashReceiver(QObject *parent)
    : QObject{parent}
{
    m_serialPort = new QSerialPort(this);
    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setInterval(1000);

    connect(m_serialPort, &QSerialPort::errorOccurred, this, &UsbDashReceiver::handleError);
    connect(m_reconnectTimer, &QTimer::timeout, this, &UsbDashReceiver::handleAutoreconnect);
    connect(m_serialPort, &QSerialPort::readyRead, this, &UsbDashReceiver::handleRead);
}

void UsbDashReceiver::start(quint16 targetVid, quint16 targetPid)
{
    m_targetVid = targetVid;
    m_targetPid = targetPid;
    handleAutoreconnect();
}

DashData UsbDashReceiver::dashData() const { return m_data; }

bool UsbDashReceiver::isConnected() const { return m_serialPort->isOpen(); }

void UsbDashReceiver::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qWarning() << "Serial port error " << m_serialPort->errorString();
        m_serialPort->close();
        emit connectionStatusChanged();

        if (!m_reconnectTimer->isActive()) {
            m_reconnectTimer->start();
        }
    }
}

void UsbDashReceiver::handleAutoreconnect()
{
    if (m_serialPort != nullptr && m_serialPort->isOpen()) {
        m_reconnectTimer->stop();
        return;
    }

    QString foundPortName = "";

    for (const QSerialPortInfo &portInfo : QSerialPortInfo::availablePorts()) {
        if (portInfo.hasVendorIdentifier() && portInfo.hasProductIdentifier()) {
            if (portInfo.vendorIdentifier() == m_targetVid && portInfo.productIdentifier() == m_targetPid) {
                foundPortName = portInfo.portName();
                qDebug() << "Found usb port:" << foundPortName;
                break;
            }
        }
    }

    // Not found
    if (foundPortName.isEmpty()) {
        if (!m_reconnectTimer->isActive()) {
            m_reconnectTimer->start();
        }
        return;
    }

    // Device found
    m_serialPort->setPortName(foundPortName);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);

    if (m_serialPort->open(QIODevice::ReadOnly)) {
        qDebug() << "Successfully connected";
        m_reconnectTimer->stop();
        emit connectionStatusChanged();
    } else {
        qWarning() << "Found the device but failed to open";
        if (!m_reconnectTimer->isActive()) {
            m_reconnectTimer->start();
        }
    }
}

void UsbDashReceiver::handleRead()
{
    while (m_serialPort->canReadLine()) {
        QString line = QString::fromUtf8(m_serialPort->readLine().trimmed());
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

    if (parts.size() != 18) return;

    m_data.tickMs = parts[1].toInt();
    m_data.stateStr = stateToString(parts[2].toUInt());
    m_data.rpm = parts[3].toInt();
    m_data.motorTemp = parts[4].toFloat();
    m_data.mosfetTemp = parts[5].toFloat();
    m_data.accelX = parts[6].toFloat();
    m_data.accelY = parts[7].toFloat();
    m_data.accelZ = parts[8].toFloat();
    m_data.throttlePercent = parts[9].toFloat();
    m_data.motorCurrent = parts[12].toFloat();
    m_data.inputCurrent = parts[13].toFloat();
    m_data.inputVoltage = parts[14].toFloat();
    m_data.canRxCount = parts[15].toInt();
    m_data.canTxCount = parts[16].toInt();
    m_data.canTxFailCount = parts[17].toInt();

    uint8_t inputFlags = parts[10].toUInt();
    uint8_t outputFlags = parts[11].toUInt();

    m_data.isBrakePressed = (inputFlags & (1 << 0)) != 0;
    m_data.isStartPressed = (inputFlags & (1 << 1)) != 0;
    m_data.isEStopPressed = (inputFlags & (1 << 2)) != 0;
    m_data.isForwardDirection = (inputFlags & (1 << 3)) != 0;
    m_data.isThrottleValid = (inputFlags & (1 << 4)) != 0;
    m_data.isMotorEnabled = (outputFlags & (1 << 0)) != 0;
    m_data.isPrechargeOn  = (outputFlags & (1 << 1)) != 0;
    m_data.isContactorOn  = (outputFlags & (1 << 2)) != 0;


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
