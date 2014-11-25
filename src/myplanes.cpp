#include "myplanes.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

MyPlanes::MyPlanes(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    count=0;//用来记录爆炸时播放到了第几张图片
    planesState=false;
    setFlag(QGraphicsItem::ItemHasNoContents,false);
#ifdef MEEGO_EDITION_HARMATTAN
    double imageScaled=0.96;
#elif defined(Q_OS_SYMBIAN_V5)
    double imageScaled=0.72;
#else
    double imageScaled=0.72;
    //double imageScaled=0.96;
#endif
    image.load(":/Image/F1_01.png");
    pixmap1=QPixmap::fromImage(image.scaled(imageScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F1_02.png");
    pixmap2=QPixmap::fromImage(image.scaled(imageScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F1_03.png");
    pixmap3=QPixmap::fromImage(image.scaled(imageScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F1_04.png");
    pixmap4=QPixmap::fromImage(image.scaled(imageScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F1_05.png");
    pixmap5=QPixmap::fromImage(image.scaled(imageScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    pixmap=&pixmap1;
    setSize(pixmap->size());

    connect(&timer,SIGNAL(timeout()),SLOT(setPlanesState()));
    connect(&mymovie,SIGNAL(timeout()),SLOT(movie()));
    setZValue(1);
    //setSmooth(true);
}
void MyPlanes::paint(QPainter *new_painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //new_painter->setRenderHints(QPainter::SmoothPixmapTransform,true);
    new_painter->drawPixmap(0,0,*pixmap);
}
void MyPlanes::setPlanesState()
{
    //qDebug()<<"setPlanesState";
    if(planesState)
    {
        pixmap=&pixmap1;
        planesState=false;
        //qDebug()<<"drops:"<<acceptDrops();
    }
    else
    {
        pixmap=&pixmap2;
        planesState=true;
    }
    //qDebug()<<"timer debug:"<<width()<<height();

    //update(0,0,width(),height());
}
void MyPlanes::movie()
{
    if(count==0)
        pixmap=&pixmap3;
    else if(count==1)
        pixmap=&pixmap4;
    else if(count==2)
        pixmap=&pixmap5;
    else if(count==3)
        pixmap=&pixmap1;
    else if(count==4)
        setVisible(false);
    else if(count==5)
        setVisible(true);
    else if(count==6)
        setVisible(false);
    else if(count==7)
        setVisible(true);
    else
    {
        setVisible(false);
        count=0;
        mymovie.stop();
    }
    count++;
    //update(0,0,width(),height());
}

void MyPlanes::remove_me()
{
    mymovie.start(80);
    emit play_music("game_over");
}
MyPlanes::~MyPlanes()
{
    //delete bullet_image;
}
