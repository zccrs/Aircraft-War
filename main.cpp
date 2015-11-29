#include <QGuiApplication>
#include <QNetworkProxy>
#include <QWebSettings>
#include <QDebug>
#include "src/mynetworkaccessmanagerfactory.h"
#include "src/settings.h"
#include "src/windowplanes.h"
#include "src/utility.h"
#include "src/myimage.h"
#if defined(Q_OS_SYMBIAN)||defined(MEEGO_EDITION_HARMATTAN)||defined(Q_WS_SIMULATOR)
#include "qmlapplicationviewer.h"
#include "audioplugin.h"
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QSysInfo>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QImage>
#include <QPixmap>
#elif defined(Q_OS_SAILFISH)
#include <QtQuick>
#include <sailfishapp.h>
#include "src/myhttprequest.h"
#include "mydes.h"
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
#ifdef Q_OS_SAILFISH
    QGuiApplication *app = new QGuiApplication(argc, argv);
#else
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    GE::AudioPlugin plugin;
    plugin.registerTypes("GEAudio");
#endif
#ifdef Q_OS_SAILFISH//这是最重要的，不然商店都别想审核通过
    app->setApplicationName ("harbour-aircraftwar");
    app->setOrganizationName ("harbour-aircraftwar");
#else
    app->setApplicationName ("AircraftWar");
    app->setOrganizationName ("Star");
#endif
    app->setApplicationVersion ("2.1.0");
    QWebSettings::globalSettings()->setUserStyleSheetUrl(QUrl::fromLocalFile(":/i18n/comment.css"));
    QWebSettings::globalSettings()->setFontSize(QWebSettings::MinimumFontSize, 24);

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
    qmlRegisterType<MyImage> ("com.star.widget", 1, 0, "MyImage");
    qmlRegisterType<WindowPlanes> ("planes", 1, 0, "Myplanes");

#if defined(Q_OS_SYMBIAN)||defined(MEEGO_EDITION_HARMATTAN)||defined(Q_WS_SIMULATOR)
    QmlApplicationViewer *viewer = new QmlApplicationViewer();
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);// 锁定为竖屏
#elif defined(Q_OS_SAILFISH)
    QQuickView *viewer = new QQuickView();
    QObject::connect(viewer->engine(), SIGNAL(quit()), app, SLOT(quit()));
    viewer->engine ()->rootContext ()->setContextProperty ("httpRequest", new MyHttpRequest());
    viewer->engine ()->rootContext ()->setContextProperty ("mydes", new MyDes());
#elif defined(Q_OS_LINUX)
    QmlApplicationViewer *viewer = new QmlApplicationViewer();
#else
    MyPos mypos(&setting);
    QmlApplicationViewer viewer(&mypos);
    mypos.init(&viewer);
#endif

    Settings setting;
    MyNetworkAccessManagerFactory network;

    Utility *utility = Utility::createUtilityClass();
    utility->setScreenSize (viewer->screen ()->size ());
    viewer->engine()->setNetworkAccessManagerFactory(&network);
    viewer->rootContext()->setContextProperty("mysettings", &setting);
    viewer->rootContext()->setContextProperty("utility", utility);
#if defined(Q_OS_SYMBIAN)||defined(Q_WS_SIMULATOR)
    viewer->setSource(QUrl("qrc:/qml/symbian3/main.qml"));
    viewer->showExpanded();
#elif defined(MEEGO_EDITION_HARMATTAN)
    viewer->setSource(QUrl("qrc:/qml/meego/main.qml"));
    viewer->showExpanded();
#elif defined(Q_OS_SAILFISH)
    viewer->setSource(QUrl("qrc:/sailfish_qml/main.qml"));
    viewer->showFullScreen();
#elif defined(Q_OS_LINUX)
    viewer->setSource(QUrl("qrc:/qml/symbian3/main.qml"));
    viewer->setFixedSize(viewer->width(),viewer->height());
    viewer->move(width/2-viewer->width()/2,height/2-viewer->height()/2);
    viewer->show();
#else
    viewer->setSource(QUrl("qrc:/qml/symbian3/main.qml"));
    mypos.setWindowFlags(Qt::FramelessWindowHint);
    mypos.setAttribute(Qt::WA_TranslucentBackground);
    viewer->setWindowFlags(Qt::FramelessWindowHint);
    viewer->setAttribute(Qt::WA_TranslucentBackground);

    viewer->move(QPoint(mypos.width()/2-viewer->width()/2,mypos.height()/2-viewer->height()/2));
    mypos.show();
#endif
#if defined(Q_OS_SYMBIAN)||defined(Q_WS_SIMULATOR)
    splash->finish(viewer);
    splash->deleteLater();
#endif
    return app->exec();
}
