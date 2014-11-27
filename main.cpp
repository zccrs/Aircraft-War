#include <QApplication>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QNetworkProxy>
#include <QWebSettings>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QSysInfo>
#include "src/mynetworkaccessmanagerfactory.h"
#include "qmlapplicationviewer.h"
#include "src/settings.h"
#include "src/windowplanes.h"
#include "src/utility.h"
#include "audioplugin.h"
#include "src/myimage.h"
#include "qmlapplicationviewer.h"

#if defined(Q_OS_SYMBIAN)||defined(MEEGO_EDITION_HARMATTAN)||defined(Q_WS_SIMULATOR)
#else
#include "src/mypos.h"
#endif


Q_DECL_EXPORT int main(int argc, char *argv[])
{
#ifdef Q_OS_SYMBIAN_V5
    QSysInfo::S60Version temp = QSysInfo::s60Version();
    if ( temp != QSysInfo::SV_S60_3_1 && temp != QSysInfo::SV_S60_3_2 && temp!= QSysInfo::SV_S60_5_0)
        return 0;//如果非s60 v5的机型使用此版本就退出
#endif

    QScopedPointer<QApplication> app(createApplication(argc, argv));
    app->setApplicationName ("AircraftWar");
    app->setOrganizationName ("Stars");
    app->setApplicationVersion ("2.1.0");

    QWebSettings::globalSettings()->setUserStyleSheetUrl(QUrl::fromLocalFile(":/i18n/comment.css"));

    //int width=QApplication::desktop()->width();
    //int height=QApplication::desktop()->height();
#if defined(Q_WS_SIMULATOR)
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("localhost");
    proxy.setPort(8888);
    QNetworkProxy::setApplicationProxy(proxy);
#endif

#if defined(Q_OS_SYMBIAN)||defined(Q_WS_SIMULATOR)
    int width=QApplication::desktop()->width();

    QString splash_path;
    if(width == 640)
        splash_path = ":/Image/Symbian640.png";
    else
        splash_path = ":/Image/Symbian.png";
    QPixmap pixmap(splash_path);
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();
    splash->raise();
#endif

    Settings setting;

    qmlRegisterType<WindowPlanes> ("planes",1,0,"Myplanes");
    qmlRegisterType<MyImage> ("com.star.widget",1,0,"MyImage");

#if defined(Q_OS_SYMBIAN)||defined(MEEGO_EDITION_HARMATTAN)||defined(Q_WS_SIMULATOR)

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);// 锁定为竖屏

#elif defined(Q_OS_LINUX)
    QmlApplicationViewer viewer;
#else
    MyPos mypos(&setting);
    QmlApplicationViewer viewer(&mypos);
    mypos.init(&viewer);

#endif
    MyNetworkAccessManagerFactory *network = new MyNetworkAccessManagerFactory();
    viewer.engine()->setNetworkAccessManagerFactory(network);

    Utility utility;
    viewer.rootContext()->setContextProperty("mysettings",&setting);
    viewer.rootContext()->setContextProperty("utility",&utility);

    GE::AudioPlugin plugin;
    plugin.registerTypes("GEAudio");
#if defined(Q_OS_SYMBIAN)||defined(Q_WS_SIMULATOR)
    viewer.setSource(QUrl("qrc:/qml/symbian3/main.qml"));
    viewer.showExpanded();
#elif defined(MEEGO_EDITION_HARMATTAN)
    viewer.setSource(QUrl("qrc:/qml/meego/main.qml"));
    viewer.showExpanded();
#elif defined(Q_OS_LINUX)
    viewer.setSource(QUrl("qrc:/qml/symbian3/main.qml"));
    viewer.setFixedSize(viewer.width(),viewer.height());
    viewer.move(width/2-viewer.width()/2,height/2-viewer.height()/2);
    viewer.show();
#else
    viewer.setSource(QUrl("qrc:/qml/symbian3/main.qml"));
    mypos.setWindowFlags(Qt::FramelessWindowHint);
    mypos.setAttribute(Qt::WA_TranslucentBackground);
    viewer.setWindowFlags(Qt::FramelessWindowHint);
    viewer.setAttribute(Qt::WA_TranslucentBackground);

    viewer.move(QPoint(mypos.width()/2-viewer.width()/2,mypos.height()/2-viewer.height()/2));
    mypos.show();
#endif
#if defined(Q_OS_SYMBIAN)||defined(Q_WS_SIMULATOR)
    splash->finish(&viewer);
    splash->deleteLater();
#endif
    return app->exec();
}
