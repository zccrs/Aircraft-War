#include "prop.h"
#include "windowplanes.h"
#include "utility.h"

Prop::Prop(WindowPlanes *parent) :
#if(QT_VERSION<0x050000)
    QDeclarativeItem(parent)
#else
    QQuickPaintedItem(parent)
 #endif
{
    utility = Utility::createUtilityClass ();
    //haha=0;
    mytype=0;
    mystate=true;

#if(QT_VERSION<0x050000)
    setZValue(2);
    setFlag(QGraphicsItem::ItemHasNoContents,false);
    animation.setPropertyName("pos");
#else
    setZ(1);
    animation.setPropertyName("y");
#endif

    animation.setTargetObject(this);


    connect(parent,SIGNAL(pause_launch_bullet()),SLOT(pause_me()));
    connect(parent,SIGNAL(remove_all_bullet()),SLOT(deleteLater()));
    connect(parent,SIGNAL(resume_launch_bullet()),SLOT(resume_me()));
    connect(this,SIGNAL(mypos(int,int,int,int,int,int)),parent->mythread,SLOT(receivtProp(int,int,int,int,int,int)));
    connect(parent->mythread,SIGNAL(demandPos()),SLOT(callme()));
    connect(parent->mythread,SIGNAL(hitProp(int)),SLOT(isMe(int)));

    connect(&animation,SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)),SLOT(anime_state(QAbstractAnimation::State,QAbstractAnimation::State)));
}


void Prop::anime_state(QAbstractAnimation::State newState, QAbstractAnimation::State)
{
    if(newState==QAbstractAnimation::Stopped)
        deleteLater();
}
#if(QT_VERSION<0x050000)
void Prop::paint(QPainter *new_painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    new_painter->drawPixmap(0,0,*pixmap);
}
#else
void Prop::paint(QPainter *painter)
{
    painter->drawPixmap(0,0,*pixmap);
}
#endif
void Prop::go()
{
    //setSize(pixmap->size());
    animation.setDuration(1500);
    animation.setEasingCurve(QEasingCurve::InBack);
#ifdef MEEGO_EDITION_HARMATTAN
    animation.setStartValue(QPoint(x(),height()/2));
    animation.setEndValue(QPoint(x(),854));
#elif defined(Q_OS_SAILFISH)
    animation.setStartValue(height()/2);
    animation.setEndValue(utility->screenHeight ());
#else
    animation.setStartValue(QPoint(x(),height()/2));
    animation.setEndValue(QPoint(x(),640));
#endif

    //qDebug()<<"prop pos:"<<width()<<height()<<x()<<y();
    animation.start();
    //haha=1;
}
void Prop::pause_me()
{
    if(animation.state()==QAbstractAnimation::Running)
        animation.pause();
}
void Prop::resume_me()
{
    if(animation.state()==QAbstractAnimation::Paused)
        animation.resume();
}

void Prop::setNumber(int x)
{
    number=x;
}

void Prop::callme()
{
    //qDebug()<<"call prop ok";
    if(mystate)
        emit mypos(y(),y()+height(),x(),x()+width(),number,mytype);//发射自己的详细信息
}
void Prop::isMe(int num)
{
    if(num==number&&mystate)
    {
        qDebug()<<"is prop"<<number;
        mystate=false;
        deleteLater();//说明击中敌机的就是自己，否则忽略信号
    }
}

Prop::~Prop()
{
    //if(haha==0)
    //qDebug()<<"prop haha:"<<haha;
}
