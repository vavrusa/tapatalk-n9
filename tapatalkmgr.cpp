#include <QUrl>
#include <QDebug>
#include <QStack>
#include <QListView>
#include <QCryptographicHash>
#include <QSettings>

#include "tapatalkmgr.h"
#include "xmlrpcclient.h"
#include "forumlistmodel.h"
#include "profile.h"

static inline QString fromUtf8(QVariant v)
{
    return QString::fromUtf8(v.toString().toLatin1());
}

static void unpackList(QList<ForumItem> &dst, QVariantList src, const QString& section = QString())
{
    QListIterator<QVariant> i(src);
    while(i.hasNext()) {
        QVariantHash f = i.next().toHash();
        if (!f["sub_only"].toBool()) {
            ForumItem item(f["forum_id"].toInt());
            item.setParent(section);
            item.setName(fromUtf8(f["forum_name"]));
            item.setInfo(fromUtf8(f["description"]));
            item.setType(ForumItem::Forum);
            dst.append(item);
        }
        if (f.contains("child")) {
            unpackList(dst, f["child"].toList(), fromUtf8(f["forum_name"]));
        }
    }
}

enum SessionState {
    Guest = 0,
    LoggedIn,
};

class TapatalkMgrPrivate {
public:
    TapatalkMgrPrivate() {
        cli = NULL;
    }

    unsigned state;
    XmlRpcClient* cli;
    QmlApplicationViewer* v;
    Profile sess;
    QMap<QNetworkReply*, TapatalkQuery> pending;
};

TapatalkMgr::TapatalkMgr(QmlApplicationViewer *v) : d(new TapatalkMgrPrivate)
{
    d->v = v;
}

TapatalkMgr::~TapatalkMgr()
{
    if (d->state == LoggedIn) {
        d->sess.setToken(d->cli->cookie());
        d->sess.save();
    }
    
    delete d->cli;
    delete d;
}

void TapatalkMgr::activate()
{
    QSettings set;
    if (set.contains("lastProfile")) {
        loadProfile(set.value("lastProfile").toString());
    }
}

int TapatalkMgr::readTopic(int id, ForumListModel* m, int from, int to)
{
    /* Check if cached. */
    if (d->sess.hasCached(id, from, to)) {
        QList<ForumItem> list = d->sess.cachedPosts(id, from, to);
        ForumListModel::Direction dir = ForumListModel::Append;
        if (from < m->base() || m->count() == 0) {
            m->setBase(from);
            dir = ForumListModel::Prepend;
        }
        m->updateList(list, dir);
        queryFinished();
        return 0;
    }
    
    QVariantList p;
    p << QString::number(id);
    p << from;
    p << to;

    TapatalkQuery& q = query("get_thread", p, m);
    q.from = from;
    return 0;
}

int TapatalkMgr::openForum(int id, ForumListModel* m)
{
    QVariantList p;
    if (id > 0) {
        p << false; // Description
        p << QString::number(id);
    } else {
        p << false; // Description
    }
    
    query("get_forum", p, m);
    
    return 0;
}

int TapatalkMgr::loadTopics(int id, ForumListModel* m, int from, int to)
{
    return fetchTopics(id, m, from, to, "get_topic");
}

int TapatalkMgr::loadUnread(ForumListModel *m, int from, int to)
{
    return fetchTopics(-1, m, from, to, "get_unread_topic");
}

int TapatalkMgr::loadRecent(ForumListModel *m, int from, int to)
{
    return fetchTopics(-1, m, from, to, "get_latest_topic");
}

int TapatalkMgr::readUnread(int id, ForumListModel* m)
{
    QVariantList p;
    p << QString::number(id);
    query("get_thread_by_unread", p, m);
    return 0;
}

int TapatalkMgr::loadMailboxes(ForumListModel *m)
{
    query("get_box_info", QVariantList(), m);
    return 0;
}

int TapatalkMgr::loadMailbox(ForumListModel* m, int boxId, int from, int to)
{
    QVariantList p;
    p << QString::number(boxId);
    p << from;
    p << to;
    query("get_box", p, m);
    return 0;
}

int TapatalkMgr::sendReply(int forumId, int topicId, const QString& msg)
{
    QVariantList p;
    p << QString::number(forumId);
    p << QString::number(topicId);
    p << QString().toLatin1().toBase64(); /* subject */
    p << QString(msg).toUtf8().toBase64();
    query("reply_post", p, NULL);
    return 0;
}

