import QtQuick
import Dashboard

Row {
    property string label
    property bool isActive: false

    spacing: 8

    Rectangle {
        width: 12
        height: 12
        radius: 6
        color: isActive ? AppTheme.colorActn : AppTheme.colorNeut30
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        text: label
        color: isActive ? AppTheme.colorNeut100 : AppTheme.colorNeut70
        font.pixelSize: AppTheme.sizeExtraSmall
        font.family: AppTheme.fontMono
        anchors.verticalCenter: parent.verticalCenter
    }
}
