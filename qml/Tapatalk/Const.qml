import QtQuick 1.1

QtObject {
    id: constant
    
    /* Forum type */
    property int forumType: 1
    property int topicType: 2
    
    function typeName(t) {
        if (t == topicType) return "Topic.qml";
        if (t == forumType) return "Forum.qml";
        return "";
    }
    
    /* special identifiers */
    property int forumTopics: 0
    property int unreadTopics: -1
    property int recentTopics: -2
    
    /* Posts */
    property int postsPerPage: 20
}
