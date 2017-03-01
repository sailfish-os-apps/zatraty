import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Dialog {
    id: dialog
    acceptDestination: Qt.resolvedUrl("../pages/ImportDataPage.qml")
    acceptDestinationAction: PageStackAction.Replace
    canAccept: true

    DialogHeader {
        id: header
        title: qsTr("Import data")
        acceptText: qsTr("Import")
    }

    Label {
        id: warningLabel
        anchors {
            top: header.bottom
            topMargin: Theme.paddingLarge
            horizontalCenter: parent.horizontalCenter
        }
        font.pixelSize: Theme.fontSizeExtraLarge
        color: Theme.highlightColor
        wrapMode: Text.WordWrap
        horizontalAlignment: TextInput.AlignHCenter
        width: parent.width*0.8
        text: qsTr("Will be imported data from the old Expense app")
    }

    onAccepted: {
        acceptDestinationInstance.doImport()
    }

    Component.onCompleted: {
        var imported = Settings.value("expense_data_imported", false)
        if (imported) {
            dialog.canAccept = false
            warningLabel.text = qsTr("Data already imported")
        }
    }
}
