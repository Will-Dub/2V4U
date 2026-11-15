import QtQuick
import Dashboard

Item {
    id: root

    // Append incoming log lines
    Connections {
        target: UsbDashReceiver
        function onDebugLogReceived(message) {
            consoleModel.append({
                                    "logText": message
                                })
            // Keep the last 100 lines
            if (consoleModel.count > 100) {
                consoleModel.remove(0, 1)
            }
            // Scroll to the newest line
            terminalView.positionViewAtEnd()
        }
    }

    // Log lines
    ListModel {
        id: consoleModel
    }

    // Terminal
    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        Rectangle {
            width: parent.width
            height: 380
            color: AppTheme.colorNeut10
            border.color: AppTheme.colorNeut20

            // Log view
            ListView {
                id: terminalView
                anchors.fill: parent
                anchors.margins: 10
                model: consoleModel
                clip: true

                delegate: Text {
                    text: logText
                    color: AppTheme.colorNeut80
                    font.family: AppTheme.fontMono
                    font.pixelSize: 14
                }
            }
        }
    }
}
