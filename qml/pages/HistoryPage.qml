import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0
import "../helpers"
import "../components"

Page {
    id: historyPage

    SilicaListView {
        id: listView
        model: DateListModel {
            id: monthsModel
            group: DateListModel.ByMonth
            reverse: true
        }
        anchors.fill: parent

        header: PageHeader {
            title: qsTr("History")
        }

        delegate: ProgressItem {
            id: delegate
            x: Theme.paddingMedium
            height: Theme.itemSizeMedium
            width: parent.width - 2 * Theme.paddingMedium
            minimumValue: 0
            maximumValue: ExpenseModel.totalAmount()
            value: ExpenseModel.totalMonthAmount(date)

            Label {
                id: dateLabel
                anchors {
                    left: parent.left
                    leftMargin: Theme.paddingLarge
                    top: parent.top
                    topMargin: Theme.paddingSmall
                }
                text: LocaleExt.formatDate(date, "MMMM yyyy")
            }

            Label {
                id: valueLabel
                anchors {
                    right: parent.right
                    rightMargin: Theme.paddingLarge
                    bottom: parent.bottom
                    bottomMargin: Theme.paddingSmall
                }
                text: qsTr("%1 %2", "1 is amount and 2 is currency")
                                                        .arg(Math.round(value * 100) / 100)
                                                        .arg(Settings.currency)
            }

            onClicked: {
                pageStack.push(Qt.resolvedUrl("MonthSummaryPage.qml"), { "period": date })
            }
        }
        VerticalScrollDecorator {}
    }
}
