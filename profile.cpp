#include <QSettings>
#include <QDebug>

#include "profile.h"

Profile::Profile()
{
}

int Profile::save()
{
    if (_uid.isEmpty()) {
        QByteArray uid_b;
        uid_b = QByteArray(QString(_info["username"].toString() + _url.toString()).toLatin1());
        _uid = QString::fromLatin1(uid_b.toBase64());
    }
    
    QSettings set;
    set.beginGroup("Profile");
    set.beginGroup(_uid);
    set.setValue("url", _url);
    set.setValue("info", _info);
    set.setValue("token", _token);
    set.endGroup();
    set.endGroup();
    set.sync();
    
    qDebug() << "(login) stored ID=" << _uid;
    return 0;
}

int Profile::load(QString uid)
{
    if (uid.isEmpty()) {
        uid = _uid;
    }
    
    QSettings set;
    set.beginGroup("Profile");
    if (!set.contains(uid + "/url")) {
        set.endGroup();
        return -1;
    }
    
    set.beginGroup(uid);
    _uid = uid;
    _url = set.value("url").toUrl();
    _info = set.value("info").toHash();
    _token = set.value("token");
    set.endGroup();
    set.endGroup();
    return 0;
}

int Profile::remove()
{
    QSettings set;
    if (set.contains(_uid)) {
        set.remove(_uid);
        return 0;
    }
    return -1;
}

void Profile::remove(const QString& uid)
{
    QSettings set;
    set.beginGroup("Profile");
    set.remove(uid);
    set.endGroup();
}

bool Profile::isValid(const QString &uid)
{
    QSettings set;
    set.beginGroup("Profile");
    if (!set.contains(uid + "/url")) {
        set.endGroup();
        return false;
    }
    set.endGroup();
    return true;
}

QStringList Profile::listProfiles()
{
    QStringList list;
    QSettings set;
    set.beginGroup("Profile");
    list = set.childGroups();
    set.endGroup();
    return list;
}

int Profile::hasCached(int id, int from, int to)
{
    QSettings set;
    set.beginGroup("Profile/" + _uid + "/PostCache");
    int ret = 0;
    int a = set.value(QString::number(id) + "/Min", INT_MAX).toInt();
    int b = set.value(QString::number(id) + "/Max", 0).toInt();
    if (from >= a && to < b) {
        ret = 1;
    }
    set.endGroup();
    
    return ret;
}

QList<ForumItem> Profile::cachedPosts(int id, int from, int to)
{
    QList<ForumItem> l;
    QSettings set;
    set.beginGroup("Profile/" + _uid + "/PostCache/" + QString::number(id));
    
    ForumItem it(-1);
    for (int i = from; i <= to; ++i) {
        it.unserialize(set.value(QString::number(i)).toList());
        l.append(it);
    }
    
    set.endGroup();
    
    return l;
}

void Profile::cachePosts(int id, int from, QList<ForumItem>& list)
{
    QSettings set;
    set.beginGroup("Profile/" + _uid + "/PostCache/" + QString::number(id));
    int a = set.value("Min", INT_MAX).toInt();
    int b = set.value("Max", 0).toInt();
    int to = from + list.size();
    if (from < a) set.setValue("Min", from);
    if (to > b) set.setValue("Max", to);
    
    int i = 0;
    QListIterator<ForumItem> it(list);
    while (it.hasNext()) {
        set.setValue(QString::number(from + i), it.next().serialize());
        ++i;
    }

    set.endGroup();
}

int Profile::clearCache()
{
    QSettings set;
    set.remove("Profile/" + _uid + "/PostCache");
    return 0;
}
