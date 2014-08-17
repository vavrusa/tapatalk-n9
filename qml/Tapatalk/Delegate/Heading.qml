import QtQuick 1.1
import com.nokia.meego 1.1

Rectangle {
    width: parent.width
    height: 60
    color: "#0066cc"
    Text {
        x: parent.x + 10
        width: parent.width - 10 - 24
        anchors.verticalCenter: parent.verticalCenter
        text: section
        color: "white"
        font.pixelSize: 32
        elide: Text.ElideRight
    }
}

