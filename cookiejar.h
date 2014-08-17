#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QNetworkCookieJar>
#include <QVariant>

class CookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    explicit CookieJar(QObject *parent = 0);
    QVariant save();
    void load(const QVariant& c);
    
signals:
    
public slots:
    
};

#endif // COOKIEJAR_H
