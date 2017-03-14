import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: dialog
    canAccept: false
    property string name
    property bool isEdit: false

    Component.onCompleted: {
        dialog.isEdit = (name !== "")
    }

    DialogHeader {
        id: header
        title: qsTr("Category")
        acceptText: dialog.isEdit ? qsTr("Update") : qsTr("Add")
    }

    TextField {
        id: nameField
        width: parent.width
        anchors {
            top: header.bottom
            topMargin: Theme.paddingLarge
        }
        placeholderText: qsTr("Name", "placeholder for category name")
        text: dialog.isEdit ? dialog.name : ""
        onTextChanged: {
            dialog.canAccept = (text !== "")
            if (dialog.isEdit)
                dialog.canAccept &= (text !== dialog.name)
        }
    }

    onAccepted: {
        name = nameField.text
    }
}
