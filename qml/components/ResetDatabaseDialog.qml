import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Dialog {

    DialogHeader {
        id: header
        title: qsTr("Reset Database")
        acceptText: qsTr("Reset")
    }

    Label {
        id: warningLabel
        anchors {
            top: header.bottom
            topMargin: Theme.paddingLarge
            horizontalCenter: parent.horizontalCenter
        }
        font.pixelSize: Theme.fontSizeExtraLarge*5
        color: Theme.highlightColor
        wrapMode: Text.WordWrap
        horizontalAlignment: TextInput.AlignHCenter
        width: parent.width*0.8
        text: qsTr("!", "The exclamation mark in the DeleteCategory View")
    }

    Label {
        id: secondWarningLabel
        anchors {
            top: warningLabel.bottom
            topMargin: Theme.paddingLarge
            horizontalCenter: parent.horizontalCenter
        }
        color: Theme.highlightColor
        wrapMode: Text.WordWrap
        horizontalAlignment: TextInput.AlignHCenter
        width: parent.width*0.8
        text: qsTr("All your data will be irreversibly lost!!")
    }

    onAccepted: {
        DataBase.reset()
        DataBase.init()
    }
}
