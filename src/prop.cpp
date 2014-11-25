#include "prop.h"
#include "windowplanes.h"
//#include "myplanes.h"
Prop::Prop(WindowPlanes *parent) :
    QDeclarativeItem(parent)
{
    //haha=0;
    mytype=0;
    mystate=true;

    setZValue(2);
    setFlag(QGraphicsItem::ItemHasNoContents,false);
    animation.setPropertyName("pos");
    animation.setTargetObject(this);


    connect(parent,SIGNAL(pause_launch_bullet()),SLOT(pause_me()));
    connect(parent,SIGNAL(remove_all_bullet()),SLOT(deleteLater()));
    connect(parent,SIGNAL(resume_launch_bullet()),SLOT(resume_me()));
    connect(this,SIGNAL(mypos(int,int,int,int,int,int)),parent->mythread,SLOT(receivtProp(int,int,int,int,int,int)));
    connect(parent->mythread,SIGNAL(demandPos()),SLOT(callme()));
    connect(parent->mythread,SIGNAL(hitProp(int)),SLOT(isMe(int)));

    connect(&animation,SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)),SLOT(anime_state(QAbstractAnimation::State,QAbstractAnimation::State)));
}


void Prop::anime_state(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    if(newState==QAbstractAnimation::Stopped)
        deleteLater();
}
void Prop::paint(QPainter *new_painter, const QStyleOptionGraphicsItem *new_style, QWidget *new_widget)
{
    //new_painter->setRenderHints(QPainter::SmoothPixmapTransform,true);
    new_painter->drawPixmap(0,0,*pixmap);
}
void Prop::go()
{
    //setSize(pixmap->size());
    animation.setDuration(1500);
    animation.setEasingCurve(QEasingCurve::InBack);
    animation.setStartValue(QPoint(x(),height()/2));
#ifdef MEEGO_EDITION_HARMATTAN
    animation.setEndValue(QPoint(x(),854));
#else
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
