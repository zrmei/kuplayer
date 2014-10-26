/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/
#include "select_label.h"
#include <QMouseEvent>
#define THIS_WIDGET_COLOR(r,g,b){ QPalette text_palette = palette();\
        text_palette.setColor(QPalette::WindowText, QColor(r, g, b));\
        setPalette(text_palette);}

SelectLabel::SelectLabel(QString name, QString url, QWidget *parent)
    : QLabel(parent)
    ,url_(url)
{
    setText(name);
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    THIS_WIDGET_COLOR(0,0,0);

    QFont ft;
    ft.setPixelSize(15);
    ft.setBold(false);
    this->setFont(ft);

//    this->setFixedSize(60,25);
}

SelectLabel::~SelectLabel()
{

}
void SelectLabel::enterEvent(QEvent *)
{
    mouse_pressed = true;
    update();
}

void SelectLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton){
        mouse_pressed = true;
        emit be_selected(text(),url_);
    }
    update();
}

void SelectLabel::mouseReleaseEvent(QMouseEvent *)
{

}

void SelectLabel::leaveEvent(QEvent *)
{
    if(!be_select){
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
    if(mouse_pressed){
        this->setStyleSheet("background-color:#25BCDA");
        THIS_WIDGET_COLOR(255,255,255);

    }else{
        this->setStyleSheet("background-color:#ffffff");
        THIS_WIDGET_COLOR(0,0,0);

    }
}

