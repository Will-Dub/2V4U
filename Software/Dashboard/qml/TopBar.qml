import QtQuick

Item {
    id: root
    property string title: ""

    anchors {
        top: parent.top
        left: parent.left
        right: parent.right
    }
    height: 60

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: AppTheme.colorNeut30
    }
    Text {
        anchors {
            left: parent.left
            leftMargin: 10
            verticalCenter: parent.verticalCenter
        }

        text: root.title
        color: AppTheme.colorNeut100
        font.pixelSize: AppTheme.sizeHeader
        font.bold: true
    }

    // Connection / state badge
    StatusBadge {
        anchors {
            right: parent.right
            rightMargin: 30
            verticalCenter: parent.verticalCenter
        }

        statusText: {
            if (!UsbDashReceiver.isConnected)
                return "OFFLINE"
            if (UsbDashReceiver.dashData.stateStr === "FAULT"
                    && UsbDashReceiver.dashData.faultStr !== "")
                return UsbDashReceiver.dashData.faultStr
            return UsbDashReceiver.dashData.stateStr
        }

        statusColor: {
            if (!UsbDashReceiver.isConnected)
                return AppTheme.colorAlert
            else if (UsbDashReceiver.dashData.stateStr === "FAULT")
                return AppTheme.colorAlert
            return AppTheme.colorActn
        }
    }
}
