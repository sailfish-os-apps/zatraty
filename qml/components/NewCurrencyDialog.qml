import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Dialog {
    id: dialog
    canAccept: false

    DialogHeader {
        id: header
        title: qsTr("New currancy")
        acceptText: qsTr("Add")
    }

    TextField {
        id: currencyField
        width: parent.width
        anchors {
            top: header.bottom
            topMargin: Theme.paddingLarge
        }
        placeholderText: qsTr("Currency", "placeholder for currency")
        onTextChanged: {
            dialog.canAccept = (text !== "")
        }
    }

    onAccepted: {
        Settings.addCurrency(currencyField.text)
        appWindow.quickAddOpen = false
    }
}
