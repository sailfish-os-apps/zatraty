import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: currenciesPage
    property var currencies: Settings.currencies

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: currencies

        header: PageHeader {
            title: qsTr("Currencies")
        }

        delegate: ListItem {
            id: delegate
            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Delete")
                    onClicked: {
                        remorseAction(qsTr("Deleting currency"),
                                             function() {
                                                 Settings.delCurrency(index)
                                             })
                    }
                }
            }

            Label {
                x: Theme.paddingLarge
                text: currencies[index]
                anchors.verticalCenter: parent.verticalCenter
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            onClicked: {
                Settings.setValue("currency", index)
                pageStack.pop()
            }
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Add currency")
                onClicked: pageStack.push(Qt.resolvedUrl("../components/NewCurrencyDialog.qml"))
            }
        }

        VerticalScrollDecorator {}
    }
}
