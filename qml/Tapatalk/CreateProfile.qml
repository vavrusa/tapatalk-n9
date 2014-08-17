import QtQuick 1.1
import com.nokia.meego 1.1
import "Delegate"

Page {
    id: page
    anchors.fill: parent
    property Page user: null
    property string section: qsTr("Create profile")
    
    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: pageStack.pop();
        }
    }
    
    Heading {
        id: heading
    }
    
    
    Flickable {
        id: flickable
        anchors.top: heading.bottom
        anchors.topMargin: 32
        contentWidth: parent.width
        contentHeight: mainColumn.height + 2 * mainColumn.anchors.topMargin
        
        Column {
            id: mainColumn
            anchors.fill: parent
            anchors.leftMargin: flickable.anchors.topMargin
            height: childrenRect.height
            width: parent.contentWidth
            spacing: 16
         
            
            Text {
                text: "Forum address"
                font.bold: true
                font.pixelSize: 24
            }
            
            TextField {
                id: loginHost
                focus: true
                width: parent.width - mainColumn.anchors.leftMargin
                text: ""
            }
            
            
            Text {
                text: "Login"
                font.bold: true
                font.pixelSize: 24
            }
            TextField {
                id: loginUser
                width: parent.width - mainColumn.anchors.leftMargin
                text: ""
            }
            
            Text {
                text: "Password"
                font.bold: true
                font.pixelSize: 24
            }
            TextField {
                id: loginPassword
                width: parent.width - mainColumn.anchors.leftMargin
                echoMode: TextInput.Password
                text: ""
            }
            
            Button {
                text: "Create"
                onClicked: {
                    app.login(loginHost.text, loginUser.text, loginPassword.text)
                    if (user != null) {
                        user.refresh();
                    }

                    if (app.pageStack.depth > 1) {
                        app.pageStack.pop();
                    } else {
                        app.pageStack.clear();
                        app.addPage(constant.forumType, constant.forumTopics, 0, "");
                    }
                }
            }
        }
    }
    
    ScrollDecorator { flickableItem: flickable }
}
