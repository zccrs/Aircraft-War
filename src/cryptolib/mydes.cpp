#include "mydes.h"
#include <QDebug>

MyDes::MyDes(QObject *parent) : QObject(parent)
{

}

MyDes::~MyDes()
{

}

QByteArray MyDes::encrypt(QByteArray key, QByteArray data)
{
    des.InitializeKey (key.data (), 0);
    des.EncryptAnyLength (data.data (), data.length (), 0);

    return QByteArray(des.GetCiphertextAnyLength());
}

QByteArray MyDes::decrypt(QByteArray key, QByteArray data)
{
    des.InitializeKey (key.data (), 0);
    des.DecryptAnyLength (data.data (), data.length (), 0);
    return QByteArray(des.GetPlaintextAnyLength());
}

QByteArray MyDes::encryptAndToBase64(QByteArray key, QByteArray data)
{
    return encrypt (key, data).toBase64 ();
}

QByteArray MyDes::decryptByBase64(QByteArray key, QByteArray data)
{
    return decrypt (key, QByteArray::fromBase64 (data));
}

QByteArray MyDes::encryptAndToHex(QByteArray key, QByteArray data)
{
    return encrypt (key, data).toHex ();
}

QByteArray MyDes::decryptByHex(QByteArray key, QByteArray data)
{
    return decrypt (key, QByteArray::fromHex (data));
}

