import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: dialog
    canAccept: false
    property string name

    DialogHeader {
        id: header
        title: qsTr("New category")
        acceptText: qsTr("Add")
    }

    TextField {
        id: nameField
        width: parent.width
        anchors {
            top: header.bottom
            topMargin: Theme.paddingLarge
        }
        placeholderText: qsTr("Name", "placeholder for category name")
        onTextChanged: {
            dialog.canAccept = (text !== "")
        }
    }

    onAccepted: {
        name = nameField.text
    }
}
