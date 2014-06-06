#ifndef MYPLANES_H
#define MYPLANES_H

#include <QDeclarativeItem>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QString>
#include <QPainter>
#include "bullet.h"
class MyPlanes : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit MyPlanes(QDeclarativeItem *parent = 0);
    ~MyPlanes();
    QImage image;
    QPixmap *pixmap,pixmap1,pixmap2,pixmap3,pixmap4,pixmap5;
    QTimer timer,mymovie;
    bool planesState;
    void paint(QPainter *new_painter, const QStyleOptionGraphicsItem *new_style, QWidget *new_widget=0);
signals:
    void play_music(const QString music_name);//播放音频
private:
    int count;
private slots:
    void movie();
public slots:
    void setPlanesState();
    void remove_me();//销毁自己
};

#endif // MYPLANES_H
