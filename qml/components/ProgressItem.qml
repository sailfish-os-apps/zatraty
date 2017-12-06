import QtQuick 2.0
import Sailfish.Silica 1.0

ListItem {
    property real value
    property real minimumValue
    property real maximumValue
    property real progressValue: value / Math.abs(maximumValue - minimumValue)

    Rectangle {
        id: leftRect
        height: parent.height
        width: parent.width * progressValue
        color: Theme.highlightBackgroundColor
        opacity: Theme.highlightBackgroundOpacity
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
