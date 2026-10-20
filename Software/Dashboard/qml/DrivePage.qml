import QtQuick
import Dashboard

Item {
    id: root

    component InfoBlock: Column {
        required property string labelText
        required property string valueText
        property string valueUnit: ""
        property color valueColor: AppTheme.colorNeut100

        spacing: 4

        Text {
            text: labelText
            color: AppTheme.colorNeut70
            font.pixelSize: AppTheme.sizeExtraSmall
            font.bold: true
        }
        Row {
            spacing: 4

            Text {
                id: valText
                text: valueText
                color: valueColor
                font.pixelSize: AppTheme.sizeNormal
                font.family: AppTheme.fontMono
                font.bold: true
            }

            Text {
                text: valueUnit
                color: AppTheme.colorNeut70
                font.pixelSize: AppTheme.sizeExtraSmall
                font.bold: true

                anchors.baseline: valText.baseline
            }
        }
    }

    Row {
        anchors {
            left: root.left
            leftMargin: 30
            right: root.right
            rightMargin: 30
            bottom: root.bottom
            bottomMargin: 20
        }

        spacing: (width - (120 * 4)) / 3

        InfoBlock {
            width: 120
            labelText: "MOTOR"
            valueText: UsbDashReceiver.dashData.motorTemp.toFixed(1)
            valueUnit: "°C"
            valueColor: UsbDashReceiver.dashData.motorTemp > 85 ? AppTheme.colorSec : AppTheme.colorNeut100
        }

        InfoBlock {
            width: 120
            labelText: "MOSFET"
            valueText: UsbDashReceiver.dashData.mosfetTemp.toFixed(1)
            valueUnit: "°C"
            valueColor: UsbDashReceiver.dashData.mosfetTemp > 75 ? AppTheme.colorSec : AppTheme.colorNeut100
        }

        InfoBlock {
            width: 120
            labelText: "THROTTLE"
            valueText: Math.round(UsbDashReceiver.dashData.throttlePercent)
            valueUnit: "%"
        }

        InfoBlock {
            width: 120
            labelText: "BRAKE"
            valueText: UsbDashReceiver.dashData.isBrakePressed ? "ON" : "OFF"
            valueColor: UsbDashReceiver.dashData.isBrakePressed ? AppTheme.colorAlert : AppTheme.colorNeut100
        }
    }
}
