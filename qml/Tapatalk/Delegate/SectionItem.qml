import QtQuick 1.1
import com.nokia.meego 1.1

Component {
    Rectangle {
        width: parent.width
        height: 40
        color: "#ccc"
        Text {
            anchors.verticalCenter: parent.verticalCenter
            x: parent.x + 10
            text: section
            color: "#666"
            font.bold: true
            font.pixelSize: 24
        }
    }
}
