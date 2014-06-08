#include "mynetworkaccessmanagerfactory.h"
#include <QUrl>

MyNetworkAccessManagerFactory::MyNetworkAccessManagerFactory(QObject *parent) :
    QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(onIgnoreSSLErrors(QNetworkReply*,QList<QSslError>)));
}

QNetworkAccessManager* MyNetworkAccessManagerFactory::create(QObject *parent)
{
    QMutexLocker lock(&mutex);
    Q_UNUSED(lock);
    QNetworkAccessManager* manager = new NetworkAccessManager(parent);

#ifdef Q_OS_SYMBIAN
    bool useDiskCache = Utility::Instance()->qtVersion() >= 0x040800;
#else
    bool useDiskCache = true;
#endif
    if (useDiskCache){
        QNetworkDiskCache* diskCache = new QNetworkDiskCache(parent);
        QString dataPath = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
        QDir dir(dataPath);
        if (!dir.exists()) dir.mkpath(dir.absolutePath());

        diskCache->setCacheDirectory(dataPath);
        diskCache->setMaximumCacheSize(3*1024*1024);
        manager->setCache(diskCache);
    }

    QNetworkCookieJar* cookieJar = NetworkCookieJar::GetInstance();
    manager->setCookieJar(cookieJar);
    cookieJar->setParent(0);

    return manager;
}

void MyNetworkAccessManagerFactory::onIgnoreSSLErrors(QNetworkReply *reply, QList<QSslError> error)
{
    qDebug()<<error;
    reply->ignoreSslErrors(error);
}

NetworkAccessManager::NetworkAccessManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
}

QNetworkReply *NetworkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QNetworkRequest req(request);

    QSslConfiguration config;

    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1);
    req.setSslConfiguration(config);
    // set user-agent
    if (op == PostOperation){
        req.setRawHeader("User-Agent", "IDP");
    } else {
        req.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 7_0_4 like Mac OS X) AppleWebKit/537.51.1 (KHTML, like Gecko) Version/7.0 Mobile/11B554a Safari/9537.53");
    }

    QNetworkReply *reply = QNetworkAccessManager::createRequest(op, req, outgoingData);
    return reply;
}

NetworkCookieJar::NetworkCookieJar(QObject *parent) :
    QNetworkCookieJar(parent)
{
    keepAliveCookie = QNetworkCookie("ka", "open");
}

NetworkCookieJar::~NetworkCookieJar()
{
}

NetworkCookieJar* NetworkCookieJar::GetInstance()
{
    static NetworkCookieJar cookieJar;
    return &cookieJar;
}

void NetworkCookieJar::clearCookies()
{
    QList<QNetworkCookie> emptyList;
    setAllCookies(emptyList);
}

QList<QNetworkCookie> NetworkCookieJar::cookiesForUrl(const QUrl &url) const
{
    QMutexLocker lock(&mutex);
    Q_UNUSED(lock);
    QList<QNetworkCookie> cookies = QNetworkCookieJar::cookiesForUrl(url);
    return cookies;
}

bool NetworkCookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url)
{
    QMutexLocker lock(&mutex);
    Q_UNUSED(lock);
    return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

