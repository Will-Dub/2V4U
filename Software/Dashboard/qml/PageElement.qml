import QtQuick
import Dashboard

Rectangle {
    id: root
    width: 136
    height: 42
    color: "transparent"

    required property string iconText
    required property string labelText
    property bool isActive: false

    signal clicked()

    Rectangle {
        width: parent.width
        height: 3
        anchors.top: parent.top

        color: root.isActive ? AppTheme.colorActn : "transparent"

        Behavior on color { ColorAnimation { duration: 150 } }
    }

    Row {
        anchors.centerIn: parent
        spacing: 10

        Text {
            text: root.iconText
            font.pixelSize: 20
            color: root.isActive ? AppTheme.colorActn : AppTheme.colorNeut70
            anchors.verticalCenter: parent.verticalCenter
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
