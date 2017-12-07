import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0
import "../JS/functions.js" as Helpers

Dialog {
    id: dialog
    canAccept: false
    property string category
    property date date
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
        title: qsTr("New entry")
        acceptText: qsTr("Add")
    }

    TextField {
        id: amountField
        width: parent.width
        anchors {
            top: header.bottom
            topMargin: Theme.paddingSmall
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
            topMargin: Theme.paddingSmall
        }
        placeholderText: qsTr("Desc", "placeholder for description")
    }

    Button {
        id: dateButton
        text: qsTr("Today")
        width: Theme.buttonWidthSmall
        anchors {
            top: descField.bottom
            left: parent.left
            leftMargin: Theme.paddingLarge
        }
        property date today: Helpers.resetTime(new Date())

        onClicked: {
            var dateDialog = pageStack.push(datePicker, {
                date: today
            })
            dateDialog.accepted.connect(function() {
                var newdate = Helpers.resetTime(dateDialog.date)
                if (newdate.getTime() <= today.getTime()) {
                    if (newdate.getTime() === today.getTime())
                        dateButton.text = qsTr("Today")
                    else
                        dateButton.text = dateDialog.dateText
                    dialog.date = newdate
                }
            })
        }

        Component {
            id: datePicker
            DatePickerDialog {}
        }
    }

    Button {
        id: categoryChooseButton
        width: Theme.buttonWidthSmall
        anchors {
            top: descField.bottom
            right: parent.right
            rightMargin: Theme.paddingLarge
        }

        Label {
            id: categoryLabel
            text: CategoryModel.mostUsed().name
            anchors {
                fill: parent
                leftMargin: Theme.paddingMedium
                rightMargin: Theme.paddingMedium
            }
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            truncationMode: TruncationMode.Elide
            elide: Text.ElideRight
        }

        onClicked: categoriesDrawer.open = !categoriesDrawer.open
    }

    Drawer {
        id: categoriesDrawer
        open: false
        width: parent.width
        height: dialog.height + 4 * Theme.paddingLarge -
             ( dateButton.height + amountField.height + descField.height )
        anchors {
            top: dateButton.bottom
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
                    width: parent.width - 2 * Theme.paddingLarge
                    truncationMode: TruncationMode.Elide
                    elide: Text.ElideRight
                }
                onClicked: {
                    categoryLabel.text = name
                    categoriesDrawer.open = false
                }
            }
        }
    }

    onAccepted: {
        category = categoryLabel.text
        amount = parseFloat(amountField.text.replace(',', '.'))
        desc = descField.text
    }

    onDone: {
        appWindow.quickAddOpen = false
    }
}
