#include <QDebug>

#include "forumitem.h"

ForumItem::ForumItem(int id)
    : _id(id), _type(0)
{
}

void ForumItem::parseInfo(QString s)
{
    /* Find attachments. */
    int i = -1;
    while ((i = s.indexOf("[img]")) > -1) {
        s.remove(i, 5);
        int n = s.indexOf("[/img]");
        _attach.append(s.mid(i, n-i));
        s.remove(i, n-i+6);
    }
    
    /* URLs */
    s.replace(QRegExp("\\[url=(.*)\\](.*)\\[/url\\]"), "<a href=\"\\1\">\\2</a>");
    
    
    /* Set rest as info. */
    setInfo(s);
}

QVariantList ForumItem::serialize() const
{
    QVariantList l;
    l << _id;
    l << _forumId;
    l << _parent;
    l << _name;
    l << _info;
    l << _author;
    l << _date;
    l << _icon;
    l << _type;
    l << _attach;
    return l;
}

void ForumItem::unserialize(QVariantList l)
{
    _id = l.takeFirst().toInt();
    _forumId = l.takeFirst().toInt();
    _parent = l.takeFirst().toString();
    _name = l.takeFirst().toString();
    _info = l.takeFirst().toString();
    _author = l.takeFirst().toString();
    _date = l.takeFirst().toDateTime();
    _icon = l.takeFirst().toString();
    _type = l.takeFirst().toInt();
    if (l.size() > 0) {
        _attach = l.takeFirst().toStringList();
    }
}
