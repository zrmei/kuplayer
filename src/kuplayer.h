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

#ifndef KUPLAYER_H
#define KUPLAYER_H
#include "common.h"
#include "shadow_widget.h"

#include <QSystemTrayIcon>
class QSettings;

KUPLAYER_NAMESPACE_BEGIN //namespace begin
class PyScript;

class MainWidget : public ShadowWidget
{
    Q_OBJECT
signals:
    void send_status(int);

public:
    MainWidget(PyScript *pyinit, const QString &, QWidget *parent = 0);
    virtual ~MainWidget();
    void setIniFile(QSettings *);
    
public slots:
    void on_showMin_clicked();
    void on_skin_changed(QString);
    void on_Fullscreen_changed();
    void on_play_finished();
    void on_stop_clicked();
    void on_loadImage_started(int, QStringList);
    void on_nextPage_loaded(CLASS);
    void on_loadImage_finished(CLASS, QPixmap, QString, QString);
    void on_url_triggered(QString, QString);
    void on_url_ji_triggered(QString, QString);
    void on_url_changed(CLASS, int, QString);

private slots:
    void on_trayIcon_clicked(QSystemTrayIcon::ActivationReason);
    void on_title_widget_closed();

private:
    void init_module();
    bool is_fullscreen_can_changed();
    bool is_can_auto_play_next();

    void init_setting();
    void to_inifile();
    void init_trayicon();

private:
    struct MainWidget_Impl;
    std::unique_ptr<MainWidget_Impl> pImpl;
};

KUPLAYER_NAMESPACE_END //namespace end
#endif // KUPLAYER_H
