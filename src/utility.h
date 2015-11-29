#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QPoint>
#include <QSettings>
#include <QNetworkConfigurationManager>
#include <QPointer>
#if(QT_VERSION>=0x050000)
#include <QQmlApplicationEngine>
#else
#include <QDeclarativeEngine>
#include <QSystemNetworkInfo>
#include <QSystemDeviceInfo>
using namespace QtMobility;
#endif

class QTimer;
class UtilityPrivate : public QObject
{
    Q_OBJECT
    Q_ENUMS(ProxyType)
    Q_ENUMS(SystemType)
public:
    enum ProxyType{
        DefaultProxy,
        Socks5Proxy,
        NoProxy,
        HttpProxy,
        HttpCachingProxy,
        FtpCachingProxy
    };
};

class Utility : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString appVersion READ appVersion CONSTANT)
    Q_PROPERTY(QString sysName READ sysName CONSTANT)
    Q_PROPERTY(QString productName READ productName CONSTANT)
public:
    static Utility *createUtilityClass();
    
    int screenHeight();
    void setScreenSize(QSize size);
private:
    explicit Utility(QObject *parent = 0);
    ~Utility();

    static Utility *me;
    int screen_height;
#if(QT_VERSION>=0x050000)
    QPointer<QQmlEngine> engine;
#else
    QPointer<QDeclarativeEngine> engine;
#endif
    //QPoint old_pos;//记录鼠标上次的位置，判断鼠标位置是否改变
    //QTimer *mouse_timer;//检测鼠标位置是否变化的定时器
    QPointer<QSettings> mysettings;
    
    char numToStr(int num);
    //将数字按一定的规律换算成字母
    QByteArray strZoarium(const QByteArray &str);
    //按一定的规律加密字符串(只包含数字和字母的字符串)
    QByteArray unStrZoarium(const QByteArray &str);
    //按一定的规律解密字符串(只包含数字和字母的字符串)
    QByteArray fillContent(const QByteArray &str, int length);
    //将字符串填充到一定的长度
    int strToInt(QChar in);
private Q_SLOTS:
    //void emitDesktopPosChanged();
public:
    Q_INVOKABLE void consoleLog(QString str);//输出调试信息
    Q_INVOKABLE QString getCookie( QString cookieName );
#if(QT_VERSION>=0x050000)
    QQmlEngine *qmlEngine();
#else
    QDeclarativeEngine *qmlEngine();
#endif
    QString appVersion() const;


#if(QT_VERSION>=0x050000)
    void initUtility(QSettings *settings=0, QQmlEngine *qmlEngine=0);
    void setQmlEngine( QQmlEngine *new_engine );
#else
    void initUtility(QSettings *settings=0, QDeclarativeEngine *qmlEngine=0);
    void setQmlEngine( QDeclarativeEngine *new_engine );
#endif
    QString productName();
    QPoint mouseDesktopPos();
    void setQSettings(QSettings *settings);
    QString sysName() const;
    Q_INVOKABLE QByteArray base64Encode(QByteArray in);
    Q_INVOKABLE QByteArray base64Decode(QByteArray in);
    Q_INVOKABLE QString strToASCII(QString in);//
    Q_INVOKABLE void console(QString in);

Q_SIGNALS:
    //void mouseDesktopPosChanged(QPoint arg);
public Q_SLOTS:
    void setValue( const QString & key, const QVariant & value);
    QVariant value(const QString & key, const QVariant & defaultValue = QVariant()) const;
    void removeValue( const QString & key );
    void setNetworkProxy( int type, QString location, QString port, QString username, QString password );
    
    QString stringEncrypt(const QString &content, QString key);//加密任意字符串，中文请使用utf-8编码
    QString stringUncrypt(const QString &content_hex, QString key);//解密加密后的字符串
    
    void removePath(QString dirPath ,bool deleteHidden = true, bool deleteSelf = true );

    QString homePath() const;
};

#endif // UTILITY_H
