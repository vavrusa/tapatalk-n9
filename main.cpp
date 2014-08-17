#include <QtGui/QApplication>
#include <QDeclarativeItem>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include "qmlapplicationviewer.h"
#include "forumlistmodel.h"
#include "tapatalkmgr.h"
#include "namfactory.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Marek Vavrusa");
    QCoreApplication::setOrganizationDomain("marek.vavrusa.com");
    QCoreApplication::setApplicationName("Tapatalk");
        
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    app->setApplicationName("Tapatalk");
    
    QmlApplicationViewer viewer;
    TapatalkMgr mgr(&viewer);
    mgr.activate();
    
    qmlRegisterType<ForumListModel>("listmodel.pkg", 1, 0, "ForumListModel");
    viewer.rootContext()->setContextProperty("tap", &mgr );
    viewer.setMainQmlFile(QLatin1String("qml/Tapatalk/main.qml"));
    viewer.engine()->setNetworkAccessManagerFactory(new NAMFactory);
    viewer.showFullScreen();

    return app->exec();
}
