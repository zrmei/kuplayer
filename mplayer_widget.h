/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/
#ifndef MPLAYER_WIDGET_H
#define MPLAYER_WIDGET_H

#include <QtAV/QtAV.h>
#include <QtAV/WidgetRenderer.h>

class MPlayer final : public QtAV::AVPlayer
{
    Q_OBJECT
signals:
    void mFinished();
public:
    MPlayer(QObject *parent = 0);
    virtual ~MPlayer();
    void mPlay();

public slots:
    void mStop();
    void mSeekBack();
    void mSeekFore();
    void setPlayList();
private slots:
    void mStarted();
private:
    QStringList play_list;
};

class RendererWidget final : public QtAV::WidgetRenderer
{
    Q_OBJECT
signals:
    void double_clicked();
public:
    RendererWidget(QWidget *parent=0);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *){ emit double_clicked(); }
    virtual void mouseMoveEvent(QMouseEvent *);
private slots:
    void hide_cursor();
private:
    QPoint last_point;
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
protected:
    virtual void keyPressEvent(QKeyEvent *);
public:
     ControlWidget *control_widget;
    RendererWidget *renderer;
};

#endif // MPLAYER_WIDGET_H
