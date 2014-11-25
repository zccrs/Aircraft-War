#include "windowplanes.h"


WindowPlanes::WindowPlanes(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    number=0;
    game_state="stop";
    bullet_type=1;
    prop_state=false;
    save_timer_bullet_to_single=save_timer_two_secs=-1;

    setX(0);
    setY(0);
    planes = new MyPlanes(this);

    connect(planes,SIGNAL(play_music(QString)),SIGNAL(play_music(QString)));

    thread = new QThread(this);
    mythread = new MyThread(this);
    mythread->moveToThread(thread);

    connect(this,SIGNAL(myThreadStart()), mythread, SLOT(start()));
    connect(this,SIGNAL(myThreadStop()), mythread, SLOT(stop()));
    connect(mythread,SIGNAL(game_over()),SLOT(planes_dit()));//Qt::QueuedConnection
    connect(mythread,SIGNAL(game_over()),SIGNAL(game_over()));
    connect(mythread,SIGNAL(captureBomb()),SIGNAL(acquireBomb()));//当用户吃到炸弹道具后发射acquireBomb()信号
    connect(mythread,SIGNAL(captureDoubleBullet()),SLOT(bulletToDouble()));//吃到双倍子弹后调用bulletToDouble()

    thread->start();

#ifdef MEEGO_EDITION_HARMATTAN
    double bulletScaled=0.8,enemyScaled=0.96,enemy3Scaled=0.88,propScaled=1;
#elif defined(Q_OS_SYMBIAN_V5)//判断qt的版本
    double bulletScaled=0.8,enemyScaled=0.8,enemy3Scaled=0.66,propScaled=1;
#else
    double bulletScaled=0.6,enemyScaled=0.72,enemy3Scaled=0.66,propScaled=0.75;
#endif
    //planes->setX(width()/2-planes->width()/2);//设置飞机的位置
    //planes->setY(height()-planes->height());//设置飞机的位置
    image.load(":/Image/Bullets_01.png");
    pixmap_bullet1=QPixmap::fromImage(image.scaled(bulletScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/Bullets_02.png");
    pixmap_bullet2=QPixmap::fromImage(image.scaled(bulletScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));



    image.load(":/Image/Bullets_Bomb.png");
    pixmap_prop1=QPixmap::fromImage(image.scaled(propScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/Bullets_Dual.png");
    pixmap_prop0=QPixmap::fromImage(image.scaled(propScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));




    image.load(":/Image/F2_01.png");
    pixmap_enemy1_1=QPixmap::fromImage(image.scaled(enemyScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F2_02.png");
    pixmap_enemy1_2=QPixmap::fromImage(image.scaled(enemyScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F2_03.png");
    pixmap_enemy1_3=QPixmap::fromImage(image.scaled(enemyScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F2_04.png");
    pixmap_enemy1_4=QPixmap::fromImage(image.scaled(enemyScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F2_05.png");
    pixmap_enemy1_5=QPixmap::fromImage(image.scaled(enemyScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));



    image.load(":/Image/F3_01.png");
    pixmap_enemy2_1=QPixmap::fromImage(image.scaled(enemyScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F3_02.png");
    pixmap_enemy2_2=QPixmap::fromImage(image.scaled(enemyScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F3_03.png");
    pixmap_enemy2_3=QPixmap::fromImage(image.scaled(enemyScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F3_04.png");
    pixmap_enemy2_4=QPixmap::fromImage(image.scaled(enemyScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F3_05.png");
    pixmap_enemy2_5=QPixmap::fromImage(image.scaled(enemyScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F3_06.png");
    pixmap_enemy2_6=QPixmap::fromImage(image.scaled(enemyScaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));


    image.load(":/Image/F4_01.png");
    pixmap_enemy3_1=QPixmap::fromImage(image.scaled(enemy3Scaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F4_02.png");
    pixmap_enemy3_2=QPixmap::fromImage(image.scaled(enemy3Scaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F4_03.png");
    pixmap_enemy3_3=QPixmap::fromImage(image.scaled(enemy3Scaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F4_04.png");
    pixmap_enemy3_4=QPixmap::fromImage(image.scaled(enemy3Scaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F4_05.png");
    pixmap_enemy3_5=QPixmap::fromImage(image.scaled(enemy3Scaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F4_06.png");
    pixmap_enemy3_6=QPixmap::fromImage(image.scaled(enemy3Scaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F4_07.png");
    pixmap_enemy3_7=QPixmap::fromImage(image.scaled(enemy3Scaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F4_08.png");
    pixmap_enemy3_8=QPixmap::fromImage(image.scaled(enemy3Scaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

    image.load(":/Image/F4_09.png");
    pixmap_enemy3_9=QPixmap::fromImage(image.scaled(enemy3Scaled*image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));


    connect(&timer_bullet,SIGNAL(timeout()),this,SLOT(addBullet()));
    connect(&timer_enemy1_1,SIGNAL(timeout()),SLOT(addenemy1_1()));
    connect(&timer_enemy1_2,SIGNAL(timeout()),SLOT(addenemy1_2()));
    connect(&timer_enemy1_3,SIGNAL(timeout()),SLOT(addenemy1_3()));
    connect(&timer_enemy2_1,SIGNAL(timeout()),SLOT(addenemy2_1()));
    connect(&timer_enemy2_2,SIGNAL(timeout()),SLOT(addenemy2_2()));
    connect(&timer_enemy2_3,SIGNAL(timeout()),SLOT(addenemy2_3()));
    connect(&timer_enemy3_1,SIGNAL(timeout()),SLOT(addenemy3_1()));
    connect(&timer_enemy3_2,SIGNAL(timeout()),SLOT(addenemy3_2()));
    connect(&timer_enemy3_3,SIGNAL(timeout()),SLOT(addenemy3_3()));

    connect(&timer_prop,SIGNAL(timeout()),SLOT(addProp()));
    timer_bullet_to_single.setSingleShot(true);
    timer_two_secs.setSingleShot(true);
    connect(&timer_bullet_to_single,SIGNAL(timeout()),SLOT(bulletToSingle()));
    connect(&timer_two_secs,SIGNAL(timeout()),SLOT(enemy_timer_start()));
    grade=1;

    pixmap_bullet=&pixmap_bullet1;

}

qreal WindowPlanes::planeWidth()
{
    qDebug()<<QString::fromUtf8("飞机宽：")<<planes->width();
    return planes->width();
}
qreal WindowPlanes::planeHeight()
{
    qDebug()<<QString::fromUtf8("飞机高：")<<planes->height();
    return planes->height();
}

void WindowPlanes::upgrade()//提升游戏难度
{
    switch(grade)
    {

    case 1:
        interval_enemy1_1=1000;
        interval_enemy1_2=2000;
        interval_enemy1_3=1800;
        interval_enemy2_1=5000;
        interval_enemy2_2=15000;
        interval_enemy2_3=18000;
        interval_enemy3_1=24000;
        interval_enemy3_2=35000;
        interval_enemy3_3=47000;
        break;
    case 2:
        interval_enemy1_1=900;
        interval_enemy1_2=1500;
        interval_enemy1_3=1800;
        interval_enemy2_1=4000;
        interval_enemy2_2=6000;
        interval_enemy2_3=14000;
        interval_enemy3_1=20000;
        interval_enemy3_2=36000;
        interval_enemy3_3=46000;
        break;
    case 3:
        interval_enemy1_1=700;
        interval_enemy1_2=1000;
        interval_enemy1_3=1100;
        interval_enemy2_1=2000;
        interval_enemy2_2=2600;
        interval_enemy2_3=7000;
        interval_enemy3_1=10000;
        interval_enemy3_2=23000;
        interval_enemy3_3=35000;
        break;
    case 4:
        interval_enemy1_1=800;
        interval_enemy1_2=800;
        interval_enemy1_3=1000;
        interval_enemy2_1=1500;
        interval_enemy2_2=1600;
        interval_enemy2_3=2000;
        interval_enemy3_1=5000;
        interval_enemy3_2=12000;
        interval_enemy3_3=16000;
        break;
    default:
        interval_enemy1_1=500;
        interval_enemy1_2=600;
        interval_enemy1_3=800;
        interval_enemy2_1=1000;
        interval_enemy2_2=1200;
        interval_enemy2_3=1500;
        interval_enemy3_1=4000;
        interval_enemy3_2=8000;
        interval_enemy3_3=12000;
        break;
    }
    enemy_timer_start();
}

int WindowPlanes::getNumber()
{
    number++;
    return number%999;
}

void WindowPlanes::setScore(int num)
{
    myScore+=num;
    if(myScore>60000&&!prop_state)
    {
        timer_prop.start(30000);
        prop_state=true;
    }

    if(myScore>400000&&grade==1)
    {
        grade=2;
        upgrade();
    }
    if(myScore>500000&&grade==2)
    {
        grade=3;
        upgrade();
    }
    if(myScore>700000&&grade==3)
    {
        grade=4;
        upgrade();
    }
    if(myScore>1000000&&grade==4)
    {
        grade=5;
        upgrade();
    }
    emit addScore(num);
}


void WindowPlanes::addBullet()
{
    QPointer<Bullet> bullet= new Bullet(this);
    bullet->setNumber(getNumber());
    bullet->mytype=bullet_type;
    //qDebug()<<"bullet_type:"<<bullet_type;

    bullet->pixmap=pixmap_bullet;

    bullet->setSize(pixmap_bullet->size());
    //qDebug()<<"planes pos:"<<planes->x()<<planes->y()<<planes->width()<<planes->height();
    bullet->setX(planes->x()+planes->width()/2-bullet->width()/2);
    bullet->setY(planes->y()-bullet->height());

    bullet->go();
    emit play_music("bullet");
}
void WindowPlanes::addenemy1_1()
{
    QPointer<Enemy> enemy=new Enemy(this);
    enemy->setNumber(getNumber());
    enemy->mytype=1;
#ifdef Q_OS_SYMBIAN_V5//判断qt的版本
    enemy->speed=5600;
#else
    //enemy->speed=5600;
    enemy->speed=3100;
#endif
    enemy->pixmap=&pixmap_enemy1_1;

    enemy->setSize(enemy->pixmap->size());

    enemy->blast1=&pixmap_enemy1_1;
    enemy->blast2=&pixmap_enemy1_2;
    enemy->blast3=&pixmap_enemy1_3;
    enemy->blast4=&pixmap_enemy1_4;
    enemy->blast5=&pixmap_enemy1_5;
    enemy->HP=1;
    enemy->score=1000;


    enemy->setY(-enemy->height());
    time=QTime::currentTime();
    qsrand(time.msec()*100000+time.second()*10000);
    //qDebug()<<"qrand():"<<qrand()<<time.msec()+time.second()*1000;
    enemy->setX(qrand()%(int)(width()-enemy->width()));
    enemy->go();
}
void WindowPlanes::addenemy1_2()
{
    QPointer<Enemy> enemy=new Enemy(this);
    enemy->setNumber(getNumber());
    enemy->mytype=1;
#ifdef Q_OS_SYMBIAN_V5//判断qt的版本
    enemy->speed=4600;
#else
    //enemy->speed=4600;
    enemy->speed=2100;
#endif

    enemy->pixmap=&pixmap_enemy1_1;
    enemy->setSize(enemy->pixmap->size());
    enemy->blast1=&pixmap_enemy1_1;
    enemy->blast2=&pixmap_enemy1_2;
    enemy->blast3=&pixmap_enemy1_3;
    enemy->blast4=&pixmap_enemy1_4;
    enemy->blast5=&pixmap_enemy1_5;
    enemy->HP=1;
    enemy->score=1000;


    enemy->setY(-enemy->height());
    time=QTime::currentTime();
    qsrand(time.msec()*100000+time.second()*10000);
    //qDebug()<<"qrand():"<<qrand()<<time.msec()+time.second()*1000;
    enemy->setX(qrand()%(int)(width()-enemy->width()));
    enemy->go();
}

void WindowPlanes::addenemy1_3()
{
    QPointer<Enemy> enemy=new Enemy(this);
    enemy->setNumber(getNumber());
    enemy->mytype=1;
#ifdef Q_OS_SYMBIAN_V5//判断qt的版本
    enemy->speed=3800;
#else
    //enemy->speed=3800;
    enemy->speed=1300;
#endif

    enemy->pixmap=&pixmap_enemy1_1;
    enemy->setSize(enemy->pixmap->size());
    enemy->blast1=&pixmap_enemy1_1;
    enemy->blast2=&pixmap_enemy1_2;
    enemy->blast3=&pixmap_enemy1_3;
    enemy->blast4=&pixmap_enemy1_4;
    enemy->blast5=&pixmap_enemy1_5;
    enemy->HP=1;
    enemy->score=1000;


    enemy->setY(-enemy->height());
    time=QTime::currentTime();
    qsrand(time.msec()*100000+time.second()*10000);
    //qDebug()<<"qrand():"<<qrand()<<time.msec()+time.second()*1000;
    enemy->setX(qrand()%(int)(width()-enemy->width()));
    enemy->go();
}
void WindowPlanes::addenemy2_1()
{
    QPointer<Enemy> enemy=new Enemy(this);
    enemy->setNumber(getNumber());
    enemy->mytype=2;
#ifdef Q_OS_SYMBIAN_V5//判断qt的版本
    enemy->speed=6200;
#else
    //enemy->speed=6200;
    enemy->speed=3800;
#endif

    enemy->pixmap=&pixmap_enemy2_1;
    enemy->setSize(enemy->pixmap->size());
    enemy->blast1=&pixmap_enemy2_1;
    enemy->blast2=&pixmap_enemy2_2;
    enemy->blast3=&pixmap_enemy2_3;
    enemy->blast4=&pixmap_enemy2_4;
    enemy->blast5=&pixmap_enemy2_5;
    enemy->blast6=&pixmap_enemy2_6;

    enemy->HP=5;
    enemy->score=6000;


    enemy->setY(-enemy->height());
    time=QTime::currentTime();
    qsrand(time.msec()*100000+time.second()*10000);
    //qDebug()<<"qrand():"<<qrand()<<time.msec()+time.second()*1000;
    enemy->setX(qrand()%(int)(width()-enemy->width()));
    enemy->go();
}
void WindowPlanes::addenemy2_2()
{
    QPointer<Enemy> enemy=new Enemy(this);
    enemy->setNumber(getNumber());
    enemy->mytype=2;
#ifdef Q_OS_SYMBIAN_V5//判断qt的版本
    enemy->speed=5000;
#else
    //enemy->speed=5000;
    enemy->speed=2500;
#endif

    enemy->pixmap=&pixmap_enemy2_1;
    enemy->setSize(enemy->pixmap->size());
    enemy->blast1=&pixmap_enemy2_1;
    enemy->blast2=&pixmap_enemy2_2;
    enemy->blast3=&pixmap_enemy2_3;
    enemy->blast4=&pixmap_enemy2_4;
    enemy->blast5=&pixmap_enemy2_5;
    enemy->blast6=&pixmap_enemy2_6;
    enemy->HP=5;
    enemy->score=6000;


    enemy->setY(-enemy->height());
    time=QTime::currentTime();
    qsrand(time.msec()*100000+time.second()*10000);
    //qDebug()<<"qrand():"<<qrand()<<time.msec()+time.second()*1000;
    enemy->setX(qrand()%(int)(width()-enemy->width()));
    enemy->go();
}

void WindowPlanes::addenemy2_3()
{
    QPointer<Enemy> enemy=new Enemy(this);
    enemy->setNumber(getNumber());
    enemy->mytype=2;
#ifdef Q_OS_SYMBIAN_V5//判断qt的版版本
    enemy->speed=4000;
#else
    //enemy->speed=4000;
    enemy->speed=1500;
#endif

    enemy->pixmap=&pixmap_enemy2_1;
    enemy->setSize(enemy->pixmap->size());
    enemy->blast1=&pixmap_enemy2_1;
    enemy->blast2=&pixmap_enemy2_2;
    enemy->blast3=&pixmap_enemy2_3;
    enemy->blast4=&pixmap_enemy2_4;
    enemy->blast5=&pixmap_enemy2_5;
    enemy->blast6=&pixmap_enemy2_6;
    enemy->HP=5;
    enemy->score=6000;


    enemy->setY(-enemy->height());
    time=QTime::currentTime();
    qsrand(time.msec()*100000+time.second()*10000);
    //qDebug()<<"qrand():"<<qrand()<<time.msec()+time.second()*1000;
    enemy->setX(qrand()%(int)(width()-enemy->width()));
    enemy->go();
}
void WindowPlanes::addenemy3_1()
{
    QPointer<Enemy> enemy=new Enemy(this);
    enemy->setNumber(getNumber());
    enemy->mytype=3;
#ifdef Q_OS_SYMBIAN_V5//判断qt版本
    enemy->speed=9500;
#else
    //enemy->speed=9500;
    enemy->speed=7000;
#endif


    enemy->pixmap=&pixmap_enemy3_1;
    enemy->setSize(enemy->pixmap->size());
    enemy->blast1=&pixmap_enemy3_1;
    enemy->blast2=&pixmap_enemy3_2;
    enemy->blast3=&pixmap_enemy3_3;
    enemy->blast4=&pixmap_enemy3_4;
    enemy->blast5=&pixmap_enemy3_5;
    enemy->blast6=&pixmap_enemy3_6;
    enemy->blast7=&pixmap_enemy3_7;
    enemy->blast8=&pixmap_enemy3_8;
    enemy->blast9=&pixmap_enemy3_9;

    enemy->HP=15;
    enemy->score=30000;


    enemy->setY(-enemy->height());
    time=QTime::currentTime();
    qsrand(time.msec()*100000+time.second()*10000);
    //qDebug()<<"qrand():"<<qrand()<<time.msec()+time.second()*1000;
    enemy->setX(qrand()%(int)(width()-enemy->width()));
    enemy->go();

    emit play_music("flying");
}
void WindowPlanes::addenemy3_2()
{
    QPointer<Enemy> enemy=new Enemy(this);
    enemy->setNumber(getNumber());
    enemy->mytype=3;
#ifdef Q_OS_SYMBIAN_V5//判断qt的版本
    enemy->speed=7400;
#else
    //enemy->speed=7400;
    enemy->speed=4900;
#endif

    enemy->pixmap=&pixmap_enemy3_1;
    enemy->setSize(enemy->pixmap->size());
    enemy->blast1=&pixmap_enemy3_1;
    enemy->blast2=&pixmap_enemy3_2;
    enemy->blast3=&pixmap_enemy3_3;
    enemy->blast4=&pixmap_enemy3_4;
    enemy->blast5=&pixmap_enemy3_5;
    enemy->blast6=&pixmap_enemy3_6;
    enemy->blast7=&pixmap_enemy3_7;
    enemy->blast8=&pixmap_enemy3_8;
    enemy->blast9=&pixmap_enemy3_9;

    enemy->HP=15;
    enemy->score=30000;

    enemy->setY(-enemy->height());
    time=QTime::currentTime();
    qsrand(time.msec()*100000+time.second()*10000);
    //qDebug()<<"qrand():"<<qrand()<<time.msec()+time.second()*1000;
    enemy->setX(qrand()%(int)(width()-enemy->width()));
    enemy->go();
    emit play_music("flying");
}

void WindowPlanes::addenemy3_3()
{
    QPointer<Enemy> enemy=new Enemy(this);
    enemy->setNumber(getNumber());
    enemy->mytype=3;
#ifdef Q_OS_SYMBIAN_V5//判断qt的版朊    enemy->speed=6300;
#else
    //enemy->speed=6300;
    enemy->speed=3800;
#endif

    enemy->pixmap=&pixmap_enemy3_1;
    enemy->setSize(enemy->pixmap->size());
    enemy->blast1=&pixmap_enemy3_1;
    enemy->blast2=&pixmap_enemy3_2;
    enemy->blast3=&pixmap_enemy3_3;
    enemy->blast4=&pixmap_enemy3_4;
    enemy->blast5=&pixmap_enemy3_5;
    enemy->blast6=&pixmap_enemy3_6;
    enemy->blast7=&pixmap_enemy3_7;
    enemy->blast8=&pixmap_enemy3_8;
    enemy->blast9=&pixmap_enemy3_9;
    enemy->HP=15;
    enemy->score=30000;

    enemy->setY(-enemy->height());
    time=QTime::currentTime();
    qsrand(time.msec()*100000+time.second()*10000);
    //qDebug()<<"qrand():"<<qrand()<<time.msec()+time.second()*1000;
    enemy->setX(qrand()%(int)(width()-enemy->width()));
    enemy->go();
    emit play_music("flying");
}

void WindowPlanes::addProp()
{
    QPointer<Prop> prop=new Prop(this);//0代表是子弱代表炸弹
    time=QTime::currentTime();
    qsrand(time.msec()*100000+time.second()*10000);
    int temp=qrand()%2;

    if(temp==0)
    {
        prop->mytype=0;
        prop->pixmap=&pixmap_prop0;
    }
    else
    {
        prop->mytype=1;
        prop->pixmap=&pixmap_prop1;
    }
    prop->setNumber(getNumber());
    prop->setSize(prop->pixmap->size());

    prop->setY(-prop->height());
    time=QTime::currentTime();
    qsrand(time.msec()*100000+time.second()*10000);
    prop->setX(qrand()%(int)(width()-prop->width()));


    prop->go();

    emit play_music("out_porp");

    //qDebug()<<"add double bullet ok";
}

void WindowPlanes::bulletToDouble()
{
    time_double_bullet_begin.start();//记录双子弹状态是从什么时候开始的
    save_timer_bullet_to_single = 15000;
    pixmap_bullet=&pixmap_bullet2;
    bullet_type=2;

    emit play_music("double_saler");

    timer_bullet_to_single.start(15000);
}

void WindowPlanes::bulletToSingle()
{
    pixmap_bullet=&pixmap_bullet1;
    bullet_type=1;
    save_timer_bullet_to_single=-1;
}

void WindowPlanes::enemy_timer_stop()
{
    timer_enemy1_1.stop();
    timer_enemy1_2.stop();
    timer_enemy1_3.stop();
    timer_enemy2_1.stop();
    timer_enemy2_2.stop();
    timer_enemy2_3.stop();
    timer_enemy3_1.stop();
    timer_enemy3_2.stop();
    timer_enemy3_3.stop();
}

void WindowPlanes::enemy_timer_start()
{
    save_timer_two_secs=-1;
    timer_enemy1_1.start(interval_enemy1_1);
    timer_enemy1_2.start(interval_enemy1_2);

    timer_enemy2_1.start(interval_enemy2_1);
    timer_enemy2_2.start(interval_enemy2_2);

    timer_enemy3_1.start(interval_enemy3_1);
    timer_enemy3_2.start(interval_enemy3_2);

    if(grade>1)
    {
        timer_enemy1_3.start(interval_enemy1_3);
        timer_enemy2_3.start(interval_enemy2_3);
        timer_enemy3_3.start(interval_enemy3_3);
    }
}

void WindowPlanes::pressBomb(){//进入2秒内无飞机状怊
    time_no_enemy_begin.start();//记录无飞机状态是从什么时候开始的
    save_timer_two_secs = 2000;
    enemy_timer_stop();
    timer_two_secs.start(2000);
    emit bomb_all_enemy();
}


void WindowPlanes::pause_game()
{
    if(game_state=="run")
    {
        game_state="pause";
        emit pause_launch_bullet();//发送信号让子弹和飞机都暂停
        timer_bullet.stop();//停止发射子弹

        if(timer_two_secs.isActive()){//如果现在是无飞机时期（当用户使用过炸弹道具之呧ǒ内没有新的敌机生成＊        {
            save_timer_two_secs -= time_no_enemy_begin.elapsed();//保存剩余倒计时时闊
            qDebug()<<"save_timer_two_secs:"<<save_timer_two_secs;
            timer_two_secs.stop();//停止发射子弹
        }
        else
        {
            enemy_timer_stop();//停止新生成敌朊
        }

        if(timer_bullet_to_single.isActive()){//如果现在是双子弹状怊        {
            save_timer_bullet_to_single -= time_double_bullet_begin.elapsed();//保存剩余倒计时时闊
            qDebug()<<"save_timer_bullet_to_single:"<<save_timer_bullet_to_single;
            timer_bullet_to_single.stop();//停止发射子弹
        }


        timer_prop.stop();
//        mythread->stop();
        // Yeatse:
        emit myThreadStop();
    }
}
void WindowPlanes::quit_game()
{
    if(game_state!="stop")
    {
        game_state="stop";
        m_gameRuningTime = time_game_start_runing.elapsed();//记录游戏结束时用时多久
        emit remove_all_bullet();
        planes->timer.stop();
        timer_bullet.stop();

        timer_bullet_to_single.stop();
        enemy_timer_stop();
        timer_prop.stop();

//        mythread->stop();
        // Yeatse:
        emit myThreadStop();
    }
}
void WindowPlanes::start_game()
{
    //qDebug()<<QThread::currentThread();
    if(game_state=="stop")
    {
        myScore=0;
        grade=1;
        number=0;
        bullet_type=1;
        prop_state=false;
        pixmap_bullet=&pixmap_bullet1;
        save_timer_bullet_to_single=save_timer_two_secs=-1;

        planes->setX(width()/2-planes->width()/2);//设置飞机的位置
        planes->setY(height()-planes->height());//设置飞机的位置
        //qDebug()<<QString::fromUtf8("飞机的位置是")<<width()<<height();

        game_state="run";
        planes->show();
        planes->timer.start(100);
        timer_bullet.start(230);
        time_game_start_runing.start();

        upgrade();
//        mythread->start();
        // Yeatse:
        emit myThreadStart();
    }
}
void WindowPlanes::resume_game()
{
    if(game_state=="pause")
    {
        game_state="run";
        emit resume_launch_bullet();
        timer_bullet.start(230);

        if(save_timer_two_secs==-1)
            enemy_timer_start();
        else{//如果现在是无飞机状态（使用过炸弹道具之无内无新的飞机生成）
            timer_two_secs.start(save_timer_two_secs);
            time_no_enemy_begin.start();//启动记录无飞机状态的已经过去多久
        }

        if(save_timer_bullet_to_single!=-1){//如果现在是双子弹状怊
            timer_bullet_to_single.start(save_timer_bullet_to_single);
            time_double_bullet_begin.start();//启动记录双子弹状态已经过去多久
        }

        if(prop_state)
            timer_prop.start(30000);//29033
        //25821

//        mythread->start();
        // Yeatse:
        emit myThreadStart();
    }
}
void WindowPlanes::setPlanesXY(qreal x, qreal y)
{
    planes->setX(x);
    planes->setY(y);
}
void WindowPlanes::planes_dit()
{
    pause_game();
    planes->remove_me();
}
WindowPlanes::~WindowPlanes()
{
    // Yeatse
    if (thread){
        thread->terminate();
        thread->wait();
        mythread->deleteLater();
    }
    planes->deleteLater();
}

int WindowPlanes::gameRuningTime()
{
    return m_gameRuningTime;
}
