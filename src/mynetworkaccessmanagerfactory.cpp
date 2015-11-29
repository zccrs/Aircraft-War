#include "mynetworkaccessmanagerfactory.h"
#include <QUrl>
#include <QDebug>

MyNetworkAccessManagerFactory::MyNetworkAccessManagerFactory(QObject *parent) :
    QObject(parent)
{
    m_networkManager = new NetworkAccessManager(this);
    connect(m_networkManager,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(onIgnoreSSLErrors(QNetworkReply*,QList<QSslError>)));
}

QNetworkAccessManager* MyNetworkAccessManagerFactory::create(QObject *parent)
{
    QMutexLocker lock(&mutex);
    Q_UNUSED(lock);
    QNetworkAccessManager* manager = new NetworkAccessManager(parent);

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
    QNetworkCookieJar* cookieJar = NetworkCookieJar::GetInstance();
    setCookieJar(cookieJar);
    cookieJar->setParent(0);
}

QNetworkReply *NetworkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QNetworkRequest req(request);
    QSslConfiguration config;

    config.setPeerVerifyMode(QSslSocket::VerifyNone);
#if(QT_VERSION>=0x050000)
    config.setProtocol(QSsl::TlsV1_0);
#else
    config.setProtocol(QSsl::TlsV1);
#endif
    req.setSslConfiguration(config);
    // set user-agent
    if (op == PostOperation){
        req.setRawHeader("User-Agent", "IDP");
    } else {
        req.setRawHeader("User-Agent", "Qt;Mozilla/5.0 (iPhone; CPU iPhone OS 7_0_4 like Mac OS X) AppleWebKit/537.51.1 (KHTML, like Gecko) Version/7.0 Mobile/11B554a Safari/9537.53");
    }
    req.setRawHeader("client", "true");

    QNetworkReply *reply = QNetworkAccessManager::createRequest(op, req, outgoingData);

    return reply;
}

NetworkCookieJar::NetworkCookieJar(QObject *parent) :
    QNetworkCookieJar(parent)
{
    //keepAliveCookie = QNetworkCookie("ka", "open");
    load ();
}

NetworkCookieJar::~NetworkCookieJar()
{
}

void NetworkCookieJar::load()
{
    QMutexLocker lock(&mutex);
    Q_UNUSED(lock);

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

QList<QNetworkCookie> NetworkCookieJar::cookies() const
{
    return allCookies ();
}
