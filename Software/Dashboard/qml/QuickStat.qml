import QtQuick
import Dashboard

Column {
    property string labelText
    property string valueText
    property string unitText

    width: parent.width
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
        anchors.baselineOffset: 0
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

    Item {
        width: 1
        height: 10
    }

    Rectangle {
        width: parent.width
        height: 2
        color: AppTheme.colorActn
        opacity: 0.5
    }
}
