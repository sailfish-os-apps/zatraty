import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: page

    property real totalThisMonth
    property real totalLastMonth
    property real total
    property string mostUsedCategory

    function refresh() {
        total = ExpenseModel.totalAmount()
        var date = new Date()
        totalThisMonth = ExpenseModel.totalMonthAmount(date)
        date.setMonth(date.getMonth() - 1)
        totalLastMonth = ExpenseModel.totalMonthAmount(date)

        var category = CategoryModel.mostUsed()
        if (category)
            mostUsedCategory = category.name
    }

    onStatusChanged: {
        if (page.status === PageStatus.Activating)
            refresh()
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: page.height

        PullDownMenu {
            MenuItem {
                text: qsTr("History")
                onClicked: pageStack.push(Qt.resolvedUrl("HistoryPage.qml"))
            }

            MenuItem {
                text: qsTr("Categories")
                onClicked: pageStack.push(Qt.resolvedUrl("CategoriesPage.qml"))
            }
            MenuItem {
                text: qsTr("Quick add")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../components/NewEntryDialog.qml"))
                    dialog.accepted.connect(function() {
                        ExpenseModel.add(dialog.category, dialog.amount, dialog.desc, dialog.date)
                    })
                }
            }
        }

        PushUpMenu {
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
        }

        PageHeader {
            id: header
            title: Settings.appName
        }

        Label {
            id: moneyLabel
            anchors {
                top: header.bottom
                topMargin: Theme.paddingLarge
                horizontalCenter: parent.horizontalCenter
            }
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeExtraLarge * 3
            fontSizeMode: Text.HorizontalFit
            text: "%1 %2".arg(Math.round(totalThisMonth))
                         .arg(Settings.currency)
        }

        Label {
            anchors {
                top: moneyLabel.bottom
                topMargin: Theme.paddingSmall
                horizontalCenter: parent.horizontalCenter
            }
            text: qsTr("spent this month", "subtitle of the amount spent in the MainView")
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            id: lastMonthLabel
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: totalLabel.top
                bottomMargin: Theme.paddingLarge
            }
            width: parent.width - 2 * Theme.paddingLarge
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeSmall
            text: qsTr("Last month: %1 %2", "1 is amount and 2 is currency")
                                            .arg(Math.round(totalLastMonth))
                                            .arg(Settings.currency)
        }

        Label {
            id: totalLabel
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: mostUsedCategoryLabel.top
                bottomMargin: Theme.paddingLarge
            }
            width: parent.width - 2 * Theme.paddingLarge
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeSmall
            text: qsTr("Total: %1 %2", "1 is amount and 2 is currency")
                                                .arg(Math.round(total))
                                                .arg(Settings.currency)
        }

        Label {
            id: mostUsedCategoryLabel
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: Theme.paddingLarge * 2
            }
            width: parent.width - 2 * Theme.paddingLarge
            truncationMode: TruncationMode.Elide
            elide: Text.ElideRight
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeSmall
            text: qsTr("Most used category: %1").arg(mostUsedCategory)
        }
    }
}
