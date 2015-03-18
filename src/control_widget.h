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
#ifndef CONTROL_WIDGET_H
#define CONTROL_WIDGET_H
#include "common.h"

#include <QWidget>
class QAction;

KUPLAYER_NAMESPACE_BEGIN //namespace begin


class ControlWidget : public QWidget
{
    Q_OBJECT
signals:
    void backward_clicked();
    void stop_clicked();
    void play_pause_clicked(bool);
    void foreward_clicked();
    void xuan_ji_clcked(QString, QString);
    void vol_up_clicked();
    void vol_down_clicked();
    void vol_mute_clicked();
    
public:
    explicit ControlWidget(QWidget *parent = 0);
    virtual ~ControlWidget();
    QList<QAction *> reg_actions();
    bool isRuning;
    
public slots:
    void on_play_pause_triggered(bool);
    void on_time_changed(qint64);
    void on_douration_changed(qint64);
    
private:
    void init_actions();
    
    struct ControlWidget_Impl;
    std::unique_ptr<ControlWidget_Impl> pImpl;
};


KUPLAYER_NAMESPACE_END //namespace end
#endif // CONTROL_WIDGET_H
