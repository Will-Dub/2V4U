#include "usbdashreceiver.h"

constexpr bool isSet(unsigned flags, int bit)
{
    return (flags >> bit) & 1u;
}

UsbDashReceiver::UsbDashReceiver(QObject *parent)
    : QObject{parent},
      m_serialPort(new QSerialPort(this)),
      m_reconnectTimer(new QTimer(this)),
      m_publishTimer(new QTimer(this))
{
    m_serialPort = new QSerialPort(this);
    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setInterval(1000);
    m_publishTimer->setInterval(33);

    connect(m_serialPort, &QSerialPort::errorOccurred, this, &UsbDashReceiver::handleError);
    connect(m_reconnectTimer, &QTimer::timeout, this, &UsbDashReceiver::handleAutoreconnect);
    connect(m_serialPort, &QSerialPort::readyRead, this, &UsbDashReceiver::handleRead);
    connect(m_publishTimer, &QTimer::timeout, this, &UsbDashReceiver::publish);

    m_publishTimer->start();
}

void UsbDashReceiver::start(quint16 targetVid, quint16 targetPid)
{
    m_targetVid = targetVid;
    m_targetPid = targetPid;
    handleAutoreconnect();
}

const DashData &UsbDashReceiver::dashData() const { return m_data; }

bool UsbDashReceiver::isConnected() const { return m_serialPort->isOpen(); }

bool UsbDashReceiver::isDataOld() const
{
    return m_isDataOld;
}

void UsbDashReceiver::handleError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::ResourceError)
        return;

    qWarning() << "Serial port error " << m_serialPort->errorString();
    m_serialPort->close();
    emit connectionStatusChanged();
    startReconnecting();
}

void UsbDashReceiver::handleAutoreconnect()
{
    if (m_serialPort != nullptr && m_serialPort->isOpen())
    {
        m_reconnectTimer->stop();
        return;
    }

    QString foundPortName = "";

    for (const QSerialPortInfo &portInfo : QSerialPortInfo::availablePorts())
    {
        if (portInfo.hasVendorIdentifier() && portInfo.hasProductIdentifier())
        {
            if (portInfo.vendorIdentifier() == m_targetVid && portInfo.productIdentifier() == m_targetPid)
            {
                foundPortName = portInfo.portName();
                qDebug() << "Found usb port:" << foundPortName;
                break;
            }
        }
    }

    // Not found
    if (foundPortName.isEmpty())
    {
        if (!m_reconnectTimer->isActive())
        {
            m_reconnectTimer->start();
        }
        return;
    }

    // Device found
    m_serialPort->setPortName(foundPortName);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);

    if (m_serialPort->open(QIODevice::ReadOnly))
    {
        qDebug() << "Successfully connected";
        m_reconnectTimer->stop();
        emit connectionStatusChanged();
    }
    else
    {
        qWarning() << "Found the device but failed to open";
        startReconnecting();
    }
}

void UsbDashReceiver::publish()
{
    const bool fresh = m_sinceLastPacket.isValid() && !m_sinceLastPacket.hasExpired(DATA_LIFESPAN_MS);
    if (fresh != m_isDataOld)
    {
        m_isDataOld = fresh;
        emit dataOldChanged();
    }

    if (!m_newDataFlag)
        return;

    m_newDataFlag = false;
    m_data = m_pendingData;
    emit dashDataChanged();
}

void UsbDashReceiver::startReconnecting()
{
    if (!m_reconnectTimer->isActive())
    {
        m_reconnectTimer->start();
    }
}

void UsbDashReceiver::handleRead()
{
    while (m_serialPort->canReadLine())
    {
        QString line = QString::fromUtf8(m_serialPort->readLine().trimmed());
        if (line.isEmpty())
            continue;

        if (line.startsWith("$VCU,"))
        {
            parseVcuPacket(line.split(','));
        }
        else if (line.startsWith("$BMS,"))
        {
            parseBMSPacket(line.split(','));
        }
        else
        {
            emit debugLogReceived(line);
        }
    }
}

