import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

CoverBackground {

    Column {
           anchors.centerIn: parent
           width: parent.width
           spacing: Theme.paddingMedium

           Image {
               anchors.horizontalCenter: parent.horizontalCenter
               source: "/usr/share/icons/hicolor/86x86/apps/harbour-zatraty.png"
           }

           Label {
               anchors.horizontalCenter: parent.horizontalCenter
               text: qsTr("This month:")
               color: Theme.highlightColor
               font.pixelSize: Theme.fontSizeMedium
           }

           Label {
               id: label
               anchors.horizontalCenter: parent.horizontalCenter
               color: Theme.highlightColor
               font.pixelSize: Theme.fontSizeLarge
               text: qsTr("%1 %2", "1 is amount and 2 is currency")
                        .arg(ExpenseModel.totalMonthAmount())
                        .arg(Settings.currency)
           }
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-new"
            onTriggered: {
                if(!appWindow.quickAddOpen) {
                    var dialog = pageStack.push(Qt.resolvedUrl("../components/NewEntryDialog.qml"));
                    dialog.accepted.connect(function() {
                        ExpenseModel.add(dialog.category, dialog.amount, dialog.desc)
                    })
                }
                appWindow.activate();
            }
        }
    }
}

