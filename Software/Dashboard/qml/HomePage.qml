import QtQuick
import Dashboard

Item {
    id: root

    // Gauges
    Item {
        id: gaugeContainer
        anchors {
            top: root.top
            left: root.left
            right: root.right
            bottom: separator.top
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -50

            spacing: 60

            // Speed
            SpeedGauge {
                width: 350
                height: 350
                speedValue: 0
            }
            // Rpm
            RpmGauge {
                width: 350
                height: 350
                rpmValue: UsbDashReceiver.dashData.rpm
            }
        }
    }

    // Separator
    Rectangle {
        id: separator
        height: 2
        color: AppTheme.colorActn
        opacity: 0.5
        anchors {
            left: root.left
            leftMargin: 30
            right: root.right
            rightMargin: 30
            bottom: bottomInfo.top
            bottomMargin: 20
        }
    }

    // Bottom info
    Row {
        id: bottomInfo
        anchors {
            left: root.left
            leftMargin: 30
            right: root.right
            rightMargin: 30
            bottom: root.bottom
            bottomMargin: 20
        }

        spacing: (width - (120 * 4)) / 3

        // Motor temp
        InfoBlock {
            width: 120
            labelText: "MOTOR"
            valueText: UsbDashReceiver.dashData.motorTemp.toFixed(1)
            valueUnit: "°C"
            valueColor: UsbDashReceiver.dashData.motorTemp
                        > 85 ? AppTheme.colorSec : AppTheme.colorNeut100
        }

        // Mosfet temp
        InfoBlock {
            width: 120
            labelText: "MOSFET"
            valueText: UsbDashReceiver.dashData.mosfetTemp.toFixed(1)
            valueUnit: "°C"
            valueColor: UsbDashReceiver.dashData.mosfetTemp
                        > 75 ? AppTheme.colorSec : AppTheme.colorNeut100
        }

        // Throttle
        InfoBlock {
            width: 120
            labelText: "THROTTLE"
            valueText: Math.round(UsbDashReceiver.dashData.throttlePercent)
            valueUnit: "%"
        }

        // Brake
        InfoBlock {
            width: 120
            labelText: "BRAKE"
            valueText: UsbDashReceiver.dashData.isBrakePressed ? "ON" : "OFF"
            valueColor: UsbDashReceiver.dashData.isBrakePressed ? AppTheme.colorAlert : AppTheme.colorNeut100
        }
    }
}
