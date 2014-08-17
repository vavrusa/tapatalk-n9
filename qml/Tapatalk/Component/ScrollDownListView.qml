import QtQuick 1.1
import com.nokia.meego 1.0

ListView {
    
    signal scrollDown()
    signal scrollUp()
    property int __baseY: 0
    property bool __inMove: false
    property bool __up: false
    property bool __down: false
    property bool enableScrollHooks: false
    
    function isBusy() {
        return busy.visible;
    }
    
    function setBusy(v) {
        busy.visible = v;
    }
    
    flickableDirection: Flickable.VerticalFlick
    
    onContentYChanged: {
        if (!enableScrollHooks || !__inMove) return;
        if (atYBeginning) {
            if (__baseY >= 0) __baseY = contentY;
            var posY = contentY - __baseY;
            if (posY < -20) {
                pullDown.visible = true;
                if (posY > -80) {
                    pullDown.height = -posY;
                } else {
                    pullDownRect.opacity = 0.25;
                    __up = true;
                    __down = false;
                }
                return;
            }
        } else if (atYEnd) {
            if (__baseY <= 0) __baseY = contentY;
            var posY = contentY - __baseY;
            if (posY > 120) {
                pullDown.visible = false;
                busy.visible = true;
                __up = false;
                __down = true;
            }
            return;
        }
        
        pullDown.visible = false;
        busy.visible = false;
    }
    
    onMovementStarted: {
        __inMove = true;
        __up = false;
        __down = false;
        __baseY = 0;
        pullDownRect.opacity = 0;
    }
    
    onMovementEnded: {
        if (__up) scrollUp();
        if (__down) scrollDown();
        busy.visible = false;
        pullDown.visible = false;
        __inMove = false;
    }
    
    Item {
        id: pullDown
        height: 80
        width: parent.width
        visible: false
        y: 0
        z: 2
        Rectangle {
            id: pullDownRect
            anchors.fill: pullDown
            visible: parent.visible
            color: '#666'
            opacity: 0
        }
        
        Label {
            anchors.centerIn: parent
            visible: parent.visible
            text: qsTr("Pull down to refresh.");
        }
    }
    
    Item {
        id: busy
        height: 80
        width: parent.width
        visible: false
        y: parent.height - height
        z: 2
        Rectangle {
            anchors.fill: parent
            visible: parent.visible
            color: '#666'
            opacity: 0.25;
        }
        BusyIndicator {
            visible: parent.visible
            running: visible
            anchors.centerIn: parent
            platformStyle: BusyIndicatorStyle { size: "medium" }
        }
    }
}
