import QtQuick 1.1
import com.nokia.meego 1.1

Item {
    id: postItem
    width: parent.width
    height: postText.height + 24
    Rectangle {
        id: postLine
        width: parent.width
        height: 1
        color: '#aaa'
    }
    Label {
        id: avatar
        width: 80
        height: postText.height
        anchors.top: postLine.bottom
        anchors.topMargin: 10

        Image {
            width: 64
            height: 64
            anchors.horizontalCenter: parent.horizontalCenter
            source: itemIcon
            fillMode: Image.PreserveAspectFit
            smooth: true
        }
    }
    Item {
        id: postText
        width: parent.width - avatar.width - 10
        height: postName.height + postInfo.height + postIcon.height + 10
        anchors.top: postLine.bottom
        anchors.left: avatar.right
        anchors.topMargin: 10
        
        Label {
            id: postName
            anchors.top: postText.top
            text: itemAuthor
            width: parent.width - postDate.width
            font.pixelSize: 24
            elide: Text.ElideRight
        }
        Label {
            id: postDate
            anchors.top: postText.top
            anchors.right: postText.right
            color: '#444'
            text: itemDate
            font.pixelSize: 18
            
        }
        Text {
            id: postInfo
            anchors.top: postName.bottom
            width: parent.width
            color: '#666'
            text: itemContent
            textFormat: Text.RichText
            wrapMode: Text.WordWrap
            font.pixelSize: 18
            onLinkActivated: Qt.openUrlExternally(link)
        }
        Image {
            id: postIcon
            source: "image://theme/icon-m-toolbar-camera"
            anchors.top: postInfo.bottom
            anchors.right: postInfo.right
            visible: itemAttachments.length > 0
            fillMode: Image.PreserveAspectFit
            smooth: true
        }
    }
    
    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (itemAttachments.length == 0) return;
            var p = {author: itemAuthor, avatar: itemIcon, date: itemDate, msg: itemContent, attachments: itemAttachments };
            if (itemAttachments.length == 1) {
                app.pageStack.push(Qt.resolvedUrl("../ShowImage.qml"), {url:itemAttachments[0]});
            } else {
                app.pageStack.push(Qt.resolvedUrl("../PostDetail.qml"), p);
            }
        }
    }
}

