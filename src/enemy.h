#ifndef ENEMY_H
#define ENEMY_H

#include <QDeclarativeItem>
#include <QPropertyAnimation>
#include <QPixmap>

#include <QPainter>
#include <QTimer>
class WindowPlanes;

class Enemy : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit Enemy(WindowPlanes *parent = 0);
    ~Enemy();
    QPropertyAnimation animation;
    QPixmap *pixmap;
    QPixmap *blast1,*blast2,*blast3,*blast4,*blast5,*blast6,*blast7,*blast8,*blast9;//最后一个用来储存自己原本的样子
    void paint(QPainter *new_painter, const QStyleOptionGraphicsItem *new_style, QWidget *new_widget=0);
    Enemy *last,*next;//分别指向和他相邻的两个飞机
    QTimer mymovie,flasher;
    int HP;//生命值
    int score;
    int mytype;
    int count;//记录爆炸时的动画播放到了第几张
    void setNumber(int x);//给这个对象设置一个唯一的编号
    int speed;
signals:
    void addScore(int num);
    void mypos(int u,int d,int l,int r,int number,int mytype);

    void play_music(const QString music_name);//播放音频
private:
    int number;
private slots:
    void vary();//当子弹击中自己时会变成另一张图片，在10毫秒后调用此函数变回来
    void my_flasher();//大飞机飞行过程中头部的闪动
public slots:
    void callme();//调用这个槽函数事就发射自己的信息
    void isMe(int num,int type);//判断num是不是自己的编号，是的话说明自己已经击中敌机

    void go();
    void anime_state(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    void pause_me();//暂停飞行
    void resume_me();//恢复飞行状态
    void movie();//爆炸时的动画

    void startMovie();//留给主类调用，用来告诉此对象，让其自己爆炸
};

#endif // ENEMY_H
