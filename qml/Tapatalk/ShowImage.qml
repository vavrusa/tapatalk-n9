import QtQuick 1.1
import com.nokia.meego 1.1

Item {
    id: imagePage
    property alias url: photo.source
    property int photoHeight
    property int photoWidth
    anchors.fill: parent
    
    Rectangle {
        color: 'black'
        anchors.fill: parent
    }
    
    Flickable {
        id: flickable
        width:  parent.width
        height: parent.height
        contentWidth: photo.width
        contentHeight: photo.height
        onHeightChanged: photo.calculateSize()

        Image {
            id: photo
            property real prevScale
            anchors.centerIn: parent
            smooth: !flickable.movingVertically
            sourceSize.width:  imagePage.photoWidth
            sourceSize.height:imagePage.photoHeight
            fillMode: Image.PreserveAspectFit
            
            function calculateSize()
            {
                scale = Math.min(flickable.width / width, flickable.height / height) * 0.98;
                pinchArea.minScale = scale;
                prevScale = Math.min(scale, 1);
            }
            
            onScaleChanged: {
                
                if ((width * scale) > flickable.width) {
                    var xoff = (flickable.width / 2 + flickable.contentX) * scale / prevScale;
                    flickable.contentX = xoff - flickable.width / 2;
                }
                if ((height * scale) > flickable.height) {
                    var yoff = (flickable.height / 2 + flickable.contentY) * scale / prevScale;
                    flickable.contentY = yoff - flickable.height / 2;
                    prevScale = scale;
                }
            }
            
            PinchArea{
                id: pinchArea
                property real minScale:  1.0
                anchors.fill: parent
                property real lastScale: 1.0
                pinch.target: photo
                pinch.minimumScale: minScale
                pinch.maximumScale: 3.0
                
                onPinchFinished: flickable.returnToBounds()
            }
            
            
            MouseArea {
                anchors.fill: photo
                onClicked: app.pageStack.pop();
            }
        }
    }
}
