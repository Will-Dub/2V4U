import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Dashboard

Rectangle {
    id: debugRoot
    width: 1280
    height: 720
    color: AppTheme.colorNeut0

    // Content
    RowLayout {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 10
        }
        spacing: 10

        // Left
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

        // Middle
        Column {
            Layout.fillWidth: true
            Layout.minimumWidth: 300
            Layout.fillHeight: true
            spacing: 10

            Row {}

            // Sensor title
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

            // Column titles
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

            // Sensor rows
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

            // Digital inputs
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

        // Right
        ColumnLayout {
            Layout.preferredWidth: 180
            Layout.fillHeight: true
            spacing: 30

            // CAN bus counters
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

            // Run time
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
