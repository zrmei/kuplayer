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

#ifndef UI_CONTROL_CLASSES_H
#define UI_CONTROL_CLASSES_H

#include "common.h"

#include <QWidget>
#include "shadow_widget.h"
#include "ui_base_set_weidget.h"
#include "ui_play_set_widget.h"
#include "ui_about_widget.h"

KUPLAYER_NAMESPACE_BEGIN //namespace begin

/**********************************************/
class base_set_weidget : public QWidget
{
    Q_OBJECT
public:
    explicit base_set_weidget(QWidget *parent = 0);
    ~base_set_weidget();

    Ui::base_set_weidget *ui;
};

/**********************************************/

class play_set_widget : public QWidget
{
    Q_OBJECT
    friend class down_widget_;
public:
    explicit play_set_widget(QWidget *parent = 0);
    ~play_set_widget();

    Ui::play_set_widget *ui;
};

/**********************************************/

class about_widget : public QWidget
{
    Q_OBJECT

public:
    explicit about_widget(QWidget *parent = 0);
    ~about_widget();
    Ui::about_widget *ui;
private slots:
    void on_label_linkActivated(const QString &link);
};

KUPLAYER_NAMESPACE_END // namespace end
#endif // UI_CONTROL_CLASSES_H
