import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0
import "../helpers"

Page {
    id: monthSummaryPage

    property date period

    SilicaListView {
        id: listView
        model: ExpenseListModel {
            id: monthExpensesModel
            dateFilter: Qt.formatDate(period, "yyyy.MM")
            reverse: true
        }
        anchors.fill: parent
        header: PageHeader {
            title: LocaleExt.formatDate(period, "MMMM yyyy")
        }

        section {
            property: 'date'

            delegate: SectionHeader {
                text: Qt.formatDate(section, "d MMMM")
                height: Theme.itemSizeExtraSmall
            }
        }

        delegate: ListItem {
            id: delegate
            width: parent.width
            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Delete")
                    onClicked: {
                        var expense = monthExpensesModel.get(index)
                        var dialog = pageStack.push(Qt.resolvedUrl("../components/DeleteEntryDialog.qml"),
                                                    { "expense": expense })
                        dialog.accepted.connect(function() {
                            monthExpensesModel.remove(index)
                        })
                    }
                }
            }

            Label {
                id: amountLabel
                text: qsTr("%1 %2 in %3", "1 is amount, 2 is currency and 3 is the category")
                                                           .arg(amount)
                                                           .arg(Settings.currency)
                                                           .arg(category)
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 2 * Theme.paddingLarge
                color: Theme.highlightColor
                truncationMode: TruncationMode.Elide
                elide: Text.ElideRight
            }

            Label {
                id: descLabel
                text: (description !== undefined) ? description : ""
                color: Theme.primaryColor
                x: Theme.paddingLarge * 2
                font.pixelSize: Theme.fontSizeTiny
                anchors {
                    top: amountLabel.bottom
                    bottomMargin: Theme.paddingMedium
                }
                width: parent.width - x - Theme.paddingLarge
                truncationMode: TruncationMode.Elide
                elide: Text.ElideRight
            }
        }

        VerticalScrollDecorator {}
    }
}
