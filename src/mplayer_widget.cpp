/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/15
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "common.h"

#include "mplayer_widget.h"
#include <QApplication>
#include "AudioOutput.h"

/****************************************************************/
MPlayer::MPlayer(QObject *parent)
    :  QtAV::AVPlayer(parent)
{
    connect(this,SIGNAL(stopped()),this,SLOT(mStarted()));
#ifdef QT_NO_DEBUG_OUTPUT
    QtAV::setLogLevel(QtAV::LogLevel::LogOff);
#else
    QtAV::setLogLevel(QtAV::LogAll);
#endif
}

MPlayer::~MPlayer()
{
}

void MPlayer::setPlayList()
{
    QFile f(qApp->applicationDirPath()+"/.playlist");
    if(f.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream txtInput(&f);
        play_list.clear();
        while(!txtInput.atEnd()){
            auto tmp = txtInput.readLine();
            play_list.append(tmp);
        }
        f.close();
    }
}

void MPlayer::mPlay()
{
    setPlayList();
    if(isPlaying()){
        stop();
    }else{
        mStarted();
    }
}

void MPlayer::mStarted()
{
    if(play_list.size()){
                play(play_list.at(0));
        play_list.removeAt(0);
        emit mSetDuration(duration());
    }else{
        emit mFinished(false);
    }
}

void MPlayer::mStop()
{
    play_list.clear();
    stop();
    emit mFinished(true);
}

void MPlayer::mSeekBack()
{
    int pos = position() - 10000/*10s*/;
    if(pos <= 0) pos = 0;
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
        qreal v = audio()->volume();
#ifdef QT_NO_DEBUG_OUTPUT
        if(v >= 1.0)
            return;
#endif
        if(v > 0.5)
            v += 0.1;
        else if (v > 0.1)
            v += 0.05;
        else
            v += 0.025;
        audio()->setVolume(v);
        qDebug("vol = %.3f", audio()->volume());
    }
}

void MPlayer::vol_down()
{
    QtAV::AudioOutput *ao = audio();
    if (ao && ao->isAvailable()) {
        qreal v = audio()->volume();
#ifdef QT_NO_DEBUG_OUTPUT
        if(v <= 0.0)
            return;
#endif
        if (v > 0.5)
            v -= 0.1;
        else if (v > 0.1)
            v -= 0.05;
        else
            v -= 0.025;
        audio()->setVolume(v);
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
    if(ev->key() == Qt::Key_Escape){
        emit escape_clicked();
    }
}
