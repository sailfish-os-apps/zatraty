import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: importResult

    function doImport() {
        if (!DataBase.importDataFromOldExpenseApp())
            importResultLabel.text = DataBase.error
        else
            importResultLabel.text = qsTr("Done.")
        busyIndicator.visible = false
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
        text: "..."
    }

    BusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        visible: true
        running: visible
    }
}
