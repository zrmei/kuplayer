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

#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include "common.h"
#include "shadow_widget.h"

class QStackedWidget;

KUPLAYER_NAMESPACE_BEGIN //namespace begin
struct conf_info;

class MenuWidget : public ShadowWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = 0);
    ~MenuWidget();
public slots:
    void on_showed();
    void show_about();
    void init_setting(conf_info *);
private:
    struct MenuWidget_Impl;
    std::unique_ptr<MenuWidget_Impl> pImpl;
};

/**********************************************/
class down_widget_ : public QWidget
{
    Q_OBJECT
public:
    explicit down_widget_(QWidget *parent = 0);
    ~down_widget_();
    QStackedWidget *operator->() { return right_widget;}
public slots:
    void init_setting(conf_info *);
    void be_selected(QString, QString);
    void btn_selected(QString, QString);
    void LanguageChanged(int);

private:
    QStackedWidget *right_widget;
    struct down_widget_Impl;
    std::unique_ptr<down_widget_Impl> pImpl;
};

KUPLAYER_NAMESPACE_END //namespace end
#endif // MAIN_MENU_H
