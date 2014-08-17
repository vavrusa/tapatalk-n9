import QtQuick 1.1
import com.nokia.meego 1.1
import listmodel.pkg 1.0
import "Delegate"
import "Component"

Page {
    id: page
    anchors.fill: parent
    property int oid: constant.forumTopics
    property int parentId: 0
    property int type: constant.forumType
    property string pageName: ""
    property string section: qsTr("Forum")
    
    Heading {
        id: header
    }
    
    ScrollDownListView {
        id: forumList
        delegate: forumDelegate
        anchors { top: header.bottom; bottom: parent.bottom; left: parent.left; right: parent.right; }
        model: listModel
        section.property: "itemParent"
        section.criteria: ViewSection.FullString
        section.delegate: SectionItem {}
        clip: true
        cacheBuffer: 400
        
        highlightMoveSpeed: 1200
        highlightFollowsCurrentItem: true
        highlight: Rectangle {
            color:"lightsteelblue";
            radius: 5; opacity: 0.7
        }
        
        ForumListModel {
            id: listModel;
        }
        
        Component {
            id: forumDelegate
            TopicItem {
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        forumList.currentIndex = index;
                        var ntype = constant.forumType;
                        if (oid != constant.forumTopics)
                            ntype = constant.topicType;
                        app.addPage(ntype, itemId, itemForumId, name)
                    }
                }
            }
        }
        
        onScrollDown: {
            if (oid != constant.forumTopics) {
                forumList.setBusy(true);
                fetchTopics();
            }
        }
    }
    
    tools: ToolBarLayout {
        ToolIcon {
            id: backBtn
            enabled: oid != constant.forumTopics
            platformIconId: oid != constant.forumTopics ? "toolbar-back" : "toolbar-back-dimmed"
            onClicked: pageStack.pop();
        }
        ToolIcon {
            id: recentBtn
            iconId: "toolbar-callhistory"
            visible: oid == constant.forumTopics
            onClicked: {
                addPage(constant.forumType, constant.recentTopics, 0, "")
            }
        }
        ToolIcon {
            id: unreadBtn
            iconId: tap.isLogged() ? "toolbar-history" : "toolbar-history-dimmed"
            enabled: tap.isLogged()
            visible: oid == constant.forumTopics
            onClicked: {
                addPage(constant.forumType, constant.unreadTopics, 0, "")
            }
        }
        ToolIcon {
            iconId: "toolbar-new-chat"
            visible: oid == constant.forumTopics
            onClicked: pageStack.push(Qt.resolvedUrl("PM.qml"));
        }

        ToolIcon {
            iconId: "toolbar-addressbook"
            visible: oid == constant.forumTopics
            onClicked: {
                pageStack.push(Qt.resolvedUrl("User.qml"));
            }
        }
        ToolIcon {
            iconId: "toolbar-settings"; //settings
            visible: oid == constant.forumTopics
            onClicked: {
                pageStack.push(Qt.resolvedUrl("Config.qml"));
            }
        }
    }
    
    Label {
        id: placeholder
        anchors.centerIn: forumList
        visible: false
        font.pixelSize: 48
        color: "lightsteelblue"
    }
    
    BusyIndicator {
        id: busyIndicator
        visible: false
        running: visible
        anchors.centerIn: parent
        platformStyle: BusyIndicatorStyle { size: "large" }
    }
    
    function fetchTopics() {
        var nextId = listModel.count;
        var nval = nval + constant.postsPerPage - 1;
        switch(oid) {
        case constant.forumTopics:
            tap.openForum(oid, listModel); break;
        case constant.unreadTopics:
            tap.loadUnread(listModel, nextId, nval); break;
        case constant.recentTopics:
            tap.loadRecent(listModel, nextId, nval); break;
        default:
            tap.loadTopics(oid, listModel, nextId, nval); break;
        }
    }
    
    function refresh(count) {
        /* Update heading. */
        switch(oid) {
        case constant.forumTopics:
            section = qsTr("Forum");
            break;
        case constant.unreadTopics:
            section = qsTr("Unread topics");
            placeholder.text = qsTr("No unread topics.");
            break;
        case constant.recentTopics:
            section = qsTr("Recent topics");
            placeholder.text = qsTr("No recent topics.");
            break;
        default:
            section = pageName;
            break;
        }
        if (count > -1) section += " (" + count + ")";
        else            section += " (...)";
        placeholder.visible = (count == 0);
        busyIndicator.visible = (count < 0);
    }
    
    Connections {
        target: tap
        onQueryFinished: {
            forumList.setBusy(false);
            refresh(listModel.count);
        }
    }
    
    Component.onCompleted: {
        refresh(-1);
        fetchTopics();
        header.visible = true;
        forumList.currentIndex = -1
    }
}

