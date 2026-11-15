#ifndef USBDASHRECEIVER_H
#define USBDASHRECEIVER_H

#include <QObject>
#include <QSerialPort>
#include <qqmlintegration.h>
//#include "VcuComms.h"
#include <QDebug>
#include <QTimer>
#include <QSerialPortInfo>
#include <QVariant>
#include <QElapsedTimer>

#define BMS_MAX_CELL_COUNT 16
#define DATA_LIFESPAN_MS 3000

struct DashData {
    Q_GADGET
    Q_PROPERTY(int tickMs MEMBER tickMs)
    Q_PROPERTY(QString stateStr MEMBER stateStr)

    // VESC
    Q_PROPERTY(int rpm MEMBER rpm)
    Q_PROPERTY(float motorTemp MEMBER motorTemp)
    Q_PROPERTY(float mosfetTemp MEMBER mosfetTemp)
    Q_PROPERTY(float motorCurrent MEMBER motorCurrent)
    Q_PROPERTY(float inputCurrent MEMBER inputCurrent)
    Q_PROPERTY(float inputVoltage MEMBER inputVoltage)
    Q_PROPERTY(int canRxCount MEMBER canRxCount)
    Q_PROPERTY(int canTxCount MEMBER canTxCount)
    Q_PROPERTY(int canTxFailCount MEMBER canTxFailCount)

    // IMU
    Q_PROPERTY(float accelX MEMBER accelX)
    Q_PROPERTY(float accelY MEMBER accelY)
    Q_PROPERTY(float accelZ MEMBER accelZ)

    // Pillot
    Q_PROPERTY(float throttlePercent MEMBER throttlePercent)
    Q_PROPERTY(bool isBrakePressed MEMBER isBrakePressed)
    Q_PROPERTY(bool isStartPressed MEMBER isStartPressed)
    Q_PROPERTY(bool isEStopPressed MEMBER isEStopPressed)
    Q_PROPERTY(bool isForwardDirection MEMBER isForwardDirection)
    Q_PROPERTY(bool isThrottleValid MEMBER isThrottleValid)

    // Outputs
    Q_PROPERTY(bool isMotorEnabled MEMBER isMotorEnabled)
    Q_PROPERTY(bool isPrechargeOn MEMBER isPrechargeOn)
    Q_PROPERTY(bool isContactorOn MEMBER isContactorOn)

    // BMS
    Q_PROPERTY(float packVoltage MEMBER packVoltage)
    Q_PROPERTY(float packCurrent MEMBER packCurrent)
    Q_PROPERTY(float bmsTemp MEMBER bmsTemp)
    Q_PROPERTY(QVariantList cellVoltages MEMBER cellVoltages)

public:
    int tickMs = 0;
    QString stateStr;

    int rpm = 0;
    float motorTemp = 0.0f;
    float mosfetTemp = 0.0f;
    float motorCurrent = 0.0f;
    float inputCurrent = 0.0f;
    float inputVoltage = 0.0f;
    int canRxCount = 0;
    int canTxCount = 0;
    int canTxFailCount = 0;

    float accelX = 0.0f;
    float accelY = 0.0f;
    float accelZ = 0.0f;

    float throttlePercent = 0.0f;
    bool isBrakePressed = false;
    bool isStartPressed = false;
    bool isEStopPressed = false;
    bool isForwardDirection = false;
    bool isThrottleValid = false;

    bool isMotorEnabled = false;
    bool isPrechargeOn = false;
    bool isContactorOn = false;

    float packVoltage = 0.0f;
    float packCurrent = 0.0f;
    float bmsTemp = 0.0f;
    QVariantList cellVoltages;
};
Q_DECLARE_METATYPE(DashData)

class UsbDashReceiver : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(DashData dashData READ dashData NOTIFY dashDataChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionStatusChanged FINAL)
    Q_PROPERTY(bool isDataOld READ isDataOld NOTIFY dataOldChanged FINAL)

public:
    explicit UsbDashReceiver(QObject *parent = nullptr);
    void start(quint16 targetVid, quint16 targetPid);

    const DashData &dashData() const;
    bool isConnected() const;
    bool isDataOld() const;

signals:
    void dashDataChanged();
    void connectionStatusChanged();
    void dataOldChanged();
    void debugLogReceived(QString message);

private slots:
    void handleRead();
    void handleError(QSerialPort::SerialPortError error);
    void handleAutoreconnect();
    void publish();

private:
    void startReconnecting();
    void parseVcuPacket(const QStringList &parts);
    void parseBMSPacket(const QStringList &parts);
    static QString stateToString(uint8_t stateId);

    // $VCU,tick,state,rpm,motorTemp,mosfetTemp,ax,ay,az,throttle,inFlags,outFlags,
    //      motorA,inputA,inputV,canRx,canTx,canTxFail
    enum VcuField {
        VcuTick = 1,
        VcuState,
        VcuRpm,
        VcuMotorTemp,
        VcuMosfetTemp,
        VcuAccelX,
        VcuAccelY,
        VcuAccelZ,
        VcuThrottle,
        VcuInputFlags,
        VcuOutputFlags,
        VcuMotorCurrent,
        VcuInputCurrent,
        VcuInputVoltage,
        VcuCanRx,
        VcuCanTx,
        VcuCanTxFail,
        VcuFieldCount
    };

    // $BMS,packV,packA,tempC,cell1,cell2,...
    enum BmsField {
        BmsPackVoltage = 1,
        BmsPackCurrent,
        BmsTemp,
        BmsFirstCell
    };

    enum InputBit { InBrake, InStart, InEStop, InForward, InThrottleValid };
    enum OutputBit { OutMotor, OutPrecharge, OutContactor };

    QSerialPort *m_serialPort;
    QTimer *m_reconnectTimer;
    QTimer *m_publishTimer;
    QElapsedTimer m_sinceLastPacket;

    DashData m_pendingData{};
    DashData m_data{};
    bool m_newDataFlag = false;
    bool m_isDataOld = false;

    quint16 m_targetVid = 0;
    quint16 m_targetPid = 0;

};

#endif // USBDASHRECEIVER_H
