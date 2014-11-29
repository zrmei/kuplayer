/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "common.h"
#include "gloal_func.h"
#include "title_widget.h"
#include "push_button.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

#define THIS_WIDGET_COLOR(r,g,b){ QPalette text_palette = palette();\
    text_palette.setColor(QPalette::WindowText, QColor(r, g, b));\
    setPalette(text_palette);}

TypeLabel::TypeLabel(QString title, QWidget *parent)
    :QLabel(parent)
{
    setText(title);
    setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    //set txt color
    THIS_WIDGET_COLOR(28, 161, 227);
    //set the font size and blod
    QFont ft;
    ft.setFamily("Ubuntu");
    ft.setPointSize(18);
    setFont(ft);
    
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
struct DECLARE_NAMESPACE_KUPLAYER(TitleWidget_Impl)
{
    PushButton *btn_min;
    PushButton *btn_close;
    PushButton *btn_skin;
    PushButton *btn_menu;
    QLabel *title;
    QHBoxLayout *down_title_layout;
    QHBoxLayout *up_title_layout;
#ifdef HAS_LOGO
    QLabel *logo;
    QHBoxLayout *right_title_layout;
#endif
    QList<TypeLabel*> *labels_store;
    QList<PushButton*> *push_button_store;
    
    TitleWidget_Impl()
    #ifdef HAS_LOGO
        , right_title_layout(new QHBoxLayout)
    #endif
        : labels_store(new QList<TypeLabel*>)
        , push_button_store(new QList<PushButton*>)
        
    {
        btn_close = new PushButton;
        btn_close->setObjectName("close");
        btn_close->setToolTip(TitleWidget::tr("Close"));
        
        btn_min = new PushButton;
        btn_min->setObjectName("min");
        btn_min->setToolTip(TitleWidget::tr("Min"));
        
        btn_skin = new PushButton;
        btn_skin->setObjectName("skin");
        btn_skin->setToolTip(TitleWidget::tr("Skin"));
        
        btn_menu = new PushButton;
        btn_menu->setObjectName("menu");
        btn_menu->setToolTip(TitleWidget::tr("Menu"));
        
        title = new QLabel;
        push_button_store->append(btn_close);
        push_button_store->append(btn_min);
        push_button_store->append(btn_skin);
        push_button_store->append(btn_menu);
        
        down_title_layout = new QHBoxLayout;
        up_title_layout = new QHBoxLayout;
        btn_close->setPicName(":/sysbutton/close");    
        up_title_layout->addWidget(btn_close,0,Qt::AlignTop);
        
        btn_min->setPicName(":/sysbutton/min");    
        up_title_layout->addWidget(btn_min,0,Qt::AlignTop);
        btn_skin->setPicName(":/sysbutton/skin");
        
        up_title_layout->addWidget(btn_skin,0,Qt::AlignTop);
        btn_menu->setPicName(":/sysbutton/menu");
        up_title_layout->addWidget(btn_menu,0,Qt::AlignTop);
#ifdef HAS_LOGO
        logo = new QLabel;
        logo->setPixmap(QString(":/logo/logo"));
        right_title_layout->addWidget(logo);
        set_no_margin(right_title_layout);
#endif
        up_title_layout->addStretch(2);
        up_title_layout->addWidget(title);
        up_title_layout->addStretch(3);
        set_no_margin(up_title_layout);
    }
    
    ~TitleWidget_Impl()
    {
        delete_list(push_button_store);
        delete_list(labels_store);
        delete title;
        delete down_title_layout;
        delete up_title_layout;
#ifdef HAS_LOGO
        delete logo;
        delete right_title_layout;
#endif
    }
};

TitleWidget::TitleWidget(QWidget *parent)
    : QWidget(parent)
    , pImpl(new TitleWidget_Impl())
{
    connect(pImpl->btn_close,SIGNAL(clicked()),this,SIGNAL(close_clicked()));
    connect(pImpl->btn_min,SIGNAL(clicked()),this,SIGNAL(min_clicked()));
    connect(pImpl->btn_skin,SIGNAL(clicked()),this,SIGNAL(skin_clicked()));
    connect(pImpl->btn_menu,SIGNAL(clicked()),this,SIGNAL(menu_clicked()));
    
    for_each(down_title.begin(),down_title.end(),[&](QStringList::value_type name){
        TypeLabel *label = new TypeLabel(name);
        pImpl->down_title_layout->addWidget(label,0,Qt::AlignBottom|Qt::AlignHCenter);
        pImpl->labels_store->append(label);
        connect(label,SIGNAL(clicked(QString)),SLOT(turepage(QString)));
    });
    pImpl->down_title_layout->addStretch();
    set_no_margin(pImpl->down_title_layout);
    
    QVBoxLayout *left_layout = new QVBoxLayout;
    set_no_margin(left_layout);
    left_layout->addLayout(pImpl->up_title_layout);
    left_layout->addLayout(pImpl->down_title_layout);
    
    QHBoxLayout *main_layout = new QHBoxLayout(this);
    set_no_margin(main_layout);
    main_layout->addLayout(left_layout);
#ifdef HAS_LOGO
    main_layout->addStretch(5);
    main_layout->addLayout(pImpl->right_title_layout);
    main_layout->addStretch(1);
#endif
    setFixedHeight(TITLE_WIDGET_HEIGHT);
}

TitleWidget::~TitleWidget()
{

}
void TitleWidget::set_text(QString name)
{
    pImpl->title->setText(name);
    pImpl->title->adjustSize();
}
QString TitleWidget::get_text() const
{
    return pImpl->title->text();
}
void TitleWidget::turepage(int index)
{
    for_each(pImpl->labels_store->begin(),pImpl->labels_store->end(),
             [](QList<TypeLabel*>::value_type label){
        label->setMousePress(false);});
    pImpl->labels_store->at(index)->setMousePress(true);
    emit ture_page(index);
}

void TitleWidget::turepage(QString name)
{
    unsigned int index = down_title.indexOf(name);
    if(index > PLAYER)
        index = PLAYER;
    turepage(index);
}