int TapatalkMgr::listProfiles(ForumListModel *m)
{
    int id = 0;
    QList<ForumItem> items;
    QStringList profs = Profile::listProfiles();
    QStringListIterator i(profs);
    qDebug() << profs;
    while(i.hasNext()) {
        Profile p;
        p.load(i.next());
        ForumItem item(++id);
        item.setAuthor(p.info("username").toString());
        item.setIcon(p.info("icon_url").toString());
        item.setParent(p.info("forum").toString());
        item.setName(p.uid());
        item.setType(ForumItem::Topic);
        item.setInfo(p.info("last_activity_time").toString());
        items.append(item);
    }
    
    m->clear();
    m->updateList(items);
    return 0;
}

QString TapatalkMgr::profile(const QString& key)
{
    if (key.isEmpty()) return d->sess.uid();
    return d->sess.info(key).toString();
}

int TapatalkMgr::loadProfile(const QString& uid)
{
    if (Profile::isValid(uid)) {
        QSettings set;
        set.setValue("lastProfile", uid);
        delete d->cli;
        d->cli = NULL;
        d->state = Guest;
        qDebug() << "(login) loading session ID=" << uid;
        if (d->sess.load(uid) == 0) {
            d->cli = new XmlRpcClient(d->sess.url());
            d->cli->setCookie(d->sess.token());
            connect(d->cli, SIGNAL(response(QNetworkReply*,QVariant)), SLOT(handleResponse(QNetworkReply*,QVariant)));
            d->state = LoggedIn;
            qDebug() << "(login) loaded profile ID=" << uid;
            return 0;
        }
    } else {
        qDebug() << "(login) not valid ID=" << uid;
    }
    
    return -1;
}

int TapatalkMgr::deleteProfile(const QString& uid)
{
    if (d->sess.uid() == uid) {
        logout();
    }
    
    QSettings set;
    set.remove("lastProfile");
    
    Profile::remove(uid);
    return 0;
}

int TapatalkMgr::clearProfileCache()
{
    if (isLogged()) {
        return d->sess.clearCache();
    }
    return -1;
}

int TapatalkMgr::login(QString host, QString user, QString pass)
{
    /* Update Url */
    if (!host.endsWith('/')) {
        host.append("/");
    }
    host.append("mobiquo/mobiquo.php");
    QUrl url(host);
    XmlRpcClient* cli = new XmlRpcClient(url);
    
    /* Fetch configuration */
    qDebug() << "(login) retrieving forum config";
    QVariantHash cfg = cli->call("get_config").toHash();
    QByteArray passHash(pass.toLatin1());
    if (cfg["support_sha1"].toString() == "1") {
        passHash = QCryptographicHash::hash(passHash, QCryptographicHash::Sha1).toHex();
    } else if (cfg["support_md5"].toString() == "1") {
        passHash = QCryptographicHash::hash(passHash, QCryptographicHash::Md5).toHex();
    } else {
        passHash = passHash.toBase64();
    }
    
    /* Issue request. */
    QVariantList p;
    p << QByteArray(user.toLatin1()).toBase64();
    p << passHash;
    QVariantHash r = cli->call("login", p).toHash();
    
    /* Check result. */
    if (!r["result"].toBool()) {
        qDebug() << "(login) failed: " << r["result_text"];
        delete cli;
        return -1;
    }
    
    cli->call("get_inbox_stat", QVariantList());
    
    /* Fetch user info. */
    p.pop_back(); 
    r = cli->call("get_user_info", p).toHash();
    r["last_activity_time"] = r["last_activity_time"].toDateTime().toString(Qt::SystemLocaleShortDate);
    r["forum"] = cli->url().host();
    
    Profile sess;
    sess.setUrl(url);
    sess.setInfo(r);
    sess.setToken(cli->cookie());
    sess.save();
    delete cli;
    
    return loadProfile(sess.uid());
}

int TapatalkMgr::logout()
{
    qDebug() << "(logout) profile ID=" << d->sess.uid();
    d->cli->call("logout_user", QVariantList());
    d->sess.remove();
    d->sess = Profile();
    d->state = Guest;
    return 0;
}

bool TapatalkMgr::isLogged()
{
    return d->state == LoggedIn;
}

int TapatalkMgr::fetchTopics(int id, ForumListModel* m, int from, int to, const QString& method)
{
    QVariantList p;
    if (id >= 0) {
        p << QString::number(id);
    }
    p << from; //firstnum
    p << to; //lastnum
    
    query(method, p, m);
    return 0;
}

TapatalkQuery& TapatalkMgr::query(const QString& id, const QVariantList& v, ForumListModel *m)
{
    QNetworkReply *r = d->cli->request(id, v);
    d->pending.insert(r, TapatalkQuery(id, m));
    return d->pending[r];
}

