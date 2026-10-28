import QtQuick
import Dashboard

Item {
    id: root
    width: 500
    height: 350

    required property real rpmValue
    property real maxRpm: 9000
    property real redlineRpm: 8000

    readonly property int segments: 45
    readonly property real startAngle: 160
    readonly property real sweepAngle: 220
    readonly property real gaugeSize: height - 20

    property real displayRpm: rpmValue
    Behavior on displayRpm {
        NumberAnimation {
            duration: 200
            easing.type: Easing.OutCubic
        }
    }

    readonly property bool inRedline: rpmValue >= redlineRpm
    readonly property real load: rpmValue / maxRpm

    function fmt(n) {
        return Math.round(n).toLocaleString(Qt.locale(), 'f', 0)
    }

    Repeater {
        model: root.segments

        Item {
            id: seg
            width: root.gaugeSize
            height: root.gaugeSize
            anchors.centerIn: parent
            rotation: root.startAngle + index * root.sweepAngle / (root.segments - 1)

            readonly property real segRpm: index * root.maxRpm / root.segments
            readonly property bool lit: segRpm < root.displayRpm
            readonly property bool redzone: segRpm >= root.redlineRpm

            Rectangle {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: index % 5 === 0 ? 20 : 12
                height: 8
                radius: height / 2
                antialiasing: true

                color: seg.lit ? (seg.redzone ? AppTheme.colorAlert : AppTheme.colorActn) : (seg.redzone ? Qt.alpha(AppTheme.colorAlert, 0.3) : AppTheme.colorNeut40)

                scale: seg.lit ? 1.0 : 0.85
                Behavior on color {
                    ColorAnimation {
                        duration: 120
                    }
                }
                Behavior on scale {
                    NumberAnimation {
                        duration: 120
                    }
                }
            }
        }
    }

    Repeater {
        model: 10

        Item {
            width: root.gaugeSize
            height: root.gaugeSize
            anchors.centerIn: parent
            rotation: root.startAngle + index * root.sweepAngle / 9

            Text {
                anchors.right: parent.right
                anchors.rightMargin: 36
                anchors.verticalCenter: parent.verticalCenter
                rotation: -parent.rotation

                text: index
                color: index * 1000 >= root.redlineRpm ? AppTheme.colorAlert : AppTheme.colorNeut70
                font.family: AppTheme.fontMono
                font.pixelSize: AppTheme.sizeSmall
                font.bold: true
            }
        }
    }

    Column {
        id: readout
        anchors.centerIn: parent
        spacing: -10
        property real pulse: 1
        opacity: root.inRedline ? pulse : 1

        SequentialAnimation on pulse {
            running: root.inRedline
            loops: Animation.Infinite
            NumberAnimation {
                to: 0.55
                duration: 180
            }
            NumberAnimation {
                to: 1.0
                duration: 180
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.fmt(root.rpmValue)
            color: root.inRedline ? AppTheme.colorAlert : AppTheme.colorNeut100
            font.family: "Oswald"
            font.pixelSize: 70
            font.bold: true
            Behavior on color {
                ColorAnimation {
                    duration: 150
                }
            }
        }
    }
}
