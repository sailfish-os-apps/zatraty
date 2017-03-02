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
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: qsTr("History")
                onClicked: pageStack.push(Qt.resolvedUrl("HystoryPage.qml"))
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
                        ExpenseModel.add(dialog.category, dialog.amount, dialog.desc)
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

        Column {
            id: column
            width: page.width
            spacing: Theme.paddingSmall

            PageHeader {
                title: Settings.appName
            }

            Label {
                id: moneyLabel
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                height: contentHeight
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge * 3
                fontSizeMode: Text.HorizontalFit
                text: "%1 %2".arg(Math.round(totalThisMonth))
                             .arg(Settings.currency)
            }

            Label {
                anchors { horizontalCenter: parent.horizontalCenter }
                text: qsTr("spent this month", "subtitle of the amount spent in the MainView")
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeLarge
            }
        }

        Label {
            id: lastMonthLabel
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: column.bottom
                topMargin: Theme.paddingLarge*15
            }
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeMedium
            text: qsTr("Last Month: %1 %2", "1 is amount and 2 is currency")
                                            .arg(Math.round(totalLastMonth))
                                            .arg(Settings.currency)
        }

        Label {
            id: totalLabel
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: lastMonthLabel.bottom
                topMargin: Theme.paddingLarge
            }
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeMedium
            text: qsTr("Total: %1 %2", "1 is amount and 2 is currency")
                                                .arg(Math.round(total))
                                                .arg(Settings.currency)
        }

        Label {
            id: mostUsedCategoryLabel
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: totalLabel.bottom
                topMargin: Theme.paddingLarge
            }
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeMedium
            text: qsTr("Most used category: %1").arg(mostUsedCategory)
        }
    }
}
