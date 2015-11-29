#ifndef MYPLANES_H
#define MYPLANES_H

#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QString>
#include <QPainter>
#if(QT_VERSION>=0x050000)
#include <QQuickPaintedItem>
#else
#include <QDeclarativeItem>
#endif
#include "bullet.h"

#if(QT_VERSION<0x050000)
class MyPlanes : public QDeclarativeItem
#else
class MyPlanes : public QQuickPaintedItem
#endif
{
    Q_OBJECT
public:
#if(QT_VERSION<0x050000)
    explicit MyPlanes(QDeclarativeItem *parent = 0);
#else
    explicit MyPlanes(QQuickPaintedItem *parent = 0);
#endif
    ~MyPlanes();
    QImage image;
    QPixmap *pixmap,pixmap1,pixmap2,pixmap3,pixmap4,pixmap5;
    QTimer timer,mymovie;
    bool planesState;
#if(QT_VERSION>=0x050000)
    void paint(QPainter * painter);
#else
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *new_style, QWidget *new_widget=0);
#endif
signals:
    void play_music(const QString music_name);//播放音频
private:
    int count;
private slots:
    void movie();
    void pause_me();
    void resume_me();
public slots:
    void setPlanesState();
    void remove_me();//销毁自己
};

#endif // MYPLANES_H
