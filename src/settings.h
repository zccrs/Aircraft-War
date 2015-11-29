#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QVariant>
class QSettings;
class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    ~Settings();
    Q_INVOKABLE void setValue(const QString &key,const QVariant &value);
    Q_INVOKABLE QVariant getValue(const QString &key,const QVariant defaultValue=QVariant());
private:
    QSettings* s;
};

#endif // SETTINGS_H
