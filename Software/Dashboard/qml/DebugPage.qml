import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Dashboard

Rectangle {
    id: debugRoot
    width: 1280
    height: 720
    color: AppTheme.colorNeut0

    component QuickStat: Column {
        property string labelText
        property string valueText
        property string unitText

        width: parent.width
        spacing: 0

        Text {
            text: labelText
            color: AppTheme.colorNeut70
            font.pixelSize: AppTheme.sizeExtraSmall
            font.family: AppTheme.fontMono
            font.letterSpacing: 2
        }

        Row {
            spacing: 8
            anchors.baselineOffset: 0
            Text {
                id: valTxt
                text: valueText
                color: AppTheme.colorNeut100
                font.pixelSize: 40
                font.family: AppTheme.fontMono
                font.bold: true
            }
            Text {
                text: unitText
                color: AppTheme.colorActn
                font.pixelSize: AppTheme.sizeExtraSmall
                font.family: AppTheme.fontMono
                anchors.baseline: valTxt.baseline
            }
        }

        Item {
            width: 1
            height: 10
        }

        Rectangle {
            width: parent.width
            height: 2
            color: AppTheme.colorActn
            opacity: 0.5
        }
    }

    component SensorRow: Item {
        property string signalName
        property string sourceName
        property string valText
        property string unitText
        property string ageText
        property string stateText: "OK"
        property color stateColor: AppTheme.colorActn

        width: parent.width
        height: 50

        Rectangle {
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
            color: AppTheme.colorNeut30
        }

        RowLayout {
            anchors.fill: parent
            anchors.bottomMargin: 5

            Column {
                Layout.fillWidth: true
                Text {
                    width: parent.width
                    text: signalName
                    color: AppTheme.colorNeut100
                    font.pixelSize: AppTheme.sizeSmall
                }
                Text {
                    width: parent.width
                    text: sourceName
                    color: AppTheme.colorNeut60
                    font.pixelSize: AppTheme.sizeExtraSmall
                    font.family: AppTheme.fontMono
                }
            }

            Item {
                Layout.preferredWidth: 120
                Layout.fillHeight: true

                Row {
                    anchors.centerIn: parent
                    spacing: 4
                    Text {
                        id: rowVal
                        text: valText
                        color: AppTheme.colorNeut100
                        font.pixelSize: AppTheme.sizeSmall
                        font.family: AppTheme.fontMono
                        font.bold: true
                    }
                    Text {
                        text: unitText
                        color: AppTheme.colorNeut70
                        font.pixelSize: AppTheme.sizeExtraSmall
                        font.family: AppTheme.fontMono
                        anchors.baseline: rowVal.baseline
                    }
                }
            }

            Text {
                Layout.preferredWidth: 80
                horizontalAlignment: Text.AlignHCenter
                text: ageText
                color: AppTheme.colorNeut70
                font.pixelSize: AppTheme.sizeExtraSmall
                font.family: AppTheme.fontMono
            }
            Text {
                Layout.preferredWidth: 40
                horizontalAlignment: Text.AlignHCenter
                text: stateText
                color: stateColor
                font.pixelSize: AppTheme.sizeExtraSmall
                font.family: AppTheme.fontMono
            }
        }
    }

    component SysBar: Column {
        property string labelText
        property string pctText
        property real fillRatio: 0.5

        width: parent.width
        spacing: 4

        RowLayout {
            width: parent.width
            Text {
                text: labelText
                color: AppTheme.colorNeut70
                font.pixelSize: AppTheme.sizeExtraSmall
                font.family: AppTheme.fontMono
                Layout.fillWidth: true
            }
            Text {
                text: pctText
                color: AppTheme.colorNeut100
                font.pixelSize: AppTheme.sizeExtraSmall
                font.family: AppTheme.fontMono
            }
        }

        Rectangle {
            width: parent.width
            height: 4
            color: AppTheme.colorNeut30
            Rectangle {
                width: parent.width * fillRatio
                height: parent.height
                color: AppTheme.colorActn
            }
        }
    }

    component StatusBadge: Rectangle {
        required property string statusText
        required property color statusColor

        width: badgeText.implicitWidth + 30
        height: 32
        radius: height / 2

        color: Qt.rgba(statusColor.r, statusColor.g, statusColor.b, 0.15)
        border.color: statusColor
        border.width: 1

        Text {
            id: badgeText
            anchors.centerIn: parent
            text: parent.statusText
            color: parent.statusColor
            font.pixelSize: AppTheme.sizeExtraSmall
            font.family: AppTheme.fontMono
            font.bold: true
        }
    }

    component DigitalIndicator: Row {
        property string label
        property bool isActive: false

        spacing: 8

        Rectangle {
            width: 12
            height: 12
            radius: 6
            color: isActive ? AppTheme.colorActn : AppTheme.colorNeut30
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            text: label
            color: isActive ? AppTheme.colorNeut100 : AppTheme.colorNeut70
            font.pixelSize: AppTheme.sizeExtraSmall
            font.family: AppTheme.fontMono
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    // ==========================================
    // MAIN LAYOUT
    // ==========================================
    Item {
        id: headerArea
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 60

        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: 1
            color: AppTheme.colorNeut30
        }
        Text {
            anchors {
                left: parent.left
                leftMargin: 10
                verticalCenter: parent.verticalCenter
            }

            text: "Debug"
            color: AppTheme.colorNeut100
            font.pixelSize: AppTheme.sizeHeader
            font.bold: true
        }

        StatusBadge {
            anchors {
                right: parent.right
                rightMargin: 30
                verticalCenter: parent.verticalCenter
            }

            statusText: {
                if (!UsbDashReceiver.isConnected)
                    return "OFFLINE"
                return UsbDashReceiver.dashData.stateStr
            }

            statusColor: {
                if (!UsbDashReceiver.isConnected)
                    return AppTheme.colorAlert
                else if (UsbDashReceiver.dashData.stateStr === "FAULT")
                    return AppTheme.colorAlert
                return AppTheme.colorActn
            }
        }
    }

    RowLayout {
        anchors {
            top: headerArea.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 10
        }
        spacing: 10

        // LEFT COLUMN
        Column {
            Layout.preferredWidth: 175
            Layout.fillHeight: true
            spacing: 40

            QuickStat {
                labelText: "RPM"
                valueText: UsbDashReceiver.dashData.rpm
                unitText: "rpm"
            }
            QuickStat {
                labelText: "THROTTLE"
                valueText: UsbDashReceiver.dashData.throttlePercent
                unitText: "%"
            }
        }

        Rectangle {
            Layout.preferredWidth: 1
            Layout.fillHeight: true
            color: AppTheme.colorNeut30
        }

        // MIDDLE COLUMN
        Column {
            Layout.fillWidth: true
            Layout.minimumWidth: 300
            Layout.fillHeight: true
            spacing: 10

            Row {}

            RowLayout {
                width: parent.width

                Text {
                    text: "Sensor"
                    color: AppTheme.colorNeut100
                    font.pixelSize: AppTheme.sizeSmall
                    font.bold: true
                    Layout.fillWidth: true
                }


                /*Text {
                    text: "5 / 5 VALID"
                    color: AppTheme.colorActn
                    font.pixelSize: AppTheme.sizeExtraSmall
                    font.family: AppTheme.fontMono
                }*/
            }

            RowLayout {
                width: parent.width
                Text {
                    text: "SIGNAL / SOURCE"
                    color: AppTheme.colorNeut60
                    font.pixelSize: AppTheme.sizeExtraSmall
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                }
                Text {
                    text: "VALUE"
                    color: AppTheme.colorNeut60
                    font.pixelSize: AppTheme.sizeExtraSmall
                    Layout.preferredWidth: 120
                    horizontalAlignment: Text.AlignHCenter
                }
                Text {
                    text: "AGE"
                    color: AppTheme.colorNeut60
                    font.pixelSize: AppTheme.sizeExtraSmall
                    Layout.preferredWidth: 80
                    horizontalAlignment: Text.AlignHCenter
                }
                Text {
                    text: "STATE"
                    color: AppTheme.colorNeut60
                    font.pixelSize: AppTheme.sizeExtraSmall
                    Layout.preferredWidth: 40
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: AppTheme.colorNeut30
            }

            Flickable {
                width: parent.width
                height: 200
                clip: true
                contentHeight: sensorCol.implicitHeight

                Column {
                    id: sensorCol
                    width: parent.width

                    SensorRow {
                        signalName: "RPM"
                        sourceName: "VESC CAN"
                        valText: UsbDashReceiver.dashData.rpm
                        unitText: "rpm"
                    }
                    SensorRow {
                        signalName: "Motor Current"
                        sourceName: "VESC CAN"
                        valText: UsbDashReceiver.dashData.motorCurrent
                        unitText: "Amp"
                    }
                    SensorRow {
                        signalName: "Input Current"
                        sourceName: "VESC CAN"
                        valText: UsbDashReceiver.dashData.inputCurrent
                        unitText: "Amp"
                    }
                    SensorRow {
                        signalName: "Throttle"
                        sourceName: "ADC"
                        valText: UsbDashReceiver.dashData.throttlePercent
                        unitText: "%"
                    }
                    SensorRow {
                        signalName: "Input Voltage"
                        sourceName: "VESC CAN"
                        valText: UsbDashReceiver.dashData.inputVoltage
                        unitText: "V"
                    }
                    SensorRow {
                        signalName: "Mosfet Temp"
                        sourceName: "ADC"
                        valText: UsbDashReceiver.dashData.mosfetTemp
                        unitText: "°C"
                    }
                    SensorRow {
                        signalName: "Motor Temp"
                        sourceName: "ADC"
                        valText: UsbDashReceiver.dashData.motorTemp
                        unitText: "°C"
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded

                    contentItem: Rectangle {
                        implicitWidth: 4
                        radius: 2
                        color: AppTheme.colorNeut60
                        opacity: parent.active ? 0.8 : 0.0
                        Behavior on opacity {
                            NumberAnimation {
                                duration: 300
                            }
                        }
                    }
                }
            }

            Column {
                width: parent.width
                spacing: 15
                Text {
                    text: "Digital"
                    color: AppTheme.colorNeut100
                    font.pixelSize: AppTheme.sizeSmall
                    font.bold: true
                }

                GridLayout {
                    columns: 5
                    columnSpacing: 20
                    rowSpacing: 10

                    DigitalIndicator {
                        label: "Brake"
                        isActive: UsbDashReceiver.dashData.isBrakePressed
                    }
                    DigitalIndicator {
                        label: "EStop"
                        isActive: UsbDashReceiver.dashData.isEStopPressed
                    }
                    DigitalIndicator {
                        label: "Fwd"
                        isActive: UsbDashReceiver.dashData.isForwardDirection
                    }
                    DigitalIndicator {
                        label: "Start"
                        isActive: UsbDashReceiver.dashData.isStartPressed
                    }
                }
            }
        }

        Rectangle {
            Layout.preferredWidth: 1
            Layout.fillHeight: true
            color: AppTheme.colorNeut30
        }

        // RIGHT COLUMN
        ColumnLayout {
            Layout.preferredWidth: 180
            Layout.fillHeight: true
            spacing: 30

            Column {
                Layout.fillWidth: true
                spacing: 15
                Text {
                    text: "CAN bus"
                    color: AppTheme.colorNeut100
                    font.pixelSize: AppTheme.sizeSmall
                    font.bold: true
                }

                RowLayout {
                    width: parent.width
                    Column {
                        Layout.fillWidth: true
                        Text {
                            text: "RX"
                            color: AppTheme.colorNeut70
                            font.pixelSize: AppTheme.sizeExtraSmall
                        }
                        Text {
                            text: UsbDashReceiver.dashData.canRxCount
                            color: AppTheme.colorNeut100
                            font.pixelSize: AppTheme.sizeSmall
                            font.bold: true
                        }
                    }
                    Column {
                        Layout.fillWidth: true
                        Text {
                            text: "TX"
                            color: AppTheme.colorNeut70
                            font.pixelSize: AppTheme.sizeExtraSmall
                        }
                        Text {
                            text: UsbDashReceiver.dashData.canTxCount
                            color: AppTheme.colorNeut100
                            font.pixelSize: AppTheme.sizeSmall
                            font.bold: true
                        }
                    }
                    Column {
                        Layout.fillWidth: true
                        Text {
                            text: "ERRORS"
                            color: AppTheme.colorNeut70
                            font.pixelSize: AppTheme.sizeExtraSmall
                        }
                        Text {
                            text: UsbDashReceiver.dashData.canTxFailCount
                            color: AppTheme.colorActn
                            font.pixelSize: AppTheme.sizeSmall
                            font.bold: true
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: AppTheme.colorNeut30
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Text {
                Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                Layout.rightMargin: 10
                Layout.bottomMargin: 10

                color: AppTheme.colorNeut60
                font.pixelSize: AppTheme.sizeExtraSmall
                font.family: AppTheme.fontMono

                text: {
                    // Milli to sec
                    let totalSeconds = Math.floor(
                            UsbDashReceiver.dashData.tickMs / 1000)

                    // Get hours, minute, seconds
                    let h = Math.floor(totalSeconds / 3600)
                    let m = Math.floor((totalSeconds % 3600) / 60)
                    let s = totalSeconds % 60

                    // Format
                    let hStr = h.toString().padStart(2, '0')
                    let mStr = m.toString().padStart(2, '0')
                    let sStr = s.toString().padStart(2, '0')

                    return hStr + ":" + mStr + ":" + sStr
                }
            }
        }
    }
}
