import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: page

    property Category category
    property real total
    property real totalThisMonth

    function refresh() {
        percentIndicator.value = 0
        animationTimer.running = true
        total = ExpenseModel.totalAmount()
        totalThisMonth = ExpenseModel.totalMonthAmount(new Date(), category)
    }

    onStatusChanged: {
        if (page.status === PageStatus.Activating)
            refresh()
    }

    Timer {
        id: animationTimer
        interval: 40
        repeat: true
        running: false
        onTriggered: {
            var portion = total / interval;
            if (percentIndicator.value < totalThisMonth)
                percentIndicator.value += portion;
            else stop()
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {

            MenuItem {
                text: qsTr("Add Entry")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../components/NewEntryDialog.qml"),
                                                    { "category": category.name })
                    dialog.accepted.connect(function() {
                        expenseListModel.add(dialog.category, dialog.amount, dialog.desc)
                    })
                }
            }
        }

        Column {
            id: column
            width: page.width
            spacing: Theme.paddingSmall

            PageHeader {
                title: category.name
            }

            Label {
                id: moneyLabel
                anchors.horizontalCenter: parent.horizontalCenter
                color: Theme.secondaryHighlightColor
                width: parent.width
                height: contentHeight
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: Theme.fontSizeExtraLarge * 3
                fontSizeMode: Text.HorizontalFit
                text: qsTr("%1 %2", "1 is amount and 2 is currency")
                                                        .arg(Math.round(totalThisMonth))
                                                        .arg(Settings.currency)
            }

            Label {
                anchors {horizontalCenter: parent.horizontalCenter}
                text: qsTr("in %1 this month", "subtitle of the amount spent in the CategoryView")
                                                              .arg(category.name)
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeLarge
            }

        }

        ProgressBar {
            id: percentIndicator
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: column.bottom
                topMargin: Theme.paddingLarge
            }
            width: parent.width
            minimumValue: 0
            maximumValue: total
            value: 0
            valueText: qsTr("%1 %").arg(Math.round(value / maximumValue * 100))
            label: qsTr("of the total", "subtitle of the percentagebar")
        }

        Label {
            id: insertionsLabel
            x: Theme.paddingLarge
            anchors {
                top: percentIndicator.bottom
                topMargin: Theme.paddingLarge*1.2
            }
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeLarge
            text: qsTr("This month:")
        }

        SilicaListView {
            id: expensesListView
            model: ExpenseListModel {
                id: expenseListModel
                categoryFilter: category
                dateFilter: Qt.formatDate(new Date(), "yyyy.MM")
                reverse: true
            }
            anchors {
                top: insertionsLabel.bottom
                topMargin: Theme.paddingLarge
            }
            clip:true
            width: parent.width
            height: page.height - column.height - percentIndicator.height
                    - insertionsLabel.height - Theme.paddingLarge*2*1.2 - Theme.paddingSmall

            delegate: ListItem {
                id: delegat
                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("Delete")

                        onClicked: {
                            var expense = expenseListModel.get(index)
                            var dialog = pageStack.push(Qt.resolvedUrl("../components/DeleteEntryDialog.qml"),
                                                        { "expense": expense })
                            dialog.accepted.connect(function() {
                                expenseListModel.remove(index)
                            })
                        }
                    }
                }

                Row {
                    id: dateAmountRow
                    x: Theme.paddingLarge*2
                    spacing: Theme.paddingLarge

                    Label {
                        id: dateLabel
                        text: Qt.formatDate(date, Qt.DefaultLocaleShortDate)
                        color: Theme.primaryColor
                    }

                    Label {
                        id: amountLabel
                        text: qsTr("%1 %2", "1 is amount and 2 is currency")
                                                            .arg(amount)
                                                            .arg(Settings.currency)
                        color: Theme.primaryColor
                    }
                }

                Label {
                    id: descLabel
                    text: description
                    visible: (description !== undefined)
                    color: Theme.highlightColor
                    x: Theme.paddingLarge*2
                    anchors {
                        top: dateAmountRow.bottom
                        topMargin: Theme.paddingSmall
                    }
                }
            }
            VerticalScrollDecorator {}
        }
    }
}
