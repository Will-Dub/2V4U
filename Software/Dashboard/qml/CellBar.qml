import QtQuick
import Dashboard

Item {
    property int cellIndex: 0
    property string voltageText: "--"

    property color barColor: AppTheme.colorNeut30
    property real ratio: 0

    Text {
        id: topText
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        horizontalAlignment: Text.AlignHCenter
        text: voltageText
        color: barColor
        font.pixelSize: 12
        font.family: AppTheme.fontMono
        font.bold: true
    }

    Rectangle {
        id: track
        anchors.top: topText.bottom
        anchors.topMargin: 6
        anchors.bottom: bottomText.top
        anchors.bottomMargin: 6
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 4
        anchors.rightMargin: 4
        radius: 6
        color: AppTheme.colorNeut30

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: track.height * ratio
            radius: 6
            color: barColor

            Behavior on height {
                NumberAnimation {
                    duration: 250
                }
            }
            Behavior on color {
                ColorAnimation {
                    duration: 250
                }
            }
        }

        // Pack average line
        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            y: track.height / 2
            height: 1
            color: AppTheme.colorNeut100
            opacity: 0.6
        }
    }

    Text {
        id: bottomText
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        horizontalAlignment: Text.AlignHCenter
        text: cellIndex + 1
        color: AppTheme.colorNeut70
        font.pixelSize: AppTheme.sizeExtraSmall
        font.family: AppTheme.fontMono
    }
}
