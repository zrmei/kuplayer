/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/

#include "push_button.h"
#include <QMouseEvent>
#include <QPainter>

PushButton::PushButton(QWidget *parent)
    :QPushButton(parent)
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
    if(event->button() == Qt::LeftButton){
        is_left_pressed = true;
        status = PRESS;
        update();
    }
}

void PushButton::mouseReleaseEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if(is_left_pressed  && rect().contains(event->pos())){
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
    switch(status){
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
