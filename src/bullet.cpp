#include "bullet.h"
#include "windowplanes.h"
#include <QDebug>

Bullet::Bullet(WindowPlanes *parent) :
#if(QT_VERSION<0x050000)
    QDeclarativeItem(parent)
#else
    QQuickPaintedItem(parent)
#endif
{
    mystate=true;
#if(QT_VERSION<0x050000)
    setFlag(QGraphicsItem::ItemHasNoContents,false);
    animation.setPropertyName("pos");
#else
    animation.setPropertyName("y");
#endif

    animation.setTargetObject(this);

    connect(parent->mythread,SIGNAL(demandPos()),SLOT(callme()));
    connect(parent->mythread,SIGNAL(crashBullet(int)),SLOT(isMe(int)));
    connect(this,SIGNAL(mypos(int,int,int,int,int,int)),parent->mythread,SLOT(receivtBulletPos(int,int,int,int,int,int)));
    connect(parent,SIGNAL(pause_launch_bullet()),SLOT(pause_me()));
    connect(parent,SIGNAL(remove_all_bullet()),SLOT(deleteLater()));
    connect(parent,SIGNAL(resume_launch_bullet()),SLOT(resume_me()));

    connect(&animation,SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)),this ,SLOT(anime_state(QAbstractAnimation::State,QAbstractAnimation::State)));
}
#if(QT_VERSION<0x050000)
void Bullet::paint(QPainter *new_painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    new_painter->drawPixmap(0,0,*pixmap);
}
#else
void Bullet::paint(QPainter *painter)
{
    painter->drawPixmap(0,0,*pixmap);
}
#endif
void Bullet::anime_state(QAbstractAnimation::State newState, QAbstractAnimation::State)
{
    if(newState==QAbstractAnimation::Stopped)
        deleteLater();
}
void Bullet::go()
{
    //setSize(pixmap->size());
#ifdef MEEGO_EDITION_HARMATTAN
    if( y()<46 )
            deleteLater();
    animation.setDuration(450*y()/688.75);
    animation.setEndValue(QPoint(x(),45));
    animation.setStartValue(QPoint(x(), y() ));
#elif defined(Q_OS_SAILFISH)
    if( y()<46 ){
        deleteLater();
        return;
    }
    animation.setDuration(450*y()/688.75);
    animation.setStartValue(y());
    animation.setEndValue(45);
#else
    if( y()<31 )
            deleteLater();
    animation.setDuration(350*y()/519.8);
    animation.setEndValue(QPoint(x(),30));
    animation.setStartValue(QPoint(x(), y() ));
#endif
    animation.start();
}
void Bullet::pause_me()
{
    if(animation.state()==QAbstractAnimation::Running)
        animation.pause();
}
void Bullet::resume_me()
{
    if(animation.state()==QAbstractAnimation::Paused)
        animation.resume();
}
void Bullet::setNumber(int x)
{
    number=x;
}
void Bullet::callme()
{
    emit mypos(y(),y()+height(),x(),x()+width(),number,mytype);//发射自己的详细信息
}
void Bullet::isMe(int num)
{
    if(num==number)
        deleteLater();//说明击中敌机的就是自己，否则的忽略信号
}
