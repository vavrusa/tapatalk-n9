import QtQuick 1.1
import com.nokia.meego 1.1
import QtMobility.feedback 1.1
import listmodel.pkg 1.0
import "Delegate"


PageStackWindow {
    id: app
    
    Const {
        id: constant
    }

    Item {
        id: loadingRect
        anchors.fill: parent
        visible: false
        z: 2
        
        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
            color: '#aaa'
        }

        Rectangle {
            anchors.fill: parent
            color: "#333"
            opacity: 0.4
        }
        
        BusyIndicator {
            visible: loadingRect.visible
            running: visible
            anchors.centerIn: parent
            platformStyle: BusyIndicatorStyle { size: "medium" }
        }
    }

    Component.onCompleted: {
        if (tap.isLogged()) {
            pageStack.clear();
            addPage(constant.forumType, constant.forumTopics, 0, "");
        } else {
            /* Temporary */
            pageStack.replace(Qt.resolvedUrl("User.qml"));
        }
    }
    
    function addPage(t, oid, pid, s) {
        setBusy(true);
        var prop = { type: t, oid: oid, parentId: pid, section: s, pageName: s };
        pageStack.push(Qt.resolvedUrl(constant.typeName(t)), prop);
        setBusy(false);
    }
    
    function login(h, u, p) {
        if (tap.login(h,u,p) < 0) {
            console.log("Login failed.");
            return false;
        }
        return true;
    } 
    
    function setBusy(b) {
        loadingRect.visible = b
    }
    
    function createDialog(dlg, acceptCallback, prop) {
        var c = Qt.createComponent(dlg);
        var dialog = c.createObject(loadingRect, prop)
        dialog.open();
        dialog.accepted.connect(acceptCallback)
    }
}



