import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: page

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: page.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("About %1").arg(Settings.appName)
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: "/usr/share/icons/hicolor/86x86/apps/harbour-zatraty.png"
                width: parent.width * 0.2
                height: width
            }

            Label {
                text: qsTr("Version %1").arg(Settings.appVersion)
                font.pixelSize: Theme.fontSizeTiny
                color: Theme.secondaryHighlightColor
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                text: qsTr("A simple app to manage your expenses.")
                color: Theme.secondaryHighlightColor
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                horizontalAlignment: TextInput.AlignHCenter
                width: parent.width * 0.8
            }

            Button {
                text: qsTr("Source code")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: Theme.paddingLarge
                onClicked: Qt.openUrlExternally("https://github.com/ckazzku/zatraty")
            }
        }
    }
}
