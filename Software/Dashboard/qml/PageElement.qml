import QtQuick
import Dashboard

Rectangle {
    id: root
    width: 136
    height: 42
    radius: 6

    required property string iconText
    required property string labelText
    property bool isActive: false

    signal clicked()

    color: root.isActive ? AppTheme.colorActn : AppTheme.colorNeut40
    border.color: root.isActive ? "transparent" : AppTheme.colorNeut50
    border.width: 1

    Row {
        anchors.centerIn: parent
        spacing: 10

        Text {
            text: root.iconText
            font.pixelSize: 20
            anchors.verticalCenter: parent.verticalCenter
            opacity: root.isActive ? 1.0 : 0.4
        }

        Text {
            text: root.labelText
            color: root.isActive ? AppTheme.colorNeut10 : AppTheme.colorNeut90
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
