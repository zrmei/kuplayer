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

#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H
#include "common.h"

#include <QPushButton>

KUPLAYER_NAMESPACE_BEGIN //namespace begin

class PushButton final : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = 0);
    ~PushButton();
    void setPicName(QString pic_name);

protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *);

private:
    //枚举按钮的几种状态
    enum ButtonStatus {NORMAL, ENTER, PRESS, NOSTATUS};
    ButtonStatus status;
    QString pic_name;

    int btn_width; //按钮宽度
    int btn_height; //按钮高度
    bool is_left_pressed; //按钮左键是否按下
};

KUPLAYER_NAMESPACE_END // namespace end
#endif // PUSH_BUTTON_H
