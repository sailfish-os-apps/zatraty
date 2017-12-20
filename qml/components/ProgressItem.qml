import QtQuick 2.0
import Sailfish.Silica 1.0

ListItem {
    property real value
    property real minimumValue
    property real maximumValue
    property real progressValue: value / Math.abs(maximumValue - minimumValue)
    property bool withCenter

    Rectangle {
        id: leftRect
        height: parent.height
        width: parent.width * progressValue
        color: Theme.highlightBackgroundColor
        opacity: Theme.highlightBackgroundOpacity
    }

    Rectangle {
        id: centerRect
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height
        width: 2
        color: Theme.primaryColor
        opacity: Theme.highlightBackgroundOpacity
        visible: withCenter
    }

    Rectangle {
        id: rightRect
        anchors.left: leftRect.right
        height: parent.height
        width: parent.width - leftRect.width
        color: Theme.highlightColor
        opacity: Theme.highlightBackgroundOpacity
    }

}
