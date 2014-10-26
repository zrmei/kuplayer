/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/
#include "common.h"
#include "title_widget.h"
#include "push_button.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

#define THIS_WIDGET_COLOR(r,g,b){ QPalette text_palette = palette();\
        text_palette.setColor(QPalette::WindowText, QColor(r, g, b));\
        setPalette(text_palette);}

TypeLabel::TypeLabel(QString title_name, QWidget *parent)
    :QLabel(parent)
{
    setText(title_name);
    setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    //set txt color
    THIS_WIDGET_COLOR(28, 161, 227);
    //set the font size and blod
    QFont ft;
    ft.setFamily("宋体");
    ft.setPointSize(20);
    ft.setBold(true);
    this->setFont(ft);

    setFixedHeight(TITLE_WIDGET_HEIGHT - TITLE_CONTROL_HEIGHT);
    setFixedWidth(TITLE_WIDGET_WIDTH);
}

TypeLabel::~TypeLabel()
{

}

void TypeLabel::enterEvent(QEvent *)
{
    mouse_hover = true;
    THIS_WIDGET_COLOR(28, 121, 227);
}

void TypeLabel::leaveEvent(QEvent *)
{
    mouse_hover = false;
    THIS_WIDGET_COLOR(28, 161, 227);
}

void TypeLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        THIS_WIDGET_COLOR(185, 74, 70);
        emit clicked(text());
    }
}

void TypeLabel::mouseReleaseEvent(QMouseEvent *)
{
    THIS_WIDGET_COLOR(28, 121, 227);
}

void TypeLabel::setMousePress(bool mouse_pressed)
{
    this->mouse_pressed = mouse_pressed;
    update();
}

void TypeLabel::paintEvent(QPaintEvent *event)
{
    if(mouse_hover | mouse_pressed){
        //绘制鼠标移到按钮上的按钮效果
        painterInfo(0, 100, 150);
    }
    QLabel::paintEvent(event);
}

void TypeLabel::painterInfo(int top_color, int middle_color, int bottom_color)
{
    QPainter painter(this);
    QPen pen(Qt::NoBrush, 1);
    painter.setPen(pen);
    QLinearGradient linear(rect().topLeft(), rect().bottomLeft());
    linear.setColorAt(0, QColor(220, 220, 220, top_color));
    linear.setColorAt(0.5, QColor(220, 220, 220, middle_color));
    linear.setColorAt(1, QColor(222, 222, 222, bottom_color));
    painter.setBrush(linear);
    painter.drawRect(rect());
}


/***********************************************************************************/
TitleWidget::TitleWidget(QWidget *parent)
    : QWidget(parent)
    , down_title_layout(new QHBoxLayout)
    , up_title_layout(new QHBoxLayout)
#ifdef HAVS_LOGO
    , right_title_layout(new QHBoxLayout)
#endif
    , push_button_store(new QList<PushButton*>)
{
    btn_close = new PushButton;
    btn_close->setObjectName("close");
    btn_min = new PushButton;
    btn_min->setObjectName("min");
    btn_skin = new PushButton;
    btn_skin->setObjectName("skin");
    btn_menu = new PushButton;
    btn_menu->setObjectName("menu");
    title = new QLabel;
    push_button_store->append(btn_close);
    push_button_store->append(btn_min);
    push_button_store->append(btn_skin);
    push_button_store->append(btn_menu);

    btn_close->setPicName(":/sysbutton/close");
    connect(btn_close,SIGNAL(clicked()),this,SIGNAL(close_clicked()));
    up_title_layout->addWidget(btn_close,0,Qt::AlignTop);

    btn_min->setPicName(":/sysbutton/min");
    connect(btn_min,SIGNAL(clicked()),this,SIGNAL(min_clicked()));
    up_title_layout->addWidget(btn_min,0,Qt::AlignTop);

    btn_skin->setPicName(":/sysbutton/skin");
    connect(btn_skin,SIGNAL(clicked()),this,SIGNAL(skin_clicked()));
    up_title_layout->addWidget(btn_skin,0,Qt::AlignTop);

    btn_menu->setPicName(":/sysbutton/menu");
    connect(btn_menu,SIGNAL(clicked()),this,SIGNAL(menu_clicked()));
    up_title_layout->addWidget(btn_menu,0,Qt::AlignTop);

    up_title_layout->addStretch(2);
    up_title_layout->addWidget(title);
    up_title_layout->addStretch(3);
    set_no_margin(up_title_layout);
#ifdef HAVS_LOGO
    logo = new QLabel;
    logo->setPixmap(QString(":/logo/logo"));
    right_title_layout->addWidget(logo);
    set_no_margin(right_title_layout);
#endif
    down_title << "电视剧"<<"电  影"<<"综  艺"<<"音  乐"<<"动漫"<<"播放器";
    labels_store = new QList<TypeLabel*>;
    for(int i=0; i<down_title.size(); ++i){
        TypeLabel *label = new TypeLabel(down_title[i]);
        down_title_layout->addWidget(label,0,Qt::AlignBottom|Qt::AlignHCenter);
        labels_store->append(label);
        connect(label,SIGNAL(clicked(QString)),SLOT(turepage(QString)));
    }
    down_title_layout->addStretch();
    set_no_margin(down_title_layout);

    QVBoxLayout *left_layout = new QVBoxLayout;
    set_no_margin(left_layout);
    left_layout->addLayout(up_title_layout);
    left_layout->addLayout(down_title_layout);

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    set_no_margin(main_layout);
    main_layout->addLayout(left_layout);
#ifdef HAVS_LOGO
    main_layout->addStretch(5);
    main_layout->addLayout(right_title_layout);
    main_layout->addStretch(1);
#endif
    setFixedHeight(TITLE_WIDGET_HEIGHT);
}

TitleWidget::~TitleWidget()
{
    delete_list(push_button_store);
    delete_list(labels_store);
    delete title;
    delete down_title_layout;
    delete up_title_layout;
#ifdef HAVS_LOGO
    delete logo;
    delete right_title_layout;
#endif
}
void TitleWidget::set_text(QString name)
{
    title->setText(name);
    title->adjustSize();
}

void TitleWidget::turepage(QString name)
{
    int index = down_title.indexOf(name);

    for(int i=0; i<down_title.size(); ++i){
        labels_store->at(i)->setMousePress(false);
    }
    labels_store->at(index)->setMousePress(true);
    emit ture_page(index);
}

