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

#include "select_label.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <QMouseEvent>

#define THIS_WIDGET_COLOR(r,g,b){ QPalette text_palette = palette();\
        text_palette.setColor(QPalette::WindowText, QColor(r, g, b));\
        setPalette(text_palette);}

SelectLabel::SelectLabel(QString name, QString url, QWidget *parent)
    : QLabel(parent)
    , url_(url)
{
    setText(name);
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    THIS_WIDGET_COLOR(0, 0, 0);
    QFont ft;
    ft.setPixelSize(15);
    ft.setBold(false);
    setFont(ft);
}

SelectLabel::~SelectLabel()
{
}
void SelectLabel::enterEvent(QEvent *)
{
    mouse_pressed = true;
    update();
}



void SelectLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    QLabel::mouseReleaseEvent(ev);

    if (be_select) {
        return;
    }

    if (ev->button() == Qt::LeftButton &&
        rect().contains(ev->pos())) {
        mouse_pressed = true;
        emit be_selected(text(), url_);
    }

    update();
}

void SelectLabel::leaveEvent(QEvent *ev)
{
    QLabel::leaveEvent(ev);

    if (!be_select) {
        mouse_pressed = false;
    }

    update();
}

void SelectLabel::set_selected(bool is_selected)
{
    be_select = is_selected;
    mouse_pressed = is_selected;
    update();
}

void SelectLabel::paintEvent(QPaintEvent *ev)
{
    QLabel::paintEvent(ev);
    static auto old = this->styleSheet();

    if (mouse_pressed) {
        setStyleSheet("background-color:#25BCDA");
        THIS_WIDGET_COLOR(255, 255, 255);
    } else {
        setStyleSheet(old);
        THIS_WIDGET_COLOR(0, 0, 0);
    }
}

