import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Dialog {
    id: dialog
    canAccept: false
    property string category
    property real amount
    property string desc

    Component.onCompleted: {
        appWindow.quickAddOpen = true
        if (category !== "") {
            categoryLabel.text = category
            categoryChooseButton.enabled = false
        }
        amountField.focus = true
    }

    DialogHeader {
        id: header
        title: qsTr("New Entry")
        acceptText: qsTr("Add")
    }

    TextField {
        id: amountField
        width: parent.width
        anchors {
            top: header.bottom
            topMargin: Theme.paddingLarge
        }
        placeholderText: qsTr("Amount", "placeholder for amount")
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        onTextChanged: {
            dialog.canAccept = (text !== "")
        }
    }

    TextField {
        id: descField
        width: parent.width
        anchors {
            top: amountField.bottom
            topMargin: Theme.paddingLarge
        }
        placeholderText: qsTr("Desc", "placeholder for description")
    }

    Button {
        id: categoryChooseButton
        text: CategoryModel.mostUsed().name
        width: parent.width - Theme.paddingLarge
        anchors {
            top: descField.bottom
            topMargin: Theme.paddingLarge
            horizontalCenter: parent.horizontalCenter
        }

        onClicked: categoriesDrawer.open = !categoriesDrawer.open
    }

    Drawer {
        id: categoriesDrawer
        open: false
        width: parent.width
        height: dialog.height + 4 * Theme.paddingLarge -
             ( categoryChooseButton.height + amountField.height + descField.height)
        anchors {
            top: categoryChooseButton.bottom
            topMargin: Theme.paddingLarge
        }

        background: SilicaListView {
            id: categoryListView
            anchors.fill: parent
            model: CategoryListModel {
                id: categoryListModel
            }

            delegate: BackgroundItem {
                id: delegate

                Label {
                    x: Theme.paddingLarge
                    text: name
                    anchors.verticalCenter: parent.verticalCenter
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                }
                onClicked: {
                    categoryChooseButton.text = name
                    categoriesDrawer.open = false
                }
            }
        }
    }

    onAccepted: {
        category = categoryChooseButton.text
        amount = parseFloat(amountField.text)
        desc = descField.text
    }

    onDone: {
        appWindow.quickAddOpen = false
    }
}
