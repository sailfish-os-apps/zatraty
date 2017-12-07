import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: page

    property Category category
    property real current
    property real total
    property bool forCurrentMonth: true

    function refresh() {
        if (forCurrentMonth) {
            current = ExpenseModel.totalMonthAmount(new Date(), category)
            total = ExpenseModel.totalAmount(category)
        } else {
            current = ExpenseModel.totalAmount(category)
            total = ExpenseModel.totalAmount()
        }
        animationTimer.running = true
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
        property real portion: 2
        property int precision: 100
        property real target: total > 0 ? Math.round((100.0 * current) / total * precision) / precision : 0
        property bool reverse: percentIndicator.value > target
        onTriggered: {
            if (Math.abs(percentIndicator.value - target) > portion) {
                percentIndicator.value += (reverse ? -1 : 1) * portion
            } else {
                percentIndicator.value = target
                stop()
            }
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: forCurrentMonth ? qsTr("For all time") : qsTr("For current month")
                onClicked: {
                    forCurrentMonth = !forCurrentMonth
                    refresh()
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
                text: "%1 %2".arg(Math.round(current * 100) / 100)
                             .arg(Settings.currency)
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: forCurrentMonth ? qsTr("for this month") : qsTr("for all time")
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
            maximumValue: 100
            value: 0
            valueText: "%1 %".arg(value)
            label: forCurrentMonth ? qsTr("of the category total") :
                                     qsTr("of the total")
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
            text: qsTr("Expenses:")
            visible: current > 0
        }

        SilicaListView {
            id: expensesListView
            model: ExpenseListModel {
                id: expenseListModel
                categoryFilter: category
                dateFilter: forCurrentMonth ? Qt.formatDate(new Date(), "yyyy.MM") : ""
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
                        text: "%1 %2".arg(amount)
                                     .arg(Settings.currency)
                        color: Theme.primaryColor
                    }
                }

                Label {
                    id: descLabel
                    text: description
                    visible: (description !== undefined)
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeTiny
                    x: Theme.paddingLarge * 2
                    width: parent.width - x - Theme.paddingLarge
                    anchors {
                        top: dateAmountRow.bottom
                        topMargin: Theme.paddingSmall
                    }
                    truncationMode: TruncationMode.Elide
                    elide: Text.ElideRight
                }
            }
            VerticalScrollDecorator {}
        }
    }
}
