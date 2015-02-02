/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/15
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef MPLAYER_WIDGET_H
#define MPLAYER_WIDGET_H

#include "common.h"

#include <QApplication>
#include <QFile>
#include <QtAV/AVPlayer.h>
#include <QtAV/WidgetRenderer.h>

KUPLAYER_NAMESPACE_BEGIN //namespace begin

class MPlayer : public QtAV::AVPlayer
{
    Q_OBJECT
signals:
    void mFinished();
    void mSetDuration(qint64);
public:
    MPlayer(QObject *parent = 0);
    virtual ~MPlayer();
    void mPlay();
    void mStop();
public slots:
    void mSeekBack();
    void mSeekFore();
    void setPlayList();
    void play_pause(bool);
    void setDuration();

    void vol_down();
    void vol_up();
private slots:
    void mStarted();
private:
    QStringList play_list;
    QFile list_file{qApp->applicationDirPath()+"/.playlist"};
};

class RendererWidget final : public QtAV::WidgetRenderer
{
    Q_OBJECT
signals:
    void double_clicked();
public:
    RendererWidget(QWidget *parent=0)
        : QtAV::WidgetRenderer(parent)
    {}
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *)
    { emit double_clicked(); }
};

class ControlWidget;
class MPlayerWidget : public QWidget
{
    Q_OBJECT
signals:
    void escape_clicked();
public:
    MPlayerWidget(QWidget *parent=0);
    ~MPlayerWidget();
    ControlWidget* operator->() const
    {return control_widget;}
protected:
    virtual void keyPressEvent(QKeyEvent *);
public:
     ControlWidget *control_widget;
    RendererWidget *renderer;
};

KUPLAYER_NAMESPACE_END // namespace end
#endif // MPLAYER_WIDGET_H
