/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "common.h"
#include "control_widget.h"
#include "select_label.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QAction>
#include <QApplication>
#include <QTime>

/*****************************************************************************/
ControlLabel::ControlLabel(QString name,QWidget *parent)
    : QLabel(parent)
{
    setObjectName(name.split("/").last());
    setPixmap(QPixmap(name).scaled(26,26));
    setFixedSize(30,30);
}
void ControlLabel::mousePressEvent(QMouseEvent *ev)
{
    QLabel::mousePressEvent(ev);
    emit clicked();
}

/*****************************************************************************/
ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent)
{
    time_current = new QLabel;
    time_current->setText("00:00:00");
    QPalette text_palette = time_current->palette();
    text_palette.setColor(QPalette::WindowText, QColor(255, 255, 255));
    time_current->setPalette(text_palette);
    time_current->setMinimumWidth(65);

    backward_ = new ControlLabel(":/control/backward");
    connect(backward_,SIGNAL(clicked()),this,SIGNAL(backward_clicked()));
    stop_ = new ControlLabel(":/control/stop");
    connect(stop_,SIGNAL(clicked()),this,SIGNAL(stop_clicked()));
    play_pause = new ControlLabel(":/control/play");
    connect(play_pause,SIGNAL(clicked()),this,SLOT(on_play_pause()));
    foreward_ = new ControlLabel(":/control/foreward");
    connect(foreward_,SIGNAL(clicked()),this,SIGNAL(foreward_clicked()));

    time_all = new QLabel;
    time_all->setText("00:00:00");
    time_all->setPalette(text_palette);
    time_all->setMinimumWidth(65);
    xuan_ji = new SelectLabel("选集","");
    xuan_ji->setFixedSize(60,30);
    connect(xuan_ji,SIGNAL(be_selected(QString,QString)),this,SIGNAL(xuan_ji_clcked(QString,QString)));

    init_actions();

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    main_layout->setContentsMargins(0,3,0,0);
    main_layout->addStretch();
    main_layout->addWidget(time_current);
    main_layout->addWidget(backward_);
    main_layout->addWidget(stop_);
    main_layout->addWidget(play_pause);
    main_layout->addWidget(foreward_);
    main_layout->addWidget(time_all);
    main_layout->addStretch();
    main_layout->addWidget(xuan_ji);
}
ControlWidget::~ControlWidget()
{
    delete time_current;
    delete backward_;
    delete backward_key;
    delete stop_;
    delete stop_key;
    delete play_pause;
    delete pause_key;
    delete foreward_;
    delete foreward_key;
    delete vol_up_key;
    delete vol_down_key;
}
void ControlWidget::init_actions()
{
    backward_key = new QAction(this);
    backward_key->setShortcut(QKeySequence(Qt::Key_Left));
    connect(backward_key,SIGNAL(triggered()),this,SIGNAL(backward_clicked()));

    stop_key = new QAction(this);
    stop_key->setShortcut(QKeySequence(Qt::Key_End));
    connect(stop_key,SIGNAL(triggered()),this,SIGNAL(stop_clicked()));

    pause_key = new QAction(this);
    pause_key->setShortcut(QKeySequence(Qt::Key_Space));
    connect(pause_key,SIGNAL(triggered()),this,SLOT(on_play_pause()));

    foreward_key = new QAction(this);
    foreward_key->setShortcut(QKeySequence(Qt::Key_Right));
    connect(foreward_key,SIGNAL(triggered()),this,SIGNAL(foreward_clicked()));

    vol_up_key = new QAction(this);
    vol_up_key->setShortcut(QKeySequence(Qt::Key_Up));
    connect(vol_up_key,SIGNAL(triggered()),this,SIGNAL(vol_up_clicked()));

    vol_down_key = new QAction(this);
    vol_down_key->setShortcut(QKeySequence(Qt::Key_Down));
    connect(vol_down_key,SIGNAL(triggered()),this,SIGNAL(vol_down_clicked()));

    mute_key = new QAction(this);
    mute_key->setShortcut(QKeySequence(Qt::Key_M));
    connect(mute_key,SIGNAL(triggered()),this,SIGNAL(vol_mute_clicked()));
}

void ControlWidget::trigger_play_pause(bool)
{
    if(!isRuning) return;
    if(play_pause->objectName() == "play"){
        play_pause->setObjectName("pause");
        play_pause->setPixmap(QString(":/control/pause"));
    }else{
        play_pause->setObjectName("play");
        play_pause->setPixmap(QString(":/control/play"));
    }
}

void ControlWidget::on_play_pause()
{
    trigger_play_pause(true);
    if(play_pause->objectName() == "play")
        emit play_pause_clicked(false);
    else
        emit play_pause_clicked(true);
}

void ControlWidget::setTime(qint64 pos)
{
    time_current->setText(QTime(0, 0, 0).addMSecs(pos).toString("HH:mm:ss"));
}

void ControlWidget::setDuration(qint64 pos)
{
    time_all->setText(QTime(0, 0, 0).addMSecs(pos).toString("HH:mm:ss"));
}
