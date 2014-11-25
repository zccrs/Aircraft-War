#ifndef PROP_H
#define PROP_H

#include <QDeclarativeItem>
#include <QPropertyAnimation>
#include <QPixmap>
#include <QPainter>
#include <QTimer>

class WindowPlanes;
class Prop : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit Prop(WindowPlanes *parent = 0);
    void paint(QPainter *new_painter, const QStyleOptionGraphicsItem *new_style, QWidget *new_widget=0);
    ~Prop();
    void setNumber(int x);//给这个对象设置一个唯一的编号

    int mytype,haha;//0是双倍子弹，1是炸弹



    QPixmap *pixmap;
signals:
    void mypos(int u,int d,int l,int r,int number,int type);
private:

    QPropertyAnimation animation;


    int number;
    bool mystate;
private slots:
    void anime_state(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

public slots:
    void go();
    void pause_me();
    void resume_me();


    void callme();//调用这个槽函数事就发射自己的信息
    void isMe(int num);//判断num是不是自己的编号，是的话说明自己已经被f1战机吃了
};

#endif // PROP_H
