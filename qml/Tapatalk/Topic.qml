import QtQuick 1.1
import com.nokia.meego 1.1
import listmodel.pkg 1.0
import "Delegate"
import "Component"

Page {
    id: page
    anchors.fill: parent
    property int oid: 0
    property int parentId: 0
    property int type: constant.topicType
    property bool scrollToEnd: false
    property int scrollOff: 0
    property string pageName: ""
    property string section: ""
    
    Heading {
        id: header
    }
    
    ScrollDownListView {
        id: postView
        delegate: postDelegate
        anchors {top: header.bottom; bottom: parent.bottom; left: parent.left; right: parent.right; }
        model: listModel
        cacheBuffer: 400
        enableScrollHooks: true
        clip: true
        
        ForumListModel {
            id: listModel;
        }
        
        Component {
            id: postDelegate
            PostItem {
                id: postItem
            }
        }
        
        onScrollDown: {
            nextPage();
        }
        onScrollUp: {
            prevPage();
        }
    }
    
    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: pageStack.pop();
        }
        ToolIcon {
            platformIconId: "toolbar-new-email";
            onClicked: {
                var p = {oid: oid, parentId: parentId};
                app.createDialog("Reply.qml", null, p);
            }
        }
        ToolIcon {
            platformIconId: "toolbar-favorite-mark";
        }
    }
    
    Connections {
        target: tap
        onQueryFinished: {
            postView.setBusy(false);
            if (scrollOff != 0) {
                postView.positionViewAtIndex(scrollOff, ListView.End);
                scrollOff = 0;
            }

            if (scrollToEnd) {
                postView.positionViewAtIndex(listModel.firstUnread - 1 - listModel.base, ListView.Contain);
                scrollToEnd = false;
            }
        }
    }
    
    Component.onCompleted: {
        if (tap.isLogged()) {
            tap.readUnread(oid, listModel);
            scrollToEnd = true;
        } else {
            nextPage();
        }
    }
    
    function prevPage() {
        if (listModel.base > 0) {
            var from = listModel.base - constant.postsPerPage;
            if (from < 0) from = 0;
            var to = listModel.base - 1;
            if (!postView.isBusy()) {
                postView.setBusy(true);
                scrollOff = to - from;
                tap.readTopic(oid, listModel, from, to);
            }
        }
    }
    
    function nextPage() {
        var from = listModel.base + listModel.count;
        var to = from + constant.postsPerPage - 1;
        if (!postView.isBusy()) {
            postView.setBusy(true);
            tap.readTopic(oid, listModel, from, to);
        }
    }
}