void UsbDashReceiver::parseVcuPacket(const QStringList &parts)
{
    if (parts.size() != VcuFieldCount)
        return;

    m_pendingData.tickMs = parts[VcuTick].toInt();
    m_pendingData.stateStr = stateToString(parts[VcuState].toUInt());
    m_pendingData.faultStr = faultCodeToString(static_cast<Shared::FaultCode>(parts[VcuFaultCode].toUInt()));

    m_pendingData.rpm = parts[VcuRpm].toInt();
    m_pendingData.motorTemp = parts[VcuMotorTemp].toFloat();
    m_pendingData.mosfetTemp = parts[VcuMosfetTemp].toFloat();
    m_pendingData.motorCurrent = parts[VcuMotorCurrent].toFloat();
    m_pendingData.inputCurrent = parts[VcuInputCurrent].toFloat();
    m_pendingData.inputVoltage = parts[VcuInputVoltage].toFloat();
    m_pendingData.canRxCount = parts[VcuCanRx].toInt();
    m_pendingData.canTxCount = parts[VcuCanTx].toInt();
    m_pendingData.canTxFailCount = parts[VcuCanTxFail].toInt();

    m_pendingData.accelX = parts[VcuAccelX].toFloat();
    m_pendingData.accelY = parts[VcuAccelY].toFloat();
    m_pendingData.accelZ = parts[VcuAccelZ].toFloat();

    m_pendingData.throttlePercent = parts[VcuThrottle].toFloat() * 100;

    const unsigned in = parts[VcuInputFlags].toUInt();
    m_pendingData.isBrakePressed = isSet(in, InBrake);
    m_pendingData.isStartPressed = isSet(in, InStart);
    m_pendingData.isEStopPressed = isSet(in, InEStop);
    m_pendingData.isForwardDirection = isSet(in, InForward);
    m_pendingData.isThrottleValid = isSet(in, InThrottleValid);

    const unsigned out = parts[VcuOutputFlags].toUInt();
    m_pendingData.isMotorEnabled = isSet(out, OutMotor);
    m_pendingData.isPrechargeOn = isSet(out, OutPrecharge);
    m_pendingData.isContactorOn = isSet(out, OutContactor);

    m_sinceLastPacket.restart();
    m_newDataFlag = true;
}

void UsbDashReceiver::parseBMSPacket(const QStringList &parts)
{
    if (parts.size() != BmsFieldCount)
        return;

    m_pendingData.isBmsOk = (parts[BmsOk] == "1");
    m_pendingData.bmsVoltage = parts[BmsVoltage].toFloat();
    m_pendingData.bmsCurrent = parts[BmsCurrent].toFloat();

    // Temperatures
    QVariantList temps;
    for (int i = 0; i < Shared::BMS_TEMP_COUNT; ++i)
    {
        const double temp = parts[BmsFirstTemp + i].toDouble();
        temps.append(temp == Shared::BMS_TEMP_MISSING ? qQNaN() : temp);
    }
    m_pendingData.packTemperatures = temps;

    // Convert to volt
    QVariantList cells;
    for (int i = 0; i < Shared::BMS_CELL_COUNT; ++i)
    {
        cells.append(parts[BmsFirstCell + i].toInt() / 1000.0);
    }
    m_pendingData.cellVoltages = cells;

    m_newDataFlag = true;
}

QString UsbDashReceiver::stateToString(uint8_t stateId)
{
    switch (stateId)
    {
    case 0:
        return "INIT";
    case 1:
        return "STANDBY";
    case 2:
        return "PRECHARGE";
    case 3:
        return "DRIVE";
    case 4:
        return "FAULT";
    case 5:
        return "SHUTDOWN";
    default:
        return "UNKNOWN";
    }
}

QString UsbDashReceiver::faultCodeToString(Shared::FaultCode faultCode)
{
    switch (faultCode)
    {
    case Shared::FaultCode::ESTOP:
        return "E-STOP";
    case Shared::FaultCode::THROTTLE_INVALID:
        return "THROTTLE";
    case Shared::FaultCode::PRECHARGE_TIMEOUT:
        return "PRECHARGE";
    case Shared::FaultCode::VESC_LOST:
        return "VESC LOST";
    case Shared::FaultCode::VESC_LOW_VOLTAGE:
        return "VESC LOW VOLT";
    case Shared::FaultCode::VESC_HIGH_CURRENT:
        return "VESC HIGH AMP";
    case Shared::FaultCode::VESC_HIGH_TEMPERATURE:
        return "VESC HIGH TEMP";
    case Shared::FaultCode::BMS_LOST:
        return "BMS LOST";
    case Shared::FaultCode::BMS_HIGH_CELL_DELTA:
        return "BMS HIGH DELTA";
    case Shared::FaultCode::BMS_HIGH_CURRENT:
        return "BMS HIGH AMP";
    case Shared::FaultCode::BMS_PROTECTION:
        return "BMS PROTECTION";
    case Shared::FaultCode::BMS_LOW_VOLTAGE:
        return "BMS LOW VOLT";
    case Shared::FaultCode::BMS_HIGH_TEMPERATURE:
        return "BMS HIGH TEMP";
    default:
        return "";
    }
}
