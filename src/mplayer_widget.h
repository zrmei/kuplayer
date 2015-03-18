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

#ifndef MPLAYER_WIDGET_H
#define MPLAYER_WIDGET_H

#include "common.h"

#include <QApplication>
#include <QFile>
#include <QtAV>
#include <QtAVWidgets>

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
    QFile list_file;
};

class RendererWidget final : public QtAV::WidgetRenderer
{
    Q_OBJECT
signals:
    void double_clicked();
public:
    RendererWidget(QWidget *parent = 0)
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
    MPlayerWidget(QWidget *parent = 0);
    ~MPlayerWidget();
    ControlWidget *operator->() const
    {return control_widget;}
protected:
    virtual void keyPressEvent(QKeyEvent *);
public:
    ControlWidget *control_widget;
    RendererWidget *renderer;
};

KUPLAYER_NAMESPACE_END // namespace end
#endif // MPLAYER_WIDGET_H
