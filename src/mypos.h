#ifndef MYPOS_H
#define MYPOS_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include "qmlapplicationviewer.h"


class Settings;
class MyPos : public QLabel
{
    Q_OBJECT
public:
    explicit MyPos(Settings *setting, QLabel *parent = 0);
    ~MyPos();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    QPoint mousePos;
    //QPainter painter;
    QPixmap pixmap;
    QImage image;
    Settings *mysetting;

    void init(QmlApplicationViewer* viewer);
private:
    int state;

    QmlApplicationViewer* viewer;
signals:
    void myStateChanged();
public slots:
    
};

#endif // MYPOS_H
