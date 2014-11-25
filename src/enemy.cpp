#include "enemy.h"
#include <QDebug>
#include "windowplanes.h"
Enemy::Enemy(WindowPlanes *parent) :
    QDeclarativeItem(parent)
{
    mytype=1;
    count=0;
    speed=3500;

    setFlag(QGraphicsItem::ItemHasNoContents,false);
    animation.setPropertyName("pos");
    animation.setTargetObject(this);
    connect(&animation,SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)),this ,SLOT(anime_state(QAbstractAnimation::State,QAbstractAnimation::State)));
    connect(&mymovie,SIGNAL(timeout()),this,SLOT(movie()));
    connect(&flasher,SIGNAL(timeout()),SLOT(my_flasher()));


    connect(parent->mythread,SIGNAL(demandPos()),SLOT(callme()));
    connect(parent->mythread,SIGNAL(hitEnemy(int,int)),SLOT(isMe(int,int)));
    connect(this,SIGNAL(mypos(int,int,int,int,int,int)),parent->mythread,SLOT(receivtEnemyPos(int,int,int,int,int,int)));
    connect(this,SIGNAL(addScore(int)),parent,SLOT(setScore(int)));
    connect(parent,SIGNAL(pause_launch_bullet()),SLOT(pause_me()));
    connect(parent,SIGNAL(remove_all_bullet()),SLOT(deleteLater()));
    connect(parent,SIGNAL(resume_launch_bullet()),SLOT(resume_me()));
    connect(parent,SIGNAL(bomb_all_enemy()),SLOT(startMovie()));
    connect(this,SIGNAL(play_music(QString)),parent,SIGNAL(play_music(QString)));
}
void Enemy::paint(QPainter *new_painter, const QStyleOptionGraphicsItem *new_style, QWidget *new_widget)
{
    new_painter->drawPixmap(0,0,*pixmap);
}
void Enemy::anime_state(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    if(newState==QAbstractAnimation::Stopped)
        deleteLater();
}
void Enemy::go()
{
    //setSize(pixmap->size());
    animation.setDuration(speed);
    animation.setStartValue(QPoint(x(),-height()));
#ifdef MEEGO_EDITION_HARMATTAN
    animation.setEndValue(QPoint(x(),854));
#else
    animation.setEndValue(QPoint(x(),640));
#endif
    animation.start();
    vary();//用此函数判断是否是大飞机，是的话启动大飞机专有的动画
}
void Enemy::pause_me()
{
    if(animation.state()==QAbstractAnimation::Running)
        animation.pause();
}
void Enemy::resume_me()
{
    if(animation.state()==QAbstractAnimation::Paused)
        animation.resume();
}
void Enemy::setNumber(int x)
{
    number=x;
}
void Enemy::callme()
{
    if(HP>0||mytype==3)
        emit mypos(y(),y()+height(),x(),x()+width(),number,mytype);
}
void Enemy::isMe(int num, int type)//type是子弹的类型
{
    if(num==number&&HP>0)//如果是自己
    {
        HP-=type;
        if(HP<=0)//如果生命值小于1
        {
            pause_me();//停止动画
            mymovie.start(80);
            if(mytype==1)
                emit play_music("enemy1_down");
            else if(mytype==2)
                emit play_music("enemy2_down");
            else emit play_music("enemy3_down");
            emit addScore(score);
        }
        else
        {
            if(mytype!=3)
            {
                pixmap=blast2;
                //update(0,0,width(),height());
                QTimer::singleShot(50,this,SLOT(vary()));
            }
            else
            {
                flasher.stop();
                pixmap=blast3;

                //update(0,0,width(),height());
                QTimer::singleShot(50,this,SLOT(vary()));
            }
        }
    }
}

void Enemy::startMovie()
{
    pause_me();//停止动画
    mymovie.start(80);
    emit addScore(score);
}

void Enemy::vary()
{
    pixmap=blast1;

    //update(0,0,width(),height());
    if(mytype==3)
        flasher.start(50);
}

void Enemy::my_flasher()
{
    if(pixmap==blast1)
        pixmap=blast2;
    else
        pixmap=blast1;

    //update(0,0,width(),height());
}

void Enemy::movie()
{
    //qDebug()<<"Movie";
    if(mytype==1)
    {
        switch(count)
        {
        case 0:
            pixmap=blast2;
            break;
        case 1:
            pixmap=blast3;
            break;
        case 2:
            pixmap=blast4;
            break;
        case 3:
            pixmap=blast5;
            break;
        default:deleteLater();
        }
    }
    else if(mytype==2)
    {
        switch(count)
        {
        case 0:
            pixmap=blast3;
            break;
        case 1:
            pixmap=blast4;
            break;
        case 2:
            pixmap=blast5;
            break;
        case 3:
            pixmap=blast6;
            break;
        default:deleteLater();
        }
    }
    else if(mytype==3)
    {
        flasher.stop();
        switch(count)
        {
        case 0:
            pixmap=blast4;
            break;
        case 1:
            pixmap=blast5;
            break;
        case 2:
            pixmap=blast6;
            break;
        case 3:
            pixmap=blast7;
            break;
        case 4:
            pixmap=blast8;
            break;
        case 5:
            pixmap=blast9;
            break;
        default:deleteLater();
        }
    }
    //update(0,0,width(),height());
    count++;
}

Enemy::~Enemy()
{
    //animation.stop();
}
