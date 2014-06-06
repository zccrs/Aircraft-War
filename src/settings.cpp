#include "settings.h"
#include <QSettings>
#include <QDebug>
Settings::Settings(QObject *parent) :
    QObject(parent)
{
    s= new QSettings(this);
}
Settings::~Settings ()
{
    s->deleteLater ();
}

void Settings::setValue (const QString &key, const QVariant &value)
{
    //qDebug()<<key<<value;
    s->setValue (key,value);
}
QVariant Settings::getValue (const QString &key, const QVariant defaultValue)
{
    //qDebug()<<key<<defaultValue<<s->value (key,defaultValue);
    return s->value (key,defaultValue);
}
