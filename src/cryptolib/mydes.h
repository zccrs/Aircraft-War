#ifndef MYDES_H
#define MYDES_H

#include <QObject>
#include "yxyDES2.h"

class MyDes : public QObject
{
    Q_OBJECT
public:
    explicit MyDes(QObject *parent = 0);
    ~MyDes();

signals:

public slots:
    QByteArray encrypt(QByteArray key, QByteArray data);
    QByteArray decrypt(QByteArray key, QByteArray data);

    QByteArray encryptAndToBase64(QByteArray key, QByteArray data);
    QByteArray decryptByBase64(QByteArray key, QByteArray data);

    QByteArray encryptAndToHex(QByteArray key, QByteArray data);
    QByteArray decryptByHex(QByteArray key, QByteArray data);
private:
    yxyDES2 des;
};

#endif // MYDES_H
