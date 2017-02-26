import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Dialog {
    property Expense expense
    property real amount: expense ? expense.amount : 0
    property date date: expense ? expense.date : new Date()
    property string description: expense ? expense.description : ""

    DialogHeader {
        id: header
        title: qsTr("Delete Item")
        acceptText: qsTr("Delete")
    }

    Row {
        id: dateAmountRow
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: header.bottom
            topMargin: Theme.paddingLarge*2
        }
        spacing: Theme.paddingLarge

        Label {
            id: dateLabel
            text: Qt.formatDate(date, Qt.DefaultLocaleShortDate)
            color: Theme.highlightColor
        }

        Label {
            id: amountLabel
            text: qsTr("amount: %1 %2", "1 is amount and 2 is currency")
                                                            .arg(amount)
                                                            .arg(Settings.currency)
            color: Theme.highlightColor
        }
    }

    Label {
        id: descLabel
        text: description
        color: Theme.highlightColor

        anchors {
            top: dateAmountRow.bottom
            topMargin: Theme.paddingSmall
            horizontalCenter: parent.horizontalCenter
        }
    }
}
