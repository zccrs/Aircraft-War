#include "myplanes.h"
#include "windowplanes.h"
#include <QDebug>
#if(QT_VERSION<0x050000)
#include <QGraphicsSceneMouseEvent>
#endif

#if(QT_VERSION<0x050000)
MyPlanes::MyPlanes(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
#else
MyPlanes::MyPlanes(QQuickPaintedItem *parent) :
    QQuickPaintedItem(parent)
#endif
{
    count=0;//用来记录爆炸时播放到了第几张图片
    planesState=false;
#if(QT_VERSION<0x050000)
    setFlag(QGraphicsItem::ItemHasNoContents,false);
#endif
#ifdef MEEGO_EDITION_HARMATTAN
    double imageScaled=0.96;
#elif defined(Q_OS_SYMBIAN_V5)
    double imageScaled=0.72;
#elif defined(Q_OS_SAILFISH)
    double imageScaled=1;
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
    WindowPlanes* m_parent = qobject_cast<WindowPlanes*>(parent);
    connect(m_parent,SIGNAL(pause_launch_bullet()),SLOT(pause_me()));
    connect(m_parent,SIGNAL(resume_launch_bullet()),SLOT(resume_me()));
#if(QT_VERSION<0x050000)
    setZValue(2);
    setFlag(QGraphicsItem::ItemHasNoContents,false);
#else
    setZ(1);
#endif
}
#if(QT_VERSION<0x050000)
void MyPlanes::paint(QPainter *new_painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    new_painter->drawPixmap(0,0,*pixmap);
}
#else
void MyPlanes::paint(QPainter *painter)
{
    painter->drawPixmap(0,0,*pixmap);
}
#endif
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
    update();
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

void MyPlanes::pause_me()
{
    timer.stop ();
}

void MyPlanes::resume_me()
{
    timer.start ();
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
