import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: hystoryPage

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

        delegate: ListItem {
            id: delegate
            height: 150

            Label {
                id: valueLabel
                x: 50
                text: qsTr("%1 %2", "1 is amount and 2 is currency")
                                                        .arg(percentIndicator.value)
                                                        .arg(Settings.currency)
            }

            ProgressBar {
                id: percentIndicator
                width: parent.width
                minimumValue: 0
                maximumValue: ExpenseModel.totalAmount()
                value: ExpenseModel.totalMonthAmount(date)
                label: Qt.formatDate(date, "MMMM yyyy")
            }

            onClicked: {
                pageStack.push(Qt.resolvedUrl("MonthSummaryPage.qml"), { "period": date })
            }
        }
        VerticalScrollDecorator {}
    }
}
