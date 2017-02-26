import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: monthsPage

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

        delegate: BackgroundItem {
            id: delegate
            height: 160

            ProgressBar {
                id: percentIndicator
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                minimumValue: 0
                maximumValue: ExpenseModel.totalAmount()
                value: ExpenseModel.totalMonthAmount(date)
                valueText: qsTr("%1 %2", "1 is amount and 2 is currency")
                                                          .arg(value)
                                                          .arg(Settings.currency)
                label: Qt.formatDate(date, "MMMM yyyy")
            }

            onClicked: {
                pageStack.push(Qt.resolvedUrl("MonthSummaryPage.qml"), { "period": date })
            }
        }
        VerticalScrollDecorator {}
    }
}
