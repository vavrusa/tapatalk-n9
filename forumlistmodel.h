#ifndef FORUMLISTMODEL_H
#define FORUMLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include "forumitem.h"

class ForumListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int firstUnread READ firstUnread WRITE setFirstUnread)
    Q_PROPERTY(int base READ base WRITE setBase)
    
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        ForumIdRole,
        ParentRole,
        NameRole,
        ContentRole,
        AuthorRole,
        DateRole,
        IconRole,
        TopicRole,
        AttachmentRole
    };
    
    enum Direction {
        Prepend = -1,
        Append = 0
    };

    ForumListModel(QObject *parent = 0);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant find(int index, int role) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QList<ForumItem>& items() {
        return _list;
    }

private:
         QList<ForumItem> _list;
         int _firstUnread;
         int _base;

public slots:
    QVariant get(int index, const QString& role);
    int base() {
        return _base;
    }
    void setBase(int base) {
        _base = base;
    }

    int firstUnread() {
        return _firstUnread;
    }
    void setFirstUnread(int id) {
        _firstUnread = id;
    }
    void updateList(QList<ForumItem> l, Direction d = Append);
    void clear();
    int count() {
        return _list.count();
    }
signals:
    void countChanged(int newCount);
};

#endif // FORUMLISTMODEL_H
