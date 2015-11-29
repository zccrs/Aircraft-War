#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QTimer>
#include <QPointer>

class WindowPlanes;
class MyPlanes;
class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(WindowPlanes *win = 0,QObject *parent = 0);
    ~MyThread();

    MyPlanes *planes;//指向父对象中的f1战机的指针，此指针在程序启动时创建，不会销毁
signals:
    void demandPos();//定时发射此信号，子弹或者飞机或者道具收到这个信号后会发射一个信号，里面包含自己的位置和大小、类型和编号等信息
    void game_over();//如果判断到飞机撞到敌机就发射这个信号，主线程中收到信号后就做出相应的反应
    void hitEnemy(int number,int type);//如果判断到飞机被子弹击中，就发射这个飞机的编号以及子弹的类型（根据子弹的类型来减少自己的生命）
    void crashBullet(int number);//如果一个子弹已经击中飞机，发射这个子弹的编号，相应编号的子弹接收到这个信号之后就销毁自己

    void hitProp(int num);//如果一个道具被飞机吃掉就发射这个信号，相对应编号的道具收到信号后就会销毁自己

    void captureDoubleBullet();//判断到飞机吃到双倍子弹道具时发射
    void captureBomb();//判断到飞机吃到炸弹道具时发射
private:
    int bullet[5][6],count_bullet;//用来储存获取到的子弹信息，0-5分别是上下左右位置和编号，和子弹类型，coune_bullet是计数的
    int enemy[50][5],count_enemy;//用来储存获取到的敌机信息，0-4分别是上下左右位置和飞机编号，coune_enemy是计数的
    int prop[5][6],count_prop;//用来储存获取到的道具信息，0-1分别是编号和类型,用来储存道具的信息，coune_prop是计数的
    QPointer<QTimer> timer;//定时器用来定时运行begin函数
    void isBulletAndEnemy();//用来判断是否有子弹打中飞机
    void isPlanesAndEnemyOrProp();//用来判断f1战机是否撞到敌机或者吃到道具
private slots:
    void begin();//这个类的主要函数，用来发射demandPos()和调用isBulletAndEnemy() isPlanesAndEnemyOrProp()
public slots:
    void start();//在主线程中调用他就启动本线程中的定时器
    void stop();//在主线程中调用他来停止本线程中的定时器

    void receivtBulletPos(int u,int d,int l,int r,int number,int bullettype);//接收子弹的信息
    void receivtEnemyPos(int u,int d,int l,int r,int number,int enemytype);//接收飞机的信息
    void receivtProp(int u,int d,int l,int r,int number,int proptype);//接收道具的信息
};

#endif // MYTHREAD_H
