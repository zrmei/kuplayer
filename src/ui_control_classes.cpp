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

#include "ui_control_classes.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

base_set_weidget::base_set_weidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::base_set_weidget)
{
    ui->setupUi(this);
}

base_set_weidget::~base_set_weidget()
{
    delete ui;
}

/*******************************************************************************************/

play_set_widget::play_set_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::play_set_widget)
{
    ui->setupUi(this);
}
play_set_widget::~play_set_widget()
{
    delete ui;
}
/*******************************************************************************************/

about_widget::about_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::about_widget)
{
    ui->setupUi(this);
}

about_widget::~about_widget()
{
    delete ui;
}

#include <QDesktopServices>
#include <QUrl>
void about_widget::on_label_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

