import QtQuick
import Dashboard

Rectangle {
    property string labelText
    property string valueText
    property string unitText

    width: parent.width
    implicitHeight: contentCol.implicitHeight + 16

    radius: 8
    color: AppTheme.colorCardBg

    Column {
        id: contentCol
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 8
        spacing: 0

        Text {
            text: labelText
            color: AppTheme.colorNeut70
            font.pixelSize: AppTheme.sizeExtraSmall
            font.family: AppTheme.fontMono
            font.letterSpacing: 2
        }

        Row {
            spacing: 8
            Text {
                id: valTxt
                text: valueText
                color: AppTheme.colorNeut100
                font.pixelSize: 40
                font.family: AppTheme.fontMono
                font.bold: true
            }
            Text {
                text: unitText
                color: AppTheme.colorActn
                font.pixelSize: AppTheme.sizeExtraSmall
                font.family: AppTheme.fontMono
                anchors.baseline: valTxt.baseline
            }
        }
    }
}
