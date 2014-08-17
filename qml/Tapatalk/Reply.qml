import QtQuick 1.1
import com.nokia.meego 1.1
import listmodel.pkg 1.0
import "Delegate"
import "Component"

Sheet {
    id: reply
    anchors.fill: parent
    property int oid: 0
    property int parentId: 0
    property string section: qsTr("Reply to")
    
    acceptButtonText: qsTr("Send")
    rejectButtonText: qsTr("Cancel")
    
    content: Flickable {
        width: parent.width
        height: parent.height
        Item {
            id: separator
            anchors {left: parent.left; right: parent.right }
            height: 80
            Label {
                x: 32 /* medium margin */
                color: '#333'
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Your reply...");
                
            }
        }
        
        TextArea {
            id: replyText
            width: parent.width
            anchors {top: separator.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        }
    }
    
    onAccepted: {
        tap.sendReply(parentId, oid, replyText.text);
    }
}
