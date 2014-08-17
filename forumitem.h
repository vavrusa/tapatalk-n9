#ifndef FORUMITEM_H
#define FORUMITEM_H

#include <QVariantList>
#include <QStringList>
#include <QString>
#include <QDateTime>

class ForumItem
{
    public:
    ForumItem(int id);
    
    enum Type
    {
        Forum = 2,
        Topic = 3
    };
    
    int id() const {
        return _id;
    }
    
    int forumId() const { return _forumId; }
    void setForumId(int id) { _forumId = id; }

    void setParent(const QString& s) {
        _parent = s;
    }

    const QString& parent() const {
        return _parent;
    }
    const QString& name() const {
        return _name;
    }
    void setName(const QString& s) {
        _name = s;
    }

    const QString& info() const {
        return _info;
    }
    void setInfo(const QString& s) {
        _info = s;
    }
    
    void parseInfo(QString s);
    
    const QString& author() const {
        return _author;
    }
    
    void setAuthor(const QString&s) {
        _author = s;
    }
    
    QString date() const {
        return _date.toString(Qt::SystemLocaleShortDate);
    }
    
    void setDate(const QDateTime& s) {
        _date = s;
    }
    
    const QString& icon() const {
        return _icon;
    }
    
    const QStringList& attachments() const {
        return _attach;
    }
    
    void setAttachments(const QStringList& l) {
        _attach = l;
    }
    
    void addAttachment(const QString& a) {
        _attach.append(a);
    }
    
    void setIcon(const QString& s) {
        _icon = s;
    }

    void setType(unsigned p) {
        _type = p;
    }

    unsigned type() const {
        return _type;
    }
    
    QVariantList serialize() const;
    void unserialize(QVariantList l);
    
    private:
    int _id;
    int _forumId;
    QString _parent;
    QString _name;
    QString _info;
    QString _author;
    QDateTime _date;
    QString _icon;
    QStringList _attach;
    unsigned _type;
};

#endif // FORUMITEM_H
