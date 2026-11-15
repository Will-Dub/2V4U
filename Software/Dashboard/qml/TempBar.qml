import QtQuick
import QtQuick.Layouts
import Dashboard

Column {
    property string labelText
    property real value: NaN
    property real warnAt: 80
    property real alertAt: 90
    property real scaleMax: 120

    readonly property color barColor: {
        if (isNaN(value))
            return AppTheme.colorNeut60
        if (value >= alertAt)
            return AppTheme.colorAlert
        if (value >= warnAt)
            return AppTheme.colorWarn
        return AppTheme.colorPrm
    }

    width: parent.width
    spacing: 6

    RowLayout {
        width: parent.width

        Text {
            text: labelText
            color: AppTheme.colorNeut70
            font.pixelSize: AppTheme.sizeExtraSmall
            font.family: AppTheme.fontMono
            font.letterSpacing: 2
            Layout.fillWidth: true
        }
        Text {
            text: isNaN(value) ? "--" : Number(value).toFixed(1) + " °C"
            color: barColor
            font.pixelSize: AppTheme.sizeSmall
            font.family: AppTheme.fontMono
            font.bold: true
        }
    }

    Rectangle {
        id: track
        width: parent.width
        height: 10
        radius: 5
        color: AppTheme.colorNeut30

        Rectangle {
            width: isNaN(value) ? 0 : track.width * Math.min(
                                      Math.max(value / scaleMax, 0), 1)
            height: parent.height
            radius: 5
            color: barColor

            Behavior on width {
                NumberAnimation {
                    duration: 250
                }
            }
        }

        // Tick at the warning level
        Rectangle {
            x: track.width * warnAt / scaleMax
            width: 2
            height: parent.height + 6
            y: -3
            color: AppTheme.colorWarn
        }
        // Tick at the alert level
        Rectangle {
            x: track.width * alertAt / scaleMax
            width: 2
            height: parent.height + 6
            y: -3
            color: AppTheme.colorAlert
        }
    }
}
