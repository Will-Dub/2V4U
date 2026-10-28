#ifndef USBDASHRECEIVER_H
#define USBDASHRECEIVER_H

#include <QObject>
#include <QSerialPort>
#include <qqmlintegration.h>
#include "VcuComms.h"
#include <QDebug>
#include <QTimer>
#include <QSerialPortInfo>

struct DashData {
    Q_GADGET
    Q_PROPERTY(int tickMs MEMBER tickMs)
    Q_PROPERTY(QString stateStr MEMBER stateStr)

    Q_PROPERTY(int rpm MEMBER rpm)
    Q_PROPERTY(float motorTemp MEMBER motorTemp)
    Q_PROPERTY(float mosfetTemp MEMBER mosfetTemp)
    Q_PROPERTY(float motorCurrent MEMBER motorCurrent)
    Q_PROPERTY(float inputCurrent MEMBER inputCurrent)
    Q_PROPERTY(float inputVoltage MEMBER inputVoltage)
    Q_PROPERTY(int canRxCount MEMBER canRxCount)
    Q_PROPERTY(int canTxCount MEMBER canTxCount)
    Q_PROPERTY(int canTxFailCount MEMBER canTxFailCount)

    Q_PROPERTY(float accelX MEMBER accelX)
    Q_PROPERTY(float accelY MEMBER accelY)
    Q_PROPERTY(float accelZ MEMBER accelZ)

    Q_PROPERTY(float throttlePercent MEMBER throttlePercent)

    Q_PROPERTY(bool isBrakePressed MEMBER isBrakePressed)
    Q_PROPERTY(bool isStartPressed MEMBER isStartPressed)
    Q_PROPERTY(bool isEStopPressed MEMBER isEStopPressed)
    Q_PROPERTY(bool isForwardDirection MEMBER isForwardDirection)
    Q_PROPERTY(bool isThrottleValid MEMBER isThrottleValid)
    Q_PROPERTY(bool isMotorEnabled MEMBER isMotorEnabled)
    Q_PROPERTY(bool isPrechargeOn MEMBER isPrechargeOn)
    Q_PROPERTY(bool isContactorOn MEMBER isContactorOn)

public:
    int tickMs = 0;
    QString stateStr = "";
    int rpm = 0;
    float motorTemp = 0.0f;
    float mosfetTemp = 0.0f;
    float accelX = 0.0f;
    float accelY = 0.0f;
    float accelZ = 0.0f;
    float throttlePercent = 0.0f;
    float motorCurrent = 0.0f;
    float inputCurrent = 0.0f;
    float inputVoltage = 0.0f;
    int canRxCount = 0;
    int canTxCount = 0;
    int canTxFailCount = 0;

    bool isBrakePressed = false;
    bool isStartPressed = false;
    bool isEStopPressed = false;
    bool isForwardDirection = false;
    bool isThrottleValid = false;
    bool isMotorEnabled = false;
    bool isPrechargeOn = false;
    bool isContactorOn = false;
};
Q_DECLARE_METATYPE(DashData)

class UsbDashReceiver : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(DashData dashData READ dashData NOTIFY dashDataChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionStatusChanged FINAL)

public:
    explicit UsbDashReceiver(QObject *parent = nullptr);
    void start(quint16 targetVid, quint16 targetPid);

    DashData dashData() const;
    bool isConnected() const;

signals:
    void dashDataChanged();
    void connectionStatusChanged();
    void debugLogReceived(QString message);

private slots:
    void handleRead();
    void handleError(QSerialPort::SerialPortError error);
    void handleAutoreconnect();

private:
    QSerialPort *m_serialPort;
    QTimer *m_reconnectTimer;
    DashData m_data;
    quint16 m_targetVid;
    quint16 m_targetPid;

    void parseVcuPacket(const QString& line);
    QString stateToString(uint8_t stateId);

};

#endif // USBDASHRECEIVER_H
