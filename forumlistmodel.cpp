#include "forumlistmodel.h"

#include <QHash>
#include <QByteArray>
#include <QDebug>

ForumListModel::ForumListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "itemId";
    roles[ForumIdRole] = "itemForumId";
    roles[ParentRole] = "itemParent";
    roles[NameRole] = "name";
    roles[ContentRole] = "itemContent";
    roles[AuthorRole] = "itemAuthor";
    roles[DateRole] = "itemDate";
    roles[IconRole] = "itemIcon";
    roles[TopicRole] = "isTopic";
    roles[AttachmentRole] = "itemAttachments";
    setRoleNames(roles);
}

int ForumListModel::rowCount(const QModelIndex &parent) const
{
    return _list.count();
}

QVariant ForumListModel::find(int index, int role) const
{
    if (index < 0 || index >= _list.count())
        return QVariant();
    
    const ForumItem &f = _list[index];
    if (role == IdRole)
        return f.id();
    else if (role == ForumIdRole)
        return f.forumId();
    else if (role == ParentRole)
        return f.parent();
    else if (role == NameRole)
        return f.name();
    else if (role == ContentRole)
        return f.info();
    else if (role == AuthorRole)
        return f.author();
    else if (role == DateRole)
        return f.date();
    else if (role == IconRole)
        return f.icon();
    else if (role == TopicRole)
        return f.type() == ForumItem::Topic;
    else if (role == AttachmentRole)
        return f.attachments();
    return QVariant();
}

QVariant ForumListModel::data(const QModelIndex &index, int role) const
{
    return find(index.row(), role);
}

QVariant ForumListModel::get(int index, const QString& role)
{
    const QHash<int, QByteArray>& roles(roleNames());
    QHashIterator<int,QByteArray> it(roles);
    while (it.hasNext()) {
        it.next();
        if (it.value() == role){
            return find(index, it.key());
        }
    }
    
    return QVariant();
}

void ForumListModel::updateList(QList<ForumItem> l, Direction d)
{
    if (l.size() == 0)
        return;
    
    int firstRow = 0;
    if (d == Append) {
        firstRow = rowCount();
    }
    
    beginInsertRows(QModelIndex(), firstRow, firstRow + l.size() - 1);
    
    QListIterator<ForumItem> it(l);
    while(it.hasNext()) {
        _list.insert(firstRow++, it.next());
    }
    endInsertRows();
    countChanged(_list.count());
}

void ForumListModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    _list.clear();
    endRemoveRows();
    countChanged(_list.count());
}
