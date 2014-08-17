import QtQuick 1.1
import com.nokia.meego 1.1
import listmodel.pkg 1.0
import "Delegate"
import "Component"

Page {
    id: pm
    anchors.fill: parent
    property bool isBoxChanged: true
    property int currentBox: -1
    property string pageName: ""
    property string section: qsTr("Private Messages")
    
    Heading {
        id: header
        ToolIcon {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            iconId: "toolbar-down-white"
        }
    }
    
    MouseArea {
        anchors.fill: parent
        onClicked: {
            boxDialog.open();
        }
    }
    
    SelectionDialog {
            id:boxDialog
            titleText: qsTr("Pick your message box")
            selectedIndex: 0
            model: boxModel
            ForumListModel { id: boxModel }
            onAccepted: {
                var id = boxDialog.selectedIndex;
                getMailbox(boxModel.get(id, 'itemId'), boxModel.get(id, 'name'));
            }
    }
    
    ScrollDownListView {
        id: postView
        delegate: postDelegate
        anchors {top: header.bottom; bottom: parent.bottom; left: parent.left; right: parent.right; }
        model: listModel
        cacheBuffer: 400
        clip: true
        
        ForumListModel { id: listModel; }
        Component {
            id: postDelegate
            PostItem { id: postItem }
        }
        
        onScrollDown: {
            var nextId = boxModel.count;
            var nval = nextId + constant.postsPerPage - 1;
            postView.setBusy(true);
            tap.loadMailbox(listModel, currentBox, nextId, nval);
        }
    }
    
    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: pageStack.pop();
        }
        ToolIcon {
            platformIconId: "toolbar-new-email";
        }
        ToolIcon {
            platformIconId: "toolbar-favorite-mark";
        }
    }
    
    BusyIndicator {
        id: busyIndicator
        visible: true
        running: visible
        anchors.centerIn: parent
        platformStyle: BusyIndicatorStyle { size: "large" }
    }
    
    Connections {
        target: tap
        onQueryFinished: {
            postView.setBusy(false);
            busyIndicator.visible = false;
            refresh();
            if (isBoxChanged) {
                isBoxChanged = false;
                getMailbox(boxModel.get(0, 'itemId'), boxModel.get(0, 'name'));
            }
        }
    }
    
    Component.onCompleted: {
        tap.loadMailboxes(boxModel);
    }
    
    function refresh() {
        if (pageName.length == 0)
            section = qsTr("Private Messages");
        else
            section = pageName
        if (busyIndicator.visible)
            section += " (...)";
        else
            section += " (" + listModel.count + ")"
    }
    
    function getMailbox(boxId, boxName) {
        console.log("getMailbox " + boxId + ";" + boxName);
        listModel.clear();
        pageName = boxName;
        currentBox = boxId;
        refresh();
        busyIndicator.visible = true;
        tap.loadMailbox(listModel, boxId, 0, constant.postsPerPage - 1);
    }
}
