/*
   Copyright (C) 2015 MeiZhaorui(Mason) <listener_mei@163.com>
   
   The File is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   The File is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with the Library; if not, see
   <http://www.gnu.org/licenses/>.
*/

#include "common.h"
#include "control_widget.h"
#include "control_label.h"
#include "select_label.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QAction>
#include <QApplication>
#include <QTime>

#include <functional>
using std::bind;

struct ControlWidget::ControlWidget_Impl {
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

    ControlWidget_Impl() {
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
        xuan_ji = new SelectLabel(ControlWidget::tr("Episode"), "");
        xuan_ji->setFixedSize(60, 30);
    }

    ~ControlWidget_Impl() {
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

ControlWidget::ControlWidget(QWidget *parent)
    : QWidget(parent)
    , isRuning(false)
    , pImpl(new ControlWidget_Impl())
{
    connect(pImpl->backward_, SIGNAL(clicked()), this, SIGNAL(backward_clicked()));
    connect(pImpl->stop_, SIGNAL(clicked()), this, SIGNAL(stop_clicked()));
    connect(pImpl->play_pause, &ControlLabel::clicked, bind(&ControlWidget::on_play_pause_triggered, this, false));
    connect(pImpl->foreward_, SIGNAL(clicked()), this, SIGNAL(foreward_clicked()));
    connect(pImpl->xuan_ji, SIGNAL(be_selected(QString, QString)), this, SIGNAL(xuan_ji_clcked(QString, QString)));
    init_actions();
    QHBoxLayout *main_layout = new QHBoxLayout(this);
    main_layout->setContentsMargins(0, 3, 0, 0);
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
    return {{
            pImpl->backward_key, pImpl->stop_key, pImpl->pause_key,
            pImpl->foreward_key, pImpl->vol_up_key, pImpl->vol_down_key
        }
    };
}
void ControlWidget::init_actions()
{
    pImpl->backward_key = new QAction(this);
    pImpl->backward_key->setShortcut(QKeySequence(Qt::Key_Left));
    connect(pImpl->backward_key, SIGNAL(triggered()), this, SIGNAL(backward_clicked()));
    pImpl->stop_key = new QAction(this);
    pImpl->stop_key->setShortcut(QKeySequence(Qt::Key_End));
    connect(pImpl->stop_key, SIGNAL(triggered()), this, SIGNAL(stop_clicked()));
    pImpl->pause_key = new QAction(this);
    pImpl->pause_key->setShortcut(QKeySequence(Qt::Key_Space));
    connect(pImpl->pause_key, SIGNAL(triggered(bool)), this, SLOT(on_play_pause_triggered(bool)));
    pImpl->foreward_key = new QAction(this);
    pImpl->foreward_key->setShortcut(QKeySequence(Qt::Key_Right));
    connect(pImpl->foreward_key, SIGNAL(triggered()), this, SIGNAL(foreward_clicked()));
    pImpl->vol_up_key = new QAction(this);
    pImpl->vol_up_key->setShortcut(QKeySequence(Qt::Key_Up));
    connect(pImpl->vol_up_key, SIGNAL(triggered()), this, SIGNAL(vol_up_clicked()));
    pImpl->vol_down_key = new QAction(this);
    pImpl->vol_down_key->setShortcut(QKeySequence(Qt::Key_Down));
    connect(pImpl->vol_down_key, SIGNAL(triggered()), this, SIGNAL(vol_down_clicked()));
    pImpl->mute_key = new QAction(this);
    pImpl->mute_key->setShortcut(QKeySequence(Qt::Key_M));
    connect(pImpl->mute_key, SIGNAL(triggered()), this, SIGNAL(vol_mute_clicked()));
}

void ControlWidget::on_play_pause_triggered(bool)
{
    static QPixmap pause = QPixmap(":/control/pause").scaled(26, 26);
    static QPixmap play = QPixmap(":/control/play").scaled(26, 26);
    qDebug() << "pImpl->play_pause->objectName(): " << pImpl->play_pause->objectName();

    if (pImpl->play_pause->objectName() == "play") {
        pImpl->play_pause->setObjectName("pause");
        pImpl->play_pause->setPixmap(pause);
        emit play_pause_clicked(false);
    } else {
        pImpl->play_pause->setObjectName("play");
        pImpl->play_pause->setPixmap(play);
        emit play_pause_clicked(true);
    }
}

static void setText(QLabel *label, qint64 pos)
{
    static QTime t(0, 0, 0);
    label->setText(t.addMSecs(pos).toString("HH:mm:ss"));
    t.setHMS(0, 0, 0);
}

void ControlWidget::on_time_changed(qint64 pos)
{
    setText(pImpl->time_current, pos);
}

void ControlWidget::on_douration_changed(qint64 pos)
{
    setText(pImpl->time_all, pos);
}
