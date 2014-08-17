#ifndef TAPATALKMGR_H
#define TAPATALKMGR_H

#include <QObject>
#include <QVariantList>
#include <QDeclarativeItem>
#include <QAbstractItemModel>
#include <QNetworkReply>
#include "qmlapplicationviewer.h"

#include "forumitem.h"
#include "forumlistmodel.h"
#include "profile.h"

class TapatalkMgrPrivate;

class TapatalkQuery {
public:
    TapatalkQuery(const QString& _id = QString(), ForumListModel* _m = 0)
        : id(_id), m(_m) {}
    QString id;
    ForumListModel *m;
    int from;
};

class TapatalkMgr : public QObject
{
    Q_OBJECT
public:
    TapatalkMgr(QmlApplicationViewer* v);
    virtual ~TapatalkMgr();

public slots:
    void activate();
    
    int readTopic(int id, ForumListModel* m, int from, int to);
    int readUnread(int id, ForumListModel* m);
    int openForum(int id, ForumListModel* m);
    int loadTopics(int id, ForumListModel *m, int from, int to);
    int loadUnread(ForumListModel *m, int from, int to);
    int loadRecent(ForumListModel *m, int from, int to);
    int loadMailboxes(ForumListModel* m);
    int loadMailbox(ForumListModel* m, int boxId, int from, int to);
    
    int sendReply(int forumId, int topicId, const QString& msg);
    
    int listProfiles(ForumListModel *m);
    QString profile(const QString& key = QString());
    int loadProfile(const QString& uid);
    int deleteProfile(const QString& uid);
    int clearProfileCache();
    
    int login(QString host, QString user, QString pass);
    int logout();
    bool isLogged();
    
signals:
    void queryFinished();
    
protected:
    int fetchTopics(int id, ForumListModel* m, int from, int to, const QString& method);
    TapatalkQuery& query(const QString& id, const QVariantList& v, ForumListModel *m);
    
protected slots:
    void handleResponse(QNetworkReply* id, QVariant v);

private:
    TapatalkMgrPrivate* d;
    Q_DECLARE_PRIVATE (TapatalkMgr);
};

#endif // TAPATALKMGR_H
