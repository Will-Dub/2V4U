import QtQuick
import QtQuick.Layouts
import Dashboard

Item {
    property string signalName
    property string sourceName
    property string valText
    property string unitText
    property string ageText
    property string stateText: "OK"
    property color stateColor: AppTheme.colorActn

    width: parent.width
    height: 50

    Rectangle {
        width: parent.width
        height: 1
        anchors.bottom: parent.bottom
        color: AppTheme.colorNeut30
    }

    RowLayout {
        anchors.fill: parent
        anchors.bottomMargin: 5

        Column {
            Layout.fillWidth: true
            Text {
                width: parent.width
                text: signalName
                color: AppTheme.colorNeut100
                font.pixelSize: AppTheme.sizeSmall
            }
            Text {
                width: parent.width
                text: sourceName
                color: AppTheme.colorNeut60
                font.pixelSize: AppTheme.sizeExtraSmall
                font.family: AppTheme.fontMono
            }
        }

        Item {
            Layout.preferredWidth: 120
            Layout.fillHeight: true

            Row {
                anchors.centerIn: parent
                spacing: 4
                Text {
                    id: rowVal
                    text: valText
                    color: AppTheme.colorNeut100
                    font.pixelSize: AppTheme.sizeSmall
                    font.family: AppTheme.fontMono
                    font.bold: true
                }
                Text {
                    text: unitText
                    color: AppTheme.colorNeut70
                    font.pixelSize: AppTheme.sizeExtraSmall
                    font.family: AppTheme.fontMono
                    anchors.baseline: rowVal.baseline
                }
            }
        }

        Text {
            Layout.preferredWidth: 80
            horizontalAlignment: Text.AlignHCenter
            text: ageText
            color: AppTheme.colorNeut70
            font.pixelSize: AppTheme.sizeExtraSmall
            font.family: AppTheme.fontMono
        }
        Text {
            Layout.preferredWidth: 40
            horizontalAlignment: Text.AlignHCenter
            text: stateText
            color: stateColor
            font.pixelSize: AppTheme.sizeExtraSmall
            font.family: AppTheme.fontMono
        }
    }
}
