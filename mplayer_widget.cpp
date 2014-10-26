/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/
#include "common.h"

#include "mplayer_widget.h"
#include <QApplication>
#include "kuplayer.h"

/*****************************************************************/
MPlayer::MPlayer(QObject *parent)
    :  QtAV::AVPlayer(parent)
{
    connect(this,SIGNAL(stopped()),this,SLOT(mStarted()));
//    QtAV::setLogLevel(QtAV::LogWarning);
}

MPlayer::~MPlayer()
{
}

void MPlayer::setPlayList()
{
    QFile f(qApp->applicationDirPath()+"/sources/playlist.list");
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
        setStartPosition(0);
        play(play_list.at(0));
        play_list.removeAt(0);
    }else{
        emit mFinished();
    }
}

void MPlayer::mStop()
{
    play_list.clear();
    stop();
}

void MPlayer::mSeekBack()
{
    int pos = this->position() - 10000/*10s*/;
    if(pos <= 0) pos = 0;
    this->setPosition(pos);
}

void MPlayer::mSeekFore()
{
    setPosition(position() + 10000/*10s*/);
}



/*****************************************************************/

RendererWidget::RendererWidget(QWidget *parent)
    : QtAV::WidgetRenderer(parent)
{
    setContentsMargins(0,0,0,0);
    this->setMouseTracking(true);
}

void RendererWidget::mouseMoveEvent(QMouseEvent *ev)
{
    QtAV::WidgetRenderer::mouseMoveEvent(ev);
    if(ev->pos() != last_point){
        this->setCursor(QCursor(Qt::ArrowCursor));
        QTimer::singleShot( 3000,this, SLOT(hide_cursor()) );
        last_point = ev->pos();
    }
}

void RendererWidget::hide_cursor()
{
    this->setCursor(QCursor(Qt::BlankCursor));
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
