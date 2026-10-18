#ifndef USBDASHRECEIVER_H
#define USBDASHRECEIVER_H

#include <QObject>
#include <QSerialPort>
#include <qqmlintegration.h>
#include "VcuComms.h"
#include <QDebug>

struct DashData {
    Q_GADGET
    Q_PROPERTY(int tickMs MEMBER tickMs)
    Q_PROPERTY(QString stateStr MEMBER stateStr)

    Q_PROPERTY(int rpm MEMBER rpm)
    Q_PROPERTY(float motorTemp MEMBER motorTemp)
    Q_PROPERTY(float mosfetTemp MEMBER mosfetTemp)

    Q_PROPERTY(float accelX MEMBER accelX)
    Q_PROPERTY(float accelY MEMBER accelY)
    Q_PROPERTY(float accelZ MEMBER accelZ)

    Q_PROPERTY(float throttlePercent MEMBER throttlePercent)

    Q_PROPERTY(bool isBrakePressed MEMBER isBrakePressed)
    Q_PROPERTY(bool isStartPressed MEMBER isStartPressed)
    Q_PROPERTY(bool isEStopPressed MEMBER isEStopPressed)
    Q_PROPERTY(bool isMotorEnabled MEMBER isMotorEnabled)
    Q_PROPERTY(bool isPrechargeOn MEMBER isPrechargeOn)
    Q_PROPERTY(bool isContactorOn MEMBER isContactorOn)

public:
    int tickMs;
    QString stateStr = "INIT";
    int rpm = 0;
    float motorTemp = 0.0f;
    float mosfetTemp = 0.0f;
    float accelX = 0.0f;
    float accelY = 0.0f;
    float accelZ = 0.0f;
    float throttlePercent = 0.0f;

    bool isBrakePressed = false;
    bool isStartPressed = false;
    bool isEStopPressed = false;
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
    bool init(const QString& portName);

    DashData dashData() const;
    bool isConnected() const;

signals:
    void dashDataChanged();
    void connectionStatusChanged();
    void debugLogReceived(QString message);

private slots:
    void handleRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort m_serial;
    DashData m_data;

    void parseVcuPacket(const QString& line);
    QString stateToString(uint8_t stateId);

};

#endif // USBDASHRECEIVER_H
