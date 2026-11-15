import QtQuick
import Dashboard

Column {
    required property string labelText
    required property string valueText
    property string valueUnit: ""
    property color valueColor: AppTheme.colorNeut100

    spacing: 4

    Text {
        text: labelText
        color: AppTheme.colorNeut70
        font.pixelSize: AppTheme.sizeExtraSmall
        font.bold: true
    }
    Row {
        spacing: 4

        Text {
            id: valText
            text: valueText
            color: valueColor
            font.pixelSize: AppTheme.sizeNormal
            font.family: AppTheme.fontMono
            font.bold: true
        }

        Text {
            text: valueUnit
            color: AppTheme.colorNeut70
            font.pixelSize: AppTheme.sizeExtraSmall
            font.bold: true

            anchors.baseline: valText.baseline
        }
    }
}
