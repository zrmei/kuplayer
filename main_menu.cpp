#include "main_menu.h"
#include "push_button.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

MenuWidget::MenuWidget(QWidget *parent)
    : ShadowWidget(parent)
{
    setAttribute(Qt::WA_QuitOnClose,false);
    this->setMinimumSize(800,430);

    up_title_layout = new QHBoxLayout;
    set_no_margin(up_title_layout);

    btn_close = new PushButton;
    btn_close->setPicName(":/sysbutton/close");
    connect(btn_close,SIGNAL(clicked()),this,SLOT(hide()));
    up_title_layout->addWidget(btn_close,0,Qt::AlignTop);
    up_title_layout->addStretch();


    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addLayout(up_title_layout);
}

void MenuWidget::this_show()
{

    if(this->isHidden()){
        QPoint pos_ = QCursor::pos();
        this->move(pos_.x()+30,pos_.y()+120);
        this->show();
    }else{
        this->hide();
    }
}
