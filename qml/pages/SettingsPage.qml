import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: settingsPage

    PageHeader {
        id: header
        title: qsTr("Settings")
    }

    SilicaFlickable {
        anchors.fill: parent

        Column {
            id: column
            width: parent.width
            anchors.centerIn: parent
            spacing: Theme.paddingLarge

            Button {
                id: currencyButton
                width: parent.width * 0.7
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Currency: ") + Settings.currency

                onClicked: pageStack.push(Qt.resolvedUrl("CurrenciesPage.qml"))
            }

            Button {
                id: resetButton
                width: parent.width * 0.7
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Reset database")

                onClicked: pageStack.push(Qt.resolvedUrl("../components/ResetDatabaseDialog.qml"))
            }

            Button {
                id: importButton
                width: parent.width * 0.7
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Import data")

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
