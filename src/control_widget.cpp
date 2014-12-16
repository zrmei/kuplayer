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
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QAction>
#include <QApplication>
#include <QTime>


struct DECLARE_NAMESPACE_KUPLAYER(ControlWidget_Impl)
{
    Q_DISABLE_COPY(ControlWidget_Impl)
    
    QAction *backward_key;
    QAction *stop_key;
    QAction *pause_key;
    QAction *foreward_key;
    QAction *vol_up_key;
    QAction *vol_down_key;
    QAction *mute_key;
          QLabel *time_current;
    ControlLabel *backward_;
    ControlLabel *stop_;
    ControlLabel *play_pause;
    ControlLabel *foreward_;
          QLabel *time_all;
     SelectLabel *xuan_ji;
     
     ControlWidget_Impl()
     {
         time_current = new QLabel;
         time_current->setText("00:00:00");
         
         QPalette text_palette;
         text_palette.setColor(QPalette::WindowText, QColor(255, 255, 255));
         time_current->setPalette(text_palette);
         time_current->setMinimumWidth(65);
            
         backward_ = new ControlLabel(":/control/backward");
         backward_->setToolTip(ControlWidget::tr("Backward"));
         
         stop_ = new ControlLabel(":/control/stop");
         stop_->setToolTip(ControlWidget::tr("Stop"));
         
         play_pause = new ControlLabel(":/control/play");
         play_pause->setToolTip(ControlWidget::tr("Play/Pause"));
         
         foreward_ = new ControlLabel(":/control/foreward");
         foreward_->setToolTip(ControlWidget::tr("Foreward"));
         
         time_all = new QLabel;
         time_all->setText("00:00:00");
         time_all->setMinimumWidth(65);
         time_all->setPalette(text_palette);
         
         xuan_ji = new SelectLabel(ControlWidget::tr("Episode"),"");
         xuan_ji->setFixedSize(60,30);
     }

     ~ControlWidget_Impl()
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
};

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


ControlWidget::ControlWidget(QWidget *parent)
    : QWidget(parent)
    , pImpl(new ControlWidget_Impl())
{

    connect(pImpl->backward_,SIGNAL(clicked()),this,SIGNAL(backward_clicked()));
    connect(pImpl->stop_,SIGNAL(clicked()),this,SIGNAL(stop_clicked()));
    connect(pImpl->play_pause,SIGNAL(clicked()),this,SLOT(on_play_pause_triggered()));
    connect(pImpl->foreward_,SIGNAL(clicked()),this,SIGNAL(foreward_clicked()));
    connect(pImpl->xuan_ji,SIGNAL(be_selected(QString,QString)),this,SIGNAL(xuan_ji_clcked(QString,QString)));

    init_actions();

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    main_layout->setContentsMargins(0,3,0,0);
    main_layout->addStretch();
    main_layout->addWidget(pImpl->time_current);
    main_layout->addWidget(pImpl->backward_);
    main_layout->addWidget(pImpl->stop_);
    main_layout->addWidget(pImpl->play_pause);
    main_layout->addWidget(pImpl->foreward_);
    main_layout->addWidget(pImpl->time_all);
    main_layout->addStretch();
    main_layout->addWidget(pImpl->xuan_ji);
}
ControlWidget::~ControlWidget()
{
}
QList<QAction *> ControlWidget::reg_actions()
{
    return {{pImpl->backward_key,pImpl->stop_key,pImpl->pause_key,
                    pImpl->foreward_key,pImpl->vol_up_key,pImpl->vol_down_key}};
}
void ControlWidget::init_actions()
{
    pImpl->backward_key = new QAction(this);
    pImpl->backward_key->setShortcut(QKeySequence(Qt::Key_Left));
    connect(pImpl->backward_key,SIGNAL(triggered()),this,SIGNAL(backward_clicked()));

    pImpl->stop_key = new QAction(this);
    pImpl->stop_key->setShortcut(QKeySequence(Qt::Key_End));
    connect(pImpl->stop_key,SIGNAL(triggered()),this,SIGNAL(stop_clicked()));

    pImpl->pause_key = new QAction(this);
    pImpl->pause_key->setShortcut(QKeySequence(Qt::Key_Space));
    connect(pImpl->pause_key,SIGNAL(triggered()),this,SLOT(on_play_pause_triggered()));

    pImpl->foreward_key = new QAction(this);
    pImpl->foreward_key->setShortcut(QKeySequence(Qt::Key_Right));
    connect(pImpl->foreward_key,SIGNAL(triggered()),this,SIGNAL(foreward_clicked()));

    pImpl->vol_up_key = new QAction(this);
    pImpl->vol_up_key->setShortcut(QKeySequence(Qt::Key_Up));
    connect(pImpl->vol_up_key,SIGNAL(triggered()),this,SIGNAL(vol_up_clicked()));

    pImpl->vol_down_key = new QAction(this);
    pImpl->vol_down_key->setShortcut(QKeySequence(Qt::Key_Down));
    connect(pImpl->vol_down_key,SIGNAL(triggered()),this,SIGNAL(vol_down_clicked()));

    pImpl->mute_key = new QAction(this);
    pImpl->mute_key->setShortcut(QKeySequence(Qt::Key_M));
    connect(pImpl->mute_key,SIGNAL(triggered()),this,SIGNAL(vol_mute_clicked()));
}

void ControlWidget::on_play_pause_triggered(bool)
{
    if(!isRuning) return;
    
    static const QPixmap pause = QPixmap(":/control/pause").scaled(26,26);
    static const QPixmap play = QPixmap(":/control/play").scaled(26,26);
    
    if(pImpl->play_pause->objectName() == "play"){
        pImpl->play_pause->setObjectName("pause");
        pImpl->play_pause->setPixmap(pause);
    }else{
        pImpl->play_pause->setObjectName("play");
        pImpl->play_pause->setPixmap(play);
    }
}

void ControlWidget::on_play_pause_triggered()
{
    on_play_pause_triggered(true);
    if(pImpl->play_pause->objectName() == "play")
        emit play_pause_clicked(false);
    else
        emit play_pause_clicked(true);
}

void ControlWidget::on_time_changed(qint64 pos)
{
    pImpl->time_current->setText(QTime(0, 0, 0).addMSecs(pos).toString("HH:mm:ss"));
}

void ControlWidget::on_douration_changed(qint64 pos)
{
    pImpl->time_all->setText(QTime(0, 0, 0).addMSecs(pos).toString("HH:mm:ss"));
}