void TapatalkMgr::handleResponse(QNetworkReply* id, QVariant v)
{
    if (!d->pending.contains(id)) return;
    TapatalkQuery q = d->pending[id];
    d->pending.remove(id);
    ForumListModel::Direction dir = ForumListModel::Append;
    
    qDebug() << "reply:" << q.id << "model:" << q.m;
    if (q.id.isEmpty()) {
        qDebug() << "no pending query";
        return;
    }
    
    QList<ForumItem> list;
    if (q.id == "get_forum") {
        unpackList(list, v.toList());
    }
    
    if (q.id == "get_topic" || q.id == "get_unread_topic" || q.id == "get_latest_topic") {
        QVariantHash r = v.toHash();
        QVariantList topics(r["topics"].toList());
        QListIterator<QVariant> i(topics);
        while (i.hasNext()) {
            QVariantHash f = i.next().toHash();
            ForumItem item(f["topic_id"].toInt());
            item.setForumId(f["forum_id"].toInt());
            item.setName(fromUtf8(f["topic_title"]));
            item.setInfo(fromUtf8(f["short_content"]));
            item.setType(ForumItem::Topic);
            item.setParent(fromUtf8(r["forum_name"]));
            list.append(item);
        }
    }
    
    if (q.id == "get_thread_by_unread") {
        QVariantHash r = v.toHash();
        q.m->setFirstUnread(r["position"].toInt());
        q.m->clear();
        int from = (q.m->firstUnread() / 20) * 20;
        qDebug() << "firstUnread" << q.m->firstUnread();
        readTopic(r["topic_id"].toInt(), q.m, from, from + 20 - 1);
        return;
    }
    
    if (q.id == "get_thread") {
        QVariantHash r = v.toHash();
        if (r["total_post_num"].toInt() <= q.m->base() + q.m->count() && q.from >= q.m->base()) {
            queryFinished();
            return;
        }
        QVariantList posts(r["posts"].toList());
        QListIterator<QVariant> i(posts);
        while (i.hasNext()) {
            QVariantHash f = i.next().toHash();
            ForumItem item(f["post_id"].toInt());
            item.setName(fromUtf8(f["post_title"]));
            item.setAuthor(fromUtf8(f["post_author_name"]));
            item.parseInfo(fromUtf8(f["post_content"]));
            item.setIcon(fromUtf8(f["icon_url"]));
            item.setDate(f["post_time"].toDateTime());
            //qDebug() << f["post_time"].toString() << f["post_time"].toDateTime();
            item.setType(ForumItem::Topic);
            item.setParent(fromUtf8(r["topic_title"]));
            
            QVariantList atlist(f["attachments"].toList());
            QListIterator<QVariant>ai(atlist);
            while(ai.hasNext()) {
                QVariantHash a = ai.next().toHash();
                if (a["content_type"].toString() == "image") {
                    item.addAttachment(a["url"].toString());
                }
            }
            
            list.append(item);
        }
        
        /* Fill cache. */
        int topic_id = r["topic_id"].toInt();
        if (q.from < q.m->base() || q.m->count() == 0) {
            q.m->setBase(q.from);
            dir = ForumListModel::Prepend;
        }
        d->sess.cachePosts(topic_id, q.from, list);
    }
    
    if (q.id == "get_box_info") {
        QVariantHash r = v.toHash();
        QVariantList boxes(r["list"].toList());
        QListIterator<QVariant> i(boxes);
        while (i.hasNext()) {
            QVariantHash f = i.next().toHash();
            ForumItem item(f["box_id"].toString().toInt());
            item.setName(fromUtf8(f["box_name"]));
            item.setType(f["msg_count"].toInt());
            list.append(item);
        }
    }
    
    if (q.id == "get_box") {
        QVariantHash r = v.toHash();
        if (r["total_message_count"].toInt() <= q.m->count()) {
            queryFinished();
            return;
        }
        QVariantList posts(r["list"].toList());
        QListIterator<QVariant> i(posts);
        while (i.hasNext()) {
            QVariantHash f = i.next().toHash();
            ForumItem item(f["msg_id"].toString().toInt());
            item.setName(fromUtf8(f["msg_subject"]));
            item.setAuthor(fromUtf8(f["msg_from"]));
            item.parseInfo(fromUtf8(f["short_content"]));
            item.setIcon(fromUtf8(f["icon_url"]));
            item.setDate(f["sent_date"].toDateTime());
            item.setType(f["msg_state"].toInt());
            list.append(item);
        }
    }

    q.m->updateList(list, dir);
    queryFinished();
}
