var nextId = 0;
var topicCount = 0;
var postsPerPage = 20;

function fetchTopics(tap, oid, listModel) {
    var nval = nextId + postsPerPage - 1;
    switch(oid) {
    case 0:  /* constant.forumTopics */
        tap.openForum(oid, listModel); break;
    case -1: /* constant.unreadTopics */
        topicCount = tap.loadUnread(listModel, nextId, nval); break;
    case -2: /* constant.recentTopics */
        topicCount = tap.loadRecent(listModel, nextId, nval); break;
    default:
        topicCount = tap.loadTopics(oid, listModel, nextId, nval); break;
    }
    nextId = nval + 1;
    if (nextId >= topicCount) {
        nextId = topicCount
    }
}

WorkerScript.onMessage = function(msg) {
    if (msg.action == 'fetchTopics') {
        fetchTopics(msg.tap, msg.oid,  msg.model);
        msg.model.sync();
    }
}
