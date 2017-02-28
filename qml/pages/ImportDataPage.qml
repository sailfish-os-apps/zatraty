import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: importResult

    function doImport() {
        DataBase.importation = true
    }

    Timer {
        id: importTimer
        interval: 1000
        repeat: true
        running: DataBase.importation

        onRunningChanged: {
            if (!running) {
                importResultLabel.text = DataBase.error
                if (importResultLabel.text === "")
                    importResultLabel.text = qsTr("Done")
            }
        }
    }

    PageHeader {
        id: importResultHeader
        title: qsTr("Import data")
    }

    Label {
        id: importResultLabel
        anchors {
            top: importResultHeader.bottom
            topMargin: Theme.paddingLarge
            horizontalCenter: parent.horizontalCenter
        }
        font.pixelSize: Theme.fontSizeExtraLarge
        color: Theme.highlightColor
        wrapMode: Text.WordWrap
        horizontalAlignment: TextInput.AlignHCenter
        width: parent.width*0.8
        text: qsTr("Import in progress...")
    }

    BusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: importTimer.running
    }
}
