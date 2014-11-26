
#ifndef WINDOWPLANES_H
#define WINDOWPLANES_H

#include <QDeclarativeItem>
#include <QTimer>
#include <QTime>
#include <QImage>
#include <QPixmap>
#include <QThread>
#include <QPointer>
#include "myplanes.h"
#include "bullet.h"
#include "enemy.h"
#include "mythread.h"
#include "prop.h"
class WindowPlanes : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(qreal planeWidth READ planeWidth CONSTANT)
    Q_PROPERTY(qreal planeHeight READ planeHeight CONSTANT)
    Q_PROPERTY(int gameRuningTime READ gameRuningTime CONSTANT)

public:
    explicit WindowPlanes(QDeclarativeItem *parent = 0);
    ~WindowPlanes();

    MyPlanes *planes;

    QPointer<QThread> thread;
    QPointer<MyThread> mythread;

signals:
    void play_music(const QString music_name);//发射信号，播放名叫music_name的音乐，此信号将在qml中接收并调用music.qml中相关函
    void pause_launch_bullet();//当用户点击暂停键后发射此信号，将会调用所有的飞机和子弹中的pause_me()这个函数来暂停飞机和子弹的移
    void remove_all_bullet();//当用户点击退出游戏后发射此信号，将会调用所有的飞机和子弹中的deleteLxxx()这个函数来销毁飞机和子弹的这个对
    void resume_launch_bullet();//当用户点击继续游戏后发射此信号，将会调用所有的飞机和子弹中的resume_me()这个函数来恢复自身的动作
    void addScore(int num);//当一个飞机爆炸后发射信号给qml、，将在qml中更新游戏得分，score是打死这个飞机应得的分数
    void game_over();//f1战机死亡时发射此信号，告诉qml住界面游戏结束了，让qml做出相应的反
    void bomb_all_enemy();//当用户点击屏幕坐下角的炸弹道具后发射此信号，告诉所有的飞机和子弹销毁自

    void acquireBomb();//当飞机吃到炸弹后发射此信号，将在qml中接收，并在右下角显示这个炸
// Yeatse:
    void myThreadStart();
    void myThreadStop();
private:
    QTimer timer_bullet,timer_prop,timer_bullet_to_single,timer_two_secs;
    QTimer timer_enemy1_1,timer_enemy1_2,timer_enemy1_3;
    QTimer timer_enemy2_1,timer_enemy2_2,timer_enemy2_3;
    QTimer timer_enemy3_1,timer_enemy3_2,timer_enemy3_3;
    QTime time,time_double_bullet_begin,time_no_enemy_begin;//后两个变量分别用来记录双子弹和无敌机的状态已经持续的时间
    QString game_state;
    QTime time_game_start_runing;//记录游戏开始了多久

    QImage image;
    QPixmap *pixmap_bullet,pixmap_bullet1,pixmap_bullet2,pixmap_prop0,pixmap_prop1;
    QPixmap pixmap_enemy1_1,pixmap_enemy1_2,pixmap_enemy1_3,pixmap_enemy1_4,pixmap_enemy1_5;
    QPixmap pixmap_enemy2_1,pixmap_enemy2_2,pixmap_enemy2_3,pixmap_enemy2_4,pixmap_enemy2_5,pixmap_enemy2_6;
    QPixmap pixmap_enemy3_1,pixmap_enemy3_2,pixmap_enemy3_3,pixmap_enemy3_4,pixmap_enemy3_5,pixmap_enemy3_6,pixmap_enemy3_7,pixmap_enemy3_8,pixmap_enemy3_9;

    int number;//为每一个用new创建的飞机或者子弹一个唯一的编号，这是他们身份的id
    int bullet_type;//记录子弹的类型，双倍还是正
    int getNumber();//用此函数来创建不会重复的number，返回的int值在999个对象中是唯一

    int save_timer_bullet_to_single;
    int save_timer_two_secs;

    int interval_enemy1_1,interval_enemy1_2,interval_enemy1_3;//产生敌机的计时器间隔时间
    int interval_enemy2_1,interval_enemy2_2,interval_enemy2_3;
    int interval_enemy3_1,interval_enemy3_2,interval_enemy3_3;

    int grade;//记录当前的难
    int myScore;
    bool prop_state;

    qreal planeWidth();//返回飞机的宽
    qreal planeHeight();//返回飞机的高
private slots:

    void addenemy1_1();//与一个定时器连接，每调用一次就会生成一个新的飞_1代表1号飞机，后面的数字代表速度，数字越大速度越快
    void addenemy1_2();//同上
    void addenemy1_3();//同上
    void addenemy2_1();//同上
    void addenemy2_2();//同上
    void addenemy2_3();//同上
    void addenemy3_1();//同上
    void addenemy3_2();//同上
    void addenemy3_3();//同上

    void addBullet();//与一个定时器连接，每调用一次就会生成一个新的子

    void enemy_timer_start();//暂停制造飞
    void enemy_timer_stop();//继续制造飞

    void upgrade();//提升游戏难度
    void setScore(int num);
public slots:
    void addProp();//增加一个道


    void bulletToDouble();//在飞机吃到双倍子弹的道具后调用，将子弹变成双倍的
    void bulletToSingle();//在飞机变成双倍子弹后启动一个定时器，当定时器结束时调用此函数，将子弹恢复成单个

    void pause_game();//当用户在qml界面中按下暂停键后会调用此函在qml中调，然后通过此函数响应c++代码的动
    void quit_game();//当用户在qml界面中按下退出键后会调用此函在qml中调，然后通过此函数响应c++代码的动
    void start_game();//当用户在qml界面中按下开始键后会调用此函在qml中调，然后通过此函数响应c++代码的动
    void resume_game();//当用户在qml界面中按下继续键后会调用此函在qml中调，然后通过此函数响应c++代码的动
    void setPlanesXY(qreal x,qreal y);//在qml中被调用，可以让qml通过此函数设置f1战机的坐

    void planes_dit();//如果f1战机碰到敌机，那么mythread类中会发射信号来激活这个槽
    void pressBomb();//当用户在qml界面中按下炸弹道具后会调用此函数

private:
   int m_gameRuningTime;
   int gameRuningTime();

};

#endif // WINDOWPLANES_H
