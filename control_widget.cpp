/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/

#include "common.h"
#include "control_widget.h"
#include "select_label.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QAction>
#include <QApplication>

/*****************************************************************************/
ControlLabel::ControlLabel(QString name,QWidget *parent)
    : QLabel(parent)
{
    this->setObjectName(name.split("/").last());
    this->setPixmap(QPixmap(name).scaled(30,30));
    this->setFixedSize(30,30);
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
    time_ = new QLabel;
    time_->setText("00:00:00");
    QPalette text_palette = time_->palette();
    text_palette.setColor(QPalette::WindowText, QColor(255, 255, 255));
    time_->setPalette(text_palette);
    time_->setMinimumWidth(65);

    backward_ = new ControlLabel(":/control/backward");
    connect(backward_,SIGNAL(clicked()),this,SIGNAL(backward_clicked()));
    backward_key = new QAction(this);
    backward_key->setShortcut(QKeySequence(Qt::Key_Left));
    connect(backward_key,SIGNAL(triggered()),this,SIGNAL(backward_clicked()));

    stop_ = new ControlLabel(":/control/stop");
    connect(stop_,SIGNAL(clicked()),this,SIGNAL(stop_clicked()));
    stop_key = new QAction(this);
    stop_key->setShortcut(QKeySequence(Qt::Key_End));
    connect(stop_key,SIGNAL(triggered()),this,SIGNAL(stop_clicked()));

    play_pause = new ControlLabel(":/control/play");
    connect(play_pause,SIGNAL(clicked()),this,SLOT(on_play_pause()));
    pause_key = new QAction(this);
    pause_key->setShortcut(QKeySequence(Qt::Key_Space));
    connect(pause_key,SIGNAL(triggered()),this,SLOT(on_play_pause()));

    foreward_ = new ControlLabel(":/control/foreward");
    connect(foreward_,SIGNAL(clicked()),this,SIGNAL(foreward_clicked()));
    foreward_key = new QAction(this);
    foreward_key->setShortcut(QKeySequence(Qt::Key_Right));
    connect(foreward_key,SIGNAL(triggered()),this,SIGNAL(foreward_clicked()));

    xuan_ji = new SelectLabel("选集","");
    xuan_ji->setFixedSize(60,30);
    connect(xuan_ji,SIGNAL(be_selected(QString,QString)),this,SIGNAL(xuan_ji_clcked(QString,QString)));

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    main_layout->setContentsMargins(0,3,0,0);
    main_layout->addWidget(time_);
    main_layout->addWidget(backward_);
    main_layout->addWidget(stop_);
    main_layout->addWidget(play_pause);
    main_layout->addWidget(foreward_);
    main_layout->addStretch();
    main_layout->addWidget(xuan_ji);
}
ControlWidget::~ControlWidget()
{
    delete time_;
    delete backward_;
    delete backward_key;
    delete stop_;
    delete stop_key;
    delete play_pause;
    delete pause_key;
    delete foreward_;
    delete foreward_key;
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
#include <QTime>
void ControlWidget::setTime(qint64 pos)
{
    time_->setText(QTime(0, 0, 0).addMSecs(pos).toString("HH:mm:ss"));
}
