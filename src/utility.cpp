#include "utility.h"
#include "bullet.h"

Utility::Utility(QString appVersion, QObject *parent)
    :QObject(parent)
{
    m_appVersion = appVersion;
#if defined(Q_OS_SYMBIAN)||defined(HARMATTAN_BOOSTER)||defined(Q_WS_SIMULATOR)
    engine = new QDeclarativeEngine(this);
    component = new QDeclarativeComponent(engine,this);
    component->loadUrl( QUrl("qrc:/qml/general/DeviceInfo.qml") );
    QObject *obj = component->create();
    item = qobject_cast<QDeclarativeItem*>(obj);
    if(!item.isNull()){
        m_productName = item->property( "productName" ).toString();
    }
#else
    m_productName = QString::fromUtf8("电脑");
#endif
}

QByteArray Utility::base64Encode(QByteArray in)
{
    return in.toBase64();
}

QByteArray Utility::base64Decode(QByteArray in)
{
    return QByteArray::fromBase64(in);
}

QString Utility::strToASCII(QString in)
{
    //qDebug() << in;
    QString restr = "";

    int i=0;
    in = in.mid(2, in.count()-2);

    //qDebug() << in;
    while ( i<in.count()-1)
    {
        int temp = 16 * (strToInt(in.at(i))) + strToInt(in.at(i+1));
        restr.append((char)temp);
        i += 2;
    }
    return restr;
}

void Utility::console(QString in)
{
    qDebug() << in;
}

int Utility::strToInt(QChar in)
{
    if( in <='F' && in >= 'A' )
        return (int)in.toAscii()-'A'+10;
    else if( in <='f' && in>= 'a' )
        return (int)in.toAscii()-'a'+10;
    else if( in <='9' && in>= '0' )
        return (int)in.toAscii()-'0';
    else return 0;
}



QString Utility::productName()
{
    return m_productName;
}

QString Utility::appVersion()
{
    return m_appVersion;
}

QString Utility::sysName()
{
#if defined(Q_OS_SYMBIAN)||defined(HARMATTAN_BOOSTER)||defined(Q_WS_SIMULATOR)
#ifdef Q_OS_SYMBIAN_V5
    return "s60v5";
#else
    return "symbian3";
#endif
#elif defined(HARMATTAN_BOOSTER)
    return "meego";
#else
    return "pc";
#endif
}
