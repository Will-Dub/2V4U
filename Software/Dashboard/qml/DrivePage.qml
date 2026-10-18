import QtQuick
import Dashboard

Item {
    id: root

    Rectangle {
        x: 20; y: 20; width: 200; height: 60
        radius: 5
        color: UsbDashReceiver.dashData.stateStr === "DRIVE" ? AppTheme.colorActn :
               UsbDashReceiver.dashData.stateStr === "FAULT" ? AppTheme.colorAlert :
               AppTheme.colorNeut20

        Text {
            anchors.centerIn: parent
            text: UsbDashReceiver.dashData.stateStr
            color: AppTheme.colorNeut0
            font.pixelSize: AppTheme.sizeNormal
            font.bold: true
        }
    }

    // RPM Text
    Text {
        anchors.centerIn: parent
        text: UsbDashReceiver.dashData.rpm
        color: AppTheme.colorNeut100
        font.pixelSize: AppTheme.sizeMassive
        font.family: AppTheme.fontMono
    }
}
