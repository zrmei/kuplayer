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

#include "push_button.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <QMouseEvent>
#include <QPainter>

PushButton::PushButton(QWidget *parent)
    : QPushButton(parent)
{
    status = NORMAL;
    is_left_pressed = false;
}

PushButton::~PushButton()
{
}

void PushButton::setPicName(QString pic_name)
{
    this->pic_name = pic_name;
    setFixedSize(QPixmap(pic_name).size());
}

void PushButton::enterEvent(QEvent *)
{
    status = ENTER;
    update();
}

void PushButton::mousePressEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if (event->button() == Qt::LeftButton) {
        is_left_pressed = true;
        status = PRESS;
        update();
    }
}

void PushButton::mouseReleaseEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if (is_left_pressed  && rect().contains(event->pos())) {
        is_left_pressed = false;
        status = ENTER;
        update();
        emit clicked();
    }
}

void PushButton::leaveEvent(QEvent *)
{
    status = NORMAL;
    update();
}

void PushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap;

    switch (status) {
        case NOSTATUS:
        case NORMAL:
            pixmap.load(pic_name);
            break;

        case ENTER:
            pixmap.load(pic_name + QString("_hover"));
            break;

        case PRESS:
            pixmap.load(pic_name + QString("_pressed"));
            break;

        default:
            return;
    }

    painter.drawPixmap(rect(), pixmap);
}
