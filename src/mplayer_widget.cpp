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
#include "gloal_func.h"
#include "mplayer_widget.h"

#include <QKeyEvent>
#include <QTimer>

USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer


/****************************************************************/
MPlayer::MPlayer(QObject *parent)
    :  QtAV::AVPlayer(parent)
    , list_file(qApp->applicationDirPath() + "/.playlist")
{
    connect(this, SIGNAL(stopped()), this, SLOT(mStarted()));
#ifdef AV_NO_DEBUG_OUTPUT
    QtAV::setLogLevel(QtAV::LogLevel::LogOff);
#else
    QtAV::setLogLevel(QtAV::LogAll);
#endif
}

MPlayer::~MPlayer()
{
    list_file.remove();
}

void MPlayer::setPlayList()
{
    if (list_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        static QTextStream txtInput(&list_file);
        play_list.clear();

        while (!txtInput.atEnd()) {
            play_list.append(txtInput.readLine());
        }

        list_file.close();
    }
}

void MPlayer::mPlay()
{
    setPlayList();

    if (isPlaying()) {
        stop();
    } else {
        mStarted();
    }
}
void MPlayer::mStarted()
{
    if (play_list.size()) {
        play(play_list.at(0));
        play_list.removeAt(0);
        QTimer::singleShot(800, this, SLOT(setDuration()));
    } else {
        emit mFinished();
    }
}

void MPlayer::mStop()
{
    play_list.clear();
    stop();
}

void MPlayer::play_pause(bool p)
{
    if (isPlaying()) {
        pause(p);
    } else {
        mPlay();
    }
}

void MPlayer::setDuration()
{
    emit mSetDuration(duration());
}

void MPlayer::mSeekBack()
{
    int pos = position() - 10000/*10s*/;

    if (pos <= 0) { pos = 0; }

    setPosition(pos);
}

void MPlayer::mSeekFore()
{
    setPosition(position() + 10000/*10s*/);
}

void MPlayer::vol_up()
{
    QtAV::AudioOutput *ao = audio();

    if (ao && ao->isAvailable()) {
        qreal v = ao->volume();
#ifdef QT_NO_DEBUG_OUTPUT

        if (v >= 1.0) {
            return;
        }

#endif
        v += 0.02;
        ao->setVolume(v);
        qDebug("vol = %.3f", audio()->volume());
    }
}

void MPlayer::vol_down()
{
    QtAV::AudioOutput *ao = audio();

    if (ao && ao->isAvailable()) {
        qreal v = ao->volume();
#ifdef QT_NO_DEBUG_OUTPUT

        if (v <= 0.0) {
            return;
        }

#endif
        v -= 0.02;
        ao->setVolume(v);
        qDebug("vol = %.3f", audio()->volume());
    }
}


/*****************************************************************/
#include "control_widget.h"
#include <QVBoxLayout>
MPlayerWidget::MPlayerWidget(QWidget *parent)
    : QWidget(parent)
{
    renderer = new RendererWidget(this);
    control_widget = new ControlWidget;
    control_widget->setFixedHeight(32);
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addWidget(renderer);
    main_layout->addWidget(control_widget);
    set_no_margin(main_layout);
}
MPlayerWidget::~MPlayerWidget()
{
    delete renderer;
    delete control_widget;
}

void MPlayerWidget::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Escape) {
        emit escape_clicked();
    }
}
