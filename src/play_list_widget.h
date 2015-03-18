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

#ifndef PLAY_LIST_WIDGET_H
#define PLAY_LIST_WIDGET_H

#include "shadow_widget.h"
#include <QList>

KUPLAYER_NAMESPACE_BEGIN //namespace begin
class PushButton;
class SelectLabel;

typedef QList<std::tuple<QString, QString, QString>> list_map;

class PlayListWidget : public ShadowWidget
{
    Q_OBJECT
signals:
    void click(QString, QString);
public:
    explicit PlayListWidget(QWidget *parent = 0);
    ~PlayListWidget();
    void sort(const QStringList &list);
    QList<QAction *> init_action();
    bool IsEnd {false};
public slots:
    void on_list_changed(int, const QStringList &);
    void on_xuan_ji_show(QString, QString);
    void on_playNext_clicked();
    void on_playPrev_clicked();
protected:
    void leaveEvent(QEvent *);
private slots:
    void this_click(QString name, QString url);
private:
    int col , currentIndex;
    list_map  play_list;
    struct PlayListWidget_Impl;
    std::unique_ptr<PlayListWidget_Impl> pImpl;
};

KUPLAYER_NAMESPACE_END // namespace end
#endif // PLAY_LIST_WIDGET_H
