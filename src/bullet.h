#ifndef BULLET_H
#define BULLET_H

#include <QDeclarativeItem>
#include <QThread>

#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QPropertyAnimation>

class WindowPlanes;
class Bullet : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit Bullet(WindowPlanes *parent = 0);
    ~Bullet();
    QPropertyAnimation animation;
    QPixmap *pixmap;
    void paint(QPainter *new_painter, const QStyleOptionGraphicsItem *new_style, QWidget *new_widget=0);
    bool mystate;
    int mytype;
    void setNumber(int x);//给这个对象设置一个唯一的编号
signals:
    void mypos(int u,int d,int l,int r,int number,int type);
private:
    int number;

public slots:
    void callme();//调用这个槽函数事就发射自己的信息
    void isMe(int num);//判断num是不是自己的编号，是的话说明自己已经击中敌机
    void go();
    void anime_state(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    void pause_me();
    void resume_me();
};

#endif // BULLET_H
