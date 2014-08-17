#include <QDebug>

#include "cookiejar.h"

CookieJar::CookieJar(QObject *parent) :
    QNetworkCookieJar(parent)
{
}

QVariant CookieJar::save()
{
    QVariantList vl;
    QList<QNetworkCookie> cl = allCookies();
    QListIterator<QNetworkCookie> it(cl);
    while(it.hasNext()) {
        vl.append(it.next().toRawForm());
    }
    return QVariant::fromValue(vl);
}

void CookieJar::load(const QVariant& c)
{
    QVariantList vl(c.toList());
    QList<QNetworkCookie> cl;
    QListIterator<QVariant> it(vl);
    while (it.hasNext()) {
        cl.append(QNetworkCookie::parseCookies(it.next().toByteArray()));
    }
    setAllCookies(cl);
}
