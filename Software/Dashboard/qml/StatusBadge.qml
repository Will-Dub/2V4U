import QtQuick

Rectangle {
    id: root
    required property string statusText
    required property color statusColor

    width: badgeText.implicitWidth + 30
    height: 24
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
