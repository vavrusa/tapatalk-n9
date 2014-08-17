import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1

Item {
    id: topicItem
    width: parent.width
    height: topicLine.height + topicText.height
    Rectangle {
        id: topicLine
        width: parent.width
        height: 1
        color: '#aaa'
    }
    Label {
        id: topicText
        width: parent.width - 10
        height: topicName.height + topicInfo.height + 20
        anchors.top: topicLine.bottom
        anchors.topMargin: 10
        x: parent.x + 10
        
        Label {
            id: topicName
            width: parent.width
            text: name
            font.pixelSize: 24
            elide: Text.ElideRight
        }
        Label {
            id: topicInfo
            width: parent.width
            anchors.top: topicName.bottom
            color: '#666'
            text: itemContent
            font.pixelSize: 18
            elide: Text.ElideRight
        }
    }
}

