import QtQuick
import QtQuick.Controls
import Dashboard

Window {
    width: 800; height: 480; visible: true; color: AppTheme.colorNeut0
    maximumWidth: 800; maximumHeight: 480; minimumWidth: 800; minimumHeight: 480

    SwipeView {
        id: dashView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: navBar.top
        currentIndex: 0
        interactive: false

        DrivePage { }
        DebugPage { }
    }

    Rectangle {
        id: navBar
        width: parent.width
        height: 60
        anchors.bottom: parent.bottom
        color: AppTheme.colorNeut20

        // Page Navigation
        Row {
            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.verticalCenter: navBar.verticalCenter
            spacing: 8

            PageElement {
                iconText: ""
                labelText: "Drive"
                isActive: dashView.currentIndex === 0
                onClicked: dashView.currentIndex = 0
            }

            PageElement {
                iconText: ""
                labelText: "Debug"
                isActive: dashView.currentIndex === 1
                onClicked: dashView.currentIndex = 1
            }
        }

        // Connection Status
        Row {
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            spacing: 12

            Rectangle {
                width: 14; height: 14; radius: 7
                anchors.verticalCenter: parent.verticalCenter
                color: UsbDashReceiver.dashData.stateStr === "FAULT" ? AppTheme.colorAlert : AppTheme.colorActn

                border.color: Qt.darker(color, 1.5)
                border.width: 1
            }

            Text {
                text: "VCU ONLINE"
                color: AppTheme.colorNeut100
                font.pixelSize: AppTheme.sizeSmall
                font.family: AppTheme.fontMono
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
