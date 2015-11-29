#include "mythread.h"
#include "windowplanes.h"
#include "myplanes.h"
#include "math.h"
#include <QDebug>

MyThread::MyThread(WindowPlanes *win,QObject *parent) :
    QObject(parent)
{
    count_bullet=count_enemy=count_prop=0;
    planes=win->planes;
//    connect(&timer,SIGNAL(timeout()),this,SLOT(begin()));
}
void MyThread::isBulletAndEnemy()//判断子弹是否打中敌方飞机的函数
{
    //qDebug()<<"count:"<<count_bullet<<count_enemy;
    for(int i=0;i<count_bullet;i++)//从第一个子弹开始循环
        for(int j=0;j<count_enemy;j++)//从第一个飞机开始循环
        {//下面的if是判断是否碰撞的
            if(bullet[i][0]<enemy[j][1]&&bullet[i][1]>enemy[j][0]&&bullet[i][2]<=enemy[j][3]&&bullet[i][3]>=enemy[j][2])
            {//如果子弹打中了飞机
                emit hitEnemy(enemy[j][4],bullet[i][5]);//发射信号告诉被打中飞机enemy[j][4]是飞机的编号，飞机用来判断是不是自己
                emit crashBullet(bullet[i][4]);//bullet[i][4]是子弹的编号，功能同上         //bullet[i][5]是子弹的类型
                i=count_bullet+1;//结束外层循环
                break;//结束本层循环
            }
        }
}
void MyThread::isPlanesAndEnemyOrProp()
{
    double l=planes->x()+0.1*planes->width();//飞机的左边界
    double u=planes->y()+planes->height()*0.2;//飞机的上边界
    double d=u+planes->height()*0.8;//飞机的下边界
    double r=planes->x()+0.9*planes->width();//飞机的右边界
    for(int i=0;i<count_prop;i++)//从第一个道具循环
    {
        if(u<prop[i][1]&&d>prop[i][0]&&l<prop[i][3]&&r>prop[i][2])
        {//如果飞机撞到了道具
            //qDebug()<<"prop hit planes";
            emit hitProp(prop[i][4]);//prop[i][4]是道具的编号功能同上
            if(prop[i][5]==0)//1是双倍子弹。0是炸弹
                emit captureDoubleBullet();
            else
                emit captureBomb();
            break;
        }
    }
    for(int i=0;i<count_enemy;i++)
    {
        if(u<enemy[i][1]&&d>enemy[i][0]&&l<enemy[i][3]&&r>enemy[i][2])
        {
            emit game_over();//，如果飞机撞到敌机发射此信号告诉飞机
            break;
        }
    }
    //qDebug()<<"isPlanesAndEnemyOrProp end";
}
void MyThread::begin()//被定时器调用的函数
{
    //qDebug()<<"thread count:"<<count_bullet<<"count_enemy"<<count_enemy<<"count_prop"<<count_prop;
    //qDebug()<<"count:"<<count_bullet<<count_enemy;
    isPlanesAndEnemyOrProp();//根据接收到的数据判断飞机是否撞到敌机或者吃到道具
    isBulletAndEnemy();//根据数据判断子弹是否打中飞机
    count_bullet=count_enemy=count_prop=0;//初始化计数器
    emit demandPos();//发射信号向飞机和子弹索要信息
}

void MyThread::start()
{
    qDebug()<<QThread::currentThread();
    //qDebug()<<timer.isActive();
    count_bullet=count_enemy=count_prop=0;//初始化计数器
    if (timer.isNull()){
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(begin()));
    }
    timer->start(15);//启动计时器
    qDebug()<<timer->isActive();
}

void MyThread::stop()//停止计时器
{
    if (timer){
        timer->stop();
    }
}
void MyThread::receivtBulletPos(int u, int d, int l, int r, int number, int bullettype)//储存接收到的子弹的信息
{
    if(bullettype==1)
    {
        bullet[count_bullet][0]=u;
        bullet[count_bullet][1]=d;
        bullet[count_bullet][2]=l+2*(r-l)/8;
        bullet[count_bullet][3]=r-2*(r-l)/8;
        //qDebug()<<QString::fromUtf8("子弹的宽度：")<<bullet[count_bullet][3]-bullet[count_bullet][2];
    }
    else
    {
        bullet[count_bullet][0]=u;
        bullet[count_bullet][1]=d;
        bullet[count_bullet][2]=l;
        bullet[count_bullet][3]=r;
    }

    bullet[count_bullet][4]=number;
    bullet[count_bullet][5]=bullettype;
    count_bullet++;
    if(count_bullet>4)
        count_bullet=4;
    //qDebug()<<count_bullet;
}
void MyThread::receivtEnemyPos(int u, int d, int l, int r, int number,int enemytype)//储存接收到的敌机的信息
{
    if(enemytype==1)
    {
        enemy[count_enemy][0]=u+(d-u)/3;
        enemy[count_enemy][1]=d-(d-u)/4;
        enemy[count_enemy][2]=l+(r-l)/4;
        enemy[count_enemy][3]=r-(r-l)/4;
    }
    else if(enemytype==2)
    {
        enemy[count_enemy][0]=u+(d-u)/8;
        enemy[count_enemy][1]=d-(d-u)/8;
        enemy[count_enemy][2]=l+(r-l)/6;
        enemy[count_enemy][3]=r-(r-l)/6;
    }
    else if(enemytype==3)
    {
        enemy[count_enemy][0]=u+(d-u)/4;
        enemy[count_enemy][1]=d-(d-u)/4;
        enemy[count_enemy][2]=l+7*(r-l)/30;
        enemy[count_enemy][3]=r-7*(r-l)/30;
    }
    else qDebug()<<"enemy type error";

    //qDebug()<<enemytype;



    enemy[count_enemy][4]=number;
    count_enemy++;
    if(count_enemy>49)
        count_enemy=49;
    //qDebug()<<count_enemy;
}

void MyThread::receivtProp(int u, int d, int l, int r, int number, int proptype)//储存接收到的道具的信息
{
    //qDebug()<<"prop pos:"<<u<<d<<l<<r<<number<<proptype;
    prop[count_prop][0]=u;
    prop[count_prop][1]=d;
    prop[count_prop][2]=l;
    prop[count_prop][3]=r;
    prop[count_prop][4]=number;
    prop[count_prop][5]=proptype;
    count_prop++;
    if(count_prop>4)
        count_prop=4;
}

MyThread::~MyThread()
{
    if (timer){
        timer->deleteLater();
    }
}
