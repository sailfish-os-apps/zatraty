import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: page

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Create backup")
                onClicked: {
                    if (!backupsModel.create())
                        resultLabel.text = DataBase.error
                }
            }
        }

        PageHeader {
            id: header
            title: qsTr("Data backups")
        }

        Label {
            id: resultLabel
            anchors {
                top: header.bottom
                topMargin: Theme.paddingLarge
                horizontalCenter: parent.horizontalCenter
            }
            font.pixelSize: Theme.fontSizeExtraLarge
            color: Theme.highlightColor
            wrapMode: Text.WordWrap
            horizontalAlignment: TextInput.AlignHCenter
            width: parent.width * 0.8
            text: ""
        }

        SilicaListView {
            id: backupsList
            model: BackupListModel {
                id: backupsModel
            }
            anchors {
                top: resultLabel.bottom
                topMargin: Theme.paddingLarge
            }
            height: parent.height - x
            width: parent.width

            delegate: ListItem {
                id: backupItem
                width: parent.width
                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("Apply")
                        onClicked: {
                            remorseAction(qsTr("Applying backup"),
                             function() {
                                 if (!backupsModel.apply(index))
                                    resultLabel.text = DataBase.error
                             })
                        }
                    }

                    MenuItem {
                        text: qsTr("Delete")
                        onClicked: {
                            remorseAction(qsTr("Deleting backup"),
                             function() {
                                 if (!backupsModel.remove(index))
                                     resultLabel.text = DataBase.error
                             })
                        }
                    }
                }

                Label {
                    id: backupName
                    anchors.centerIn: parent
                    text: viewName
                }
            }
            VerticalScrollDecorator {}
        }
    }
}
