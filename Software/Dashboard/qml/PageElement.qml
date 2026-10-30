import QtQuick
import QtQuick.Effects
import Dashboard

Rectangle {
    id: root
    width: 136
    height: 42
    color: "transparent"

    required property string labelText
    property string iconSource: ""
    property bool isActive: false

    signal clicked

    Rectangle {
        width: parent.width
        height: 3
        anchors.top: parent.top

        color: root.isActive ? AppTheme.colorActn : "transparent"

        Behavior on color {
            ColorAnimation {
                duration: 150
            }
        }
    }

    Row {
        anchors.centerIn: parent
        spacing: 10

        Item {
            width: 16
            height: 16
            anchors.verticalCenter: parent.verticalCenter
            visible: iconSource !== ""

            Image {
                id: rawIcon
                source: root.iconSource
                anchors.fill: parent
                visible: false
            }

            MultiEffect {
                source: rawIcon
                anchors.fill: rawIcon
                colorization: 1.0
                colorizationColor: root.isActive ? AppTheme.colorActn : AppTheme.colorNeut70
            }
        }

        Text {
            text: root.labelText
            color: root.isActive ? AppTheme.colorNeut100 : AppTheme.colorNeut70
            font.pixelSize: AppTheme.sizeSmall
            font.bold: root.isActive
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
