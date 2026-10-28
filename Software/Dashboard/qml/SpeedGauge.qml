import QtQuick
import QtQuick.Shapes

Item {
    id: root

    width: 350
    height: 350

    required property real speedValue
    property real maxSpeed: 100

    property real startAngleOffset: 145
    property real totalSweepAngle: 250

    property color colorTrack: AppTheme.colorNeut30
    property color colorFill: AppTheme.colorActn
    property color colorText: AppTheme.colorNeut100
    property color colorDim: AppTheme.colorNeut50

    // Ticks
    Item {
        anchors.fill: parent

        Repeater {
            model: 25
            Item {
                width: root.width
                height: root.height

                rotation: root.startAngleOffset + (index * (root.totalSweepAngle / 24))

                Rectangle {
                    width: index % 6 === 0 ? 12 : 6
                    height: index % 6 === 0 ? 2 : 1
                    color: root.colorDim
                    antialiasing: true

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right

                    anchors.rightMargin: 10
                }
            }
        }
    }

    // Background arc
    Shape {
        anchors.fill: parent
        antialiasing: true

        ShapePath {
            fillColor: "transparent"
            strokeColor: root.colorTrack
            strokeWidth: 8
            capStyle: ShapePath.FlatCap

            PathAngleArc {
                centerX: root.width / 2
                centerY: root.height / 2
                radiusX: root.width / 2 - 40
                radiusY: root.height / 2 - 40
                startAngle: root.startAngleOffset
                sweepAngle: root.totalSweepAngle
            }
        }
    }

    // Active arc
    Shape {
        anchors.fill: parent
        antialiasing: true

        ShapePath {
            fillColor: "transparent"
            strokeColor: root.colorFill
            strokeWidth: 8
            capStyle: ShapePath.FlatCap

            PathAngleArc {
                centerX: root.width / 2
                centerY: root.height / 2
                radiusX: root.width / 2 - 40
                radiusY: root.height / 2 - 40
                startAngle: root.startAngleOffset

                sweepAngle: Math.min(
                                (root.speedValue / root.maxSpeed) * root.totalSweepAngle,
                                root.totalSweepAngle)
            }
        }
    }

    // Central text
    Column {
        anchors.centerIn: parent
        spacing: 0

        Text {
            text: Math.round(root.speedValue).toString()
            color: root.colorText

            font.family: "Oswald"
            font.pixelSize: 90
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            topPadding: -15
            bottomPadding: -15
        }

        Text {
            text: "KM/H"
            color: root.colorFill
            font.pixelSize: 16
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
