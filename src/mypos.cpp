#include "mypos.h"
#include <QMouseEvent>
#include "settings.h"
#include <QDebug>
MyPos::MyPos(Settings *setting,QLabel *parent) :
    QLabel(parent)
{
    mysetting=setting;
    state=mysetting->getValue("mypos-state",0).toInt();
    //qDebug()<<"mypos:"<<state;
    if(state==0)
        image.load(":/Image/n9-w.png");
    else if(state==1)
        image.load(":/Image/n9.png");
    else image.load(":/Image/n9-r.png");
    pixmap=QPixmap::fromImage(image);
    setPixmap(pixmap);
    setFixedSize(image.size());
}
void MyPos::mousePressEvent(QMouseEvent *event)
{
    mousePos=event->pos();
}
void MyPos::mouseMoveEvent(QMouseEvent *event)
{
    move(pos()+event->pos()-mousePos);

}
void MyPos::mouseDoubleClickEvent(QMouseEvent *event)
{
    state++;
    if(state>2) state=0;
    //qDebug()<<state;
    if(state==0)
        image.load(":/Image/n9-w.png");
    else if(state==1)
        image.load(":/Image/n9.png");
    else image.load(":/Image/n9-r.png");
    pixmap=QPixmap::fromImage(image);
    setPixmap(pixmap);
    setFixedSize(image.size());
    emit myStateChanged(width(),height());
}
MyPos::~MyPos()
{
    mysetting->setValue("mypos-state",state);
    //qDebug()<<mysetting->getValue("mypos-state",0).toInt();
}
