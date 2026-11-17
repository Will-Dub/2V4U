import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Dashboard

Rectangle {
    id: debugRoot
    width: 1280
    height: 720
    color: AppTheme.colorNeut0

    // Cell voltage limits (LiFePO4)
    readonly property real cellWarnHigh: 3.60
    readonly property real cellAlertHigh: 3.65
    readonly property real cellWarnLow: 3.00
    readonly property real cellAlertLow: 2.80

    // Cell data (16 voltages in volts, empty list shows --)
    readonly property var cellVoltages: {
        let c = UsbDashReceiver.dashData.cellVoltages
        return (c !== undefined && c.length > 0) ? c : []
    }
    // Min, max, average and delta of the cells
    readonly property real cellMin: cellVoltages.length > 0 ? Math.min.apply(
                                                                  null,
                                                                  cellVoltages) : NaN
    readonly property real cellMax: cellVoltages.length > 0 ? Math.max.apply(
                                                                  null,
                                                                  cellVoltages) : NaN
    readonly property real cellDeltaMv: (cellVoltages.length > 0) ? (cellMax - cellMin) * 1000 : NaN

    readonly property real cellMean: {
        if (cellVoltages.length === 0)
            return NaN
        let sum = 0
        for (var i = 0; i < cellVoltages.length; i++)
            sum += cellVoltages[i]
        return sum / cellVoltages.length
    }

    // Bar chart: +-barSpanV around the average, flat bars = balanced pack
    readonly property real barSpanV: 0.060
    // Deviation where a bar starts turning red / is fully red
    readonly property real devStartV: 0.015
    readonly property real devFullV: 0.030

    // Color for one cell voltage: too high or too low is a problem
    function cellColor(v) {
        if (isNaN(v))
            return AppTheme.colorNeut60
        if (v >= cellAlertHigh || v <= cellAlertLow)
            return AppTheme.colorAlert
        if (v >= cellWarnHigh || v <= cellWarnLow)
            return AppTheme.colorWarn
        return AppTheme.colorNeut100
    }

    // Blue when close to the pack average, fading to red as it drifts away.
    // Absolute limits always force full red.
    function cellBarColor(v) {
        if (isNaN(v) || isNaN(cellMean))
            return AppTheme.colorNeut30
        if (v >= cellAlertHigh || v <= cellAlertLow)
            return AppTheme.colorAlert
        let d = Math.abs(v - cellMean)
        let t = Math.min(Math.max((d - devStartV) / (devFullV - devStartV), 0),
                         1)
        let a = AppTheme.colorPrm
        let b = AppTheme.colorAlert
        return Qt.rgba(a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t,
                       a.b + (b.b - a.b) * t, 1)
    }

    // Bar height as 0..1 around the average (0.5 = on the average)
    function cellRatio(v) {
        if (isNaN(v) || isNaN(cellMean))
            return 0
        return Math.min(Math.max(0.5 + (v - cellMean) / (2 * barSpanV), 0), 1)
    }

    function volts(v) {
        return isNaN(v) ? "--" : Number(v).toFixed(3)
    }


    // Content
    RowLayout {
        id: content
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
            spacing: 10

            StatCard {
                labelText: "Voltage"
                valueText: UsbDashReceiver.dashData.isBmsOk ? Number(
                                                                  UsbDashReceiver.dashData.bmsVoltage).toFixed(
                                                                  1) : "--"
                unitText: "V"
            }
            StatCard {
                labelText: "Current"
                valueText: UsbDashReceiver.dashData.isBmsOk ? Number(
                                                                  UsbDashReceiver.dashData.bmsCurrent).toFixed(
                                                                  1) : "--"
                unitText: "A"
            }
        }

        // Separator left to middle
        Rectangle {
            Layout.preferredWidth: 1
            Layout.fillHeight: true
            color: AppTheme.colorNeut30
        }

        // Middle
        ColumnLayout {
            Layout.fillWidth: true
            Layout.minimumWidth: 300
            Layout.fillHeight: true
            spacing: 10

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 8
                color: AppTheme.colorCardBg

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8

                    RowLayout {
                        Layout.fillWidth: true

                        Text {
                            text: UsbDashReceiver.dashData.isBmsOk ? "AVG " + debugRoot.volts(
                                                                         debugRoot.cellMean) + " V   ±" + (debugRoot.barSpanV * 1000).toFixed(
                                                                         0) + " mV" : "--"
                            color: AppTheme.colorNeut70
                            font.pixelSize: AppTheme.sizeExtraSmall
                            font.family: AppTheme.fontMono
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 0

                        // One bar per cell
                        Repeater {
                            model: 16

                            CellBar {
                                readonly property real cellVoltage: index < debugRoot.cellVoltages.length ? debugRoot.cellVoltages[index] : NaN

                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                cellIndex: index
                                barColor: debugRoot.cellBarColor(cellVoltage)
                                ratio: debugRoot.cellRatio(cellVoltage)
                            }
                        }
                    }
                }
            }

            // Min, max and delta
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                CellStat {
                    Layout.fillWidth: true
                    labelText: "MIN CELL"
                    valueText: debugRoot.volts(debugRoot.cellMin)
                    unitText: "V"
                    valueColor: debugRoot.cellColor(debugRoot.cellMin)
                }
                CellStat {
                    Layout.fillWidth: true
                    labelText: "MAX CELL"
                    valueText: debugRoot.volts(debugRoot.cellMax)
                    unitText: "V"
                    valueColor: debugRoot.cellColor(debugRoot.cellMax)
                }
                CellStat {
                    Layout.fillWidth: true
                    labelText: "DELTA"
                    valueText: isNaN(
                                   debugRoot.cellDeltaMv) ? "--" : Number(
                                                                debugRoot.cellDeltaMv).toFixed(
                                                                0)
                    unitText: "mV"
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
            Layout.preferredWidth: 220
            Layout.fillHeight: true
            spacing: 20

            Text {
                text: "Temperature"
                color: AppTheme.colorNeut100
                font.pixelSize: AppTheme.sizeSmall
                font.bold: true
            }

            // Temperature bars
            Repeater {
                model: 3

                TempBar {
                    required property int index

                    Layout.fillWidth: true
                    labelText: "Battery temp " + (index + 1)
                    value: Number(
                               UsbDashReceiver.dashData.packTemperatures[index])
                    warnAt: 45
                    alertAt: 55
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

    Rectangle {
        anchors.fill: content
        visible: UsbDashReceiver.isConnected
                 && !UsbDashReceiver.dashData.isBmsOk
        color: Qt.rgba(0, 0, 0, 0.75)

        Text {
            anchors.centerIn: parent
            text: "BMS LOST"
            color: AppTheme.colorAlert
            font.pixelSize: AppTheme.sizeHeader
            font.bold: true
        }
    }
}
