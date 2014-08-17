import QtQuick 1.1
import com.nokia.meego 1.1
import "Delegate"

Page {
    id: page
    anchors.fill: parent
    property string section: qsTr("Settings")
    
    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: app.pageStack.pop();
        }
    }
    
    Heading {
        id: heading
    }
    
    Button {
        id: useCache
        text: "Use cache";
        onClicked: {
            if (tap.isLogged()) {
                tap.clearProfileCache();
                text = "Cache cleared";
            }
        }
    }
}
