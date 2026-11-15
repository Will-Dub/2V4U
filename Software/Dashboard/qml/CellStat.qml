import QtQuick
import Dashboard

Rectangle {
    property string labelText
    property string valueText
    property string unitText
    property color valueColor: AppTheme.colorNeut100

    implicitHeight: 76
    radius: 8
    color: AppTheme.colorCardBg

    Column {
        anchors.centerIn: parent
        spacing: 2

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: labelText
            color: AppTheme.colorNeut70
            font.pixelSize: AppTheme.sizeExtraSmall
            font.family: AppTheme.fontMono
            font.letterSpacing: 2
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 6
            Text {
                id: statVal
                text: valueText
                color: valueColor
                font.pixelSize: 30
                font.family: AppTheme.fontMono
                font.bold: true
            }
            Text {
                text: unitText
                color: AppTheme.colorActn
                font.pixelSize: AppTheme.sizeExtraSmall
                font.family: AppTheme.fontMono
                anchors.baseline: statVal.baseline
            }
        }
    }
}
