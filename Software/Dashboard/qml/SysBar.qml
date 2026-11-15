import QtQuick
import QtQuick.Layouts
import Dashboard

Column {
    property string labelText
    property string pctText
    property real fillRatio: 0.5

    width: parent.width
    spacing: 4

    RowLayout {
        width: parent.width
        Text {
            text: labelText
            color: AppTheme.colorNeut70
            font.pixelSize: AppTheme.sizeExtraSmall
            font.family: AppTheme.fontMono
            Layout.fillWidth: true
        }
        Text {
            text: pctText
            color: AppTheme.colorNeut100
            font.pixelSize: AppTheme.sizeExtraSmall
            font.family: AppTheme.fontMono
        }
    }

    Rectangle {
        width: parent.width
        height: 4
        color: AppTheme.colorNeut30
        Rectangle {
            width: parent.width * fillRatio
            height: parent.height
            color: AppTheme.colorActn
        }
    }
}
