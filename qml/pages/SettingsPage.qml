import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: settingsPage

    property var options: Settings.currencies

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column
            width: settingsPage.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Settings")
            }

            ComboBox {
                id: currencyComboBox
                width: parent.width * 0.75
                anchors {horizontalCenter: parent.horizontalCenter}
                label: qsTr("Currency: ")
                currentIndex: Settings.value("currency", 0)

                menu: ContextMenu {
                    Repeater {
                        model: options
                        MenuItem { text: options[index]}
                    }
                }

                onCurrentIndexChanged: Settings.setValue("currency", currentIndex)
            }

            Button {
                id: addCustomCurrency
                width: parent.width * 0.7
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Add New Currency")

                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../components/NewCurrencyDialog.qml"))
                }
            }

            Button {
                id: resetButton
                width: parent.width * 0.7
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Reset Database")

                onClicked: pageStack.push(Qt.resolvedUrl("../components/ResetDatabaseDialog.qml"))
            }

            Button {
                id: importButton
                width: parent.width * 0.7
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Import Data")

                onClicked: pageStack.push(Qt.resolvedUrl("../components/ImportDataDialog.qml"))
            }

            Button {
                id: contactLabel
                width: parent.width * 0.7
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("About")

                onClicked: pageStack.push(Qt.resolvedUrl("ContactsPage.qml"))
            }
        }
    }
}
