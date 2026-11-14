import QtQuick

Rectangle {
    id: root

    property string iconSource: ""
    property string titleText: "TITLE"
    property string detailText: "Detail description"
    signal clicked()

    width: parent.width
    implicitHeight: 64
    radius: 8
    color: hoverHandler.hovered ? AppTheme.colorBtnHover : AppTheme.colorBtnBg
    border.color: hoverHandler.hovered ? AppTheme.colorPrm : AppTheme.colorCardBorder
    border.width: 2

    Behavior on color { ColorAnimation { duration: 100 } }
    Behavior on border.color { ColorAnimation { duration: 100 } }

    Row {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 16

        // Image
        Rectangle {
            width: 40
            height: 40
            radius: 6
            color: AppTheme.colorNeut0
            anchors.verticalCenter: parent.verticalCenter

            Image {
                id: iconImg
                width: 24
                height: 24
                source: root.iconSource
                sourceSize.width: 24
                sourceSize.height: 24
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: parent
            }
        }

        // Text
        Column {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2

            Text {
                text: titleText
                color: AppTheme.colorNeut100
                font.pixelSize: 14
                font.bold: true
                font.family: AppTheme.fontMono
            }

            Text {
                text: detailText
                color: AppTheme.colorNeut80
                font.pixelSize: 12
                font.family: AppTheme.fontMono
            }
        }
    }

    HoverHandler {
        id: hoverHandler
    }

    TapHandler {
        id: tapHandler
        onTapped: root.clicked()
    }
}