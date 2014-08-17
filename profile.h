#ifndef PROFILE_H
#define PROFILE_H

#include <QUrl>
#include <QStringList>
#include <QVariantHash>
#include "forumitem.h"

class Profile
{
    
public:
    Profile();
    
    QString uid() { return _uid; }
    QUrl url()  { return _url; }
    void setUrl(const QUrl& url) { _url = url; }
    void setInfo(const QVariantHash& info) { _info = info; }
    QVariant info(const QString& key) { return _info[key]; }
    QVariant token() { return _token; }
    void setToken(const QVariant& token) { _token = token; }
    
    int hasCached(int id, int from, int to);
    QList<ForumItem> cachedPosts(int id, int from, int to);
    void cachePosts(int id, int from, QList<ForumItem>& list);
    int clearCache();
    
    int save();
    int load(QString uid);
    int remove();
    static void remove(const QString& uid);
    static bool isValid(const QString& uid);
    static QStringList listProfiles();
    
private:
    QString _uid;
    QUrl _url;
    QVariantHash _info;
    QVariant _token;
};

#endif // PROFILE_H
