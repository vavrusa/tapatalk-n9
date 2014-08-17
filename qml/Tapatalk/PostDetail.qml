import QtQuick 1.1
import QtMobility.gallery 1.1
import com.nokia.meego 1.1
import listmodel.pkg 1.0
import "Delegate"
import "Component"

Page {
    
    id: postDetail
    property string author
    property string avatar
    property string date
    property string msg
    property variant attachments: {}
    
    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-back"
            visible: app.pageStack.depth > 1
            onClicked: app.pageStack.pop();
        }
    }
    
    Rectangle {
        color: 'black'
        anchors.fill: parent
        ListView {
            id: gallery
            anchors.fill: parent
            anchors.topMargin: 16
            spacing: 16
            
            model: ListModel {
                id: galleryModel
            }
            
            delegate: Image {
                id: dlgImg
                source: url
                width: parent.width
                fillMode: Image.PreserveAspectFit
                MouseArea {
                    anchors.fill: parent
                    onClicked: { app.pageStack.push(Qt.resolvedUrl("ShowImage.qml"), {url: url}); }
                }
            }
        }
    }
    
    Component.onCompleted: {
        for (var i = 0; i < attachments.length; ++i)
            galleryModel.append({url:attachments[i]});
    }
}

