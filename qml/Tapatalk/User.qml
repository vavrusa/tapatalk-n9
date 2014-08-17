import QtQuick 1.1
import com.nokia.meego 1.1
import listmodel.pkg 1.0
import "Delegate"
import "Component"

Page {
    id: userPage
    anchors.fill: parent
    property string section: qsTr("Profile")
    
    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-back"
            visible: app.pageStack.depth > 1
            onClicked: app.pageStack.pop();
        }
        ToolIcon {
            platformIconId: "toolbar-add"
            onClicked: {
                app.pageStack.push(Qt.resolvedUrl("CreateProfile.qml"), {user: userPage});
            }
        }
    }
    
    Heading {
        id: heading
    }
    
    Rectangle {
        id: userInfo
        anchors.top: heading.bottom
        anchors.left: heading.left
        width: parent.width
        height: 136
        color: '#ccc'
        
        Image {
            id: avatar
            x: 14
            width: 110
            height: width
            anchors.verticalCenter: parent.verticalCenter
            source: "image://theme/icon-m-toolbar-contact"
            fillMode: Image.PreserveAspectFit
            smooth: true
        }
        
        Label {
            id: userName
            x: avatar.width + 2 * avatar.x
            y: 14
            font.pixelSize: 32
        }
        
        Label {
            id: userProfile
            x: userName.x
            y: userName.y + userName.height + 8
            color: '#666'
            font.pixelSize: 24
        }
    }
    
    Rectangle {
        id: profileHeading
        color: '#aaa'
        width: parent.width
        height: 48
        anchors.top: userInfo.bottom
        anchors.left: userInfo.left
        
        Label {
            anchors.verticalCenter: parent.verticalCenter
            x: avatar.x
            text: qsTr("List of profiles")
        }
    }
    
    ListView {
        id: profileList
        delegate: profileDelegate
        anchors { top: profileHeading.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
        model: listModel
        section.property: "itemParent"
        section.criteria: ViewSection.FullString
        section.delegate: sectionHeading
        clip: true
        
        highlightMoveSpeed: 1200
        highlightFollowsCurrentItem: true
        highlight: Rectangle {
            color:"lightsteelblue";
            radius: 5; opacity: 0.7
        }
        
        ForumListModel {
            id: listModel
        }
        
        Component {
            id: profileDelegate
            PostItem {
                property variant d: model
                id: portItem
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        profileList.currentIndex = index
                        tap.loadProfile(name);
                        app.pageStack.clear();
                        addPage(constant.forumType, constant.forumTopics, 0, "")
                    }
                    onPressAndHold: {
                        profileList.currentIndex = index;
                        profileMenu.open();
                    }
                }
            }
        }
        
        SectionItem {
            id: sectionHeading
        }
        
        Component.onCompleted: {
            refresh()
        }
    }
    
     Menu {
         id: profileMenu
         visualParent: userPage
         MenuLayout {
             MenuItem {text: qsTr("Log in"); onClicked: {
                     tap.loadProfile(profileList.currentItem.d.name);
                     app.pageStack.clear();
                     app.addPage(constant.forumType, constant.forumTopics, 0, "")
             }}
             MenuItem {text: qsTr("User info"); enabled: false; onClicked: {
                     
             }}
             MenuItem {text: qsTr("Remove profile"); onClicked: {
                     tap.deleteProfile(profileList.currentItem.d.name);
                     userPage.refresh();
             }}
         }
     }
    
    function refresh() {
        if (tap.isLogged()) {
            avatar.source = tap.profile("icon_url");
            userName.text = tap.profile("username");
            userProfile.text = tap.profile("forum");
        } else {
            avatar.source = "image://theme/icon-m-toolbar-contact";
            userName.text = qsTr("Guest");
            userProfile.text = qsTr("not connected");
        }
        tap.listProfiles(listModel)
        var curId = tap.profile();
        for (var i = 0; i < profileList.count; ++i) {
            profileList.currentIndex = i
            if (profileList.currentItem.d.name == curId) break;
        }
    }
}
