#include "main_menu.h"
#include "select_label.h"
#include "push_button.h"

#include "ui_base_set_weidget.h"
#include "ui_play_set_widget.h"
#include "ui_about_widget.h"

#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

MenuWidget::MenuWidget(QWidget *parent)
    : ShadowWidget(parent)
{
    setAttribute(Qt::WA_QuitOnClose,false);
    this->setMinimumSize(600,430);

    up_title_layout = new QHBoxLayout;
    set_no_margin(up_title_layout);
    btn_close = new PushButton;
    btn_close->setPicName(":/sysbutton/close");
    connect(btn_close,SIGNAL(clicked()),this,SLOT(hide()));
    up_title_layout->addWidget(btn_close,0,Qt::AlignTop);
    up_title_layout->addStretch();

    down_widget = new down_widget_;
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addLayout(up_title_layout);
    main_layout->addWidget(down_widget);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(5,5,5,5);
}

void MenuWidget::this_show()
{
    if(this->isHidden()){
        QPoint pos_ = QCursor::pos();
        this->move(pos_.x()+120,pos_.y()+120);
        this->show();
    }else{
        this->hide();
    }
}

/*********************************************************************************/

down_widget_::down_widget_(QWidget *parent)
    : QWidget(parent)
{
    base_set = new SelectLabel(setting_strs[0]);
    base_set->setFixedSize(125,32);
    connect(base_set,SIGNAL(be_selected(QString,QString)),this,SLOT(be_selected(QString,QString)));

    play_set = new SelectLabel(setting_strs[1]);
    play_set->setFixedHeight(32);
    connect(play_set,SIGNAL(be_selected(QString,QString)),this,SLOT(be_selected(QString,QString)));

    about_set = new SelectLabel(setting_strs[2]);
    about_set->setFixedHeight(32);
    connect(about_set,SIGNAL(be_selected(QString,QString)),this,SLOT(be_selected(QString,QString)));

    btn_save = new SelectLabel(setting_strs[3]);
    btn_save->setFixedSize(80,32);
    connect(btn_save,SIGNAL(be_selected(QString,QString)),this,SLOT(btn_selected(QString,QString)));

    btn_exit = new SelectLabel(setting_strs[4]);
    btn_exit->setFixedSize(80,32);
    connect(btn_exit,SIGNAL(be_selected(QString,QString)),this,SLOT(btn_selected(QString,QString)));

    QWidget *left_widget = new QWidget;
    left_widget->setAutoFillBackground(true);
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::Window, QColor(230, 230, 230));
    left_widget->setPalette(text_palette);
    left_layout = new QVBoxLayout(left_widget);
    left_layout->addWidget(base_set);
    left_layout->addWidget(play_set);
    left_layout->addWidget(about_set);
    left_layout->addStretch();
    left_layout->setSpacing(1);
    left_layout->setContentsMargins(0,0,0,0);

    text_palette.setColor(QPalette::Window,QColor(240,240,240));

    right_widget = new QStackedWidget;
    right_widget->setAutoFillBackground(true);
    right_widget->setPalette(text_palette);

    base_set_weidget *a = new base_set_weidget;
    right_widget->addWidget(a);
    right_widget->setCurrentIndex(0);

    play_set_widget *b = new play_set_widget;
    right_widget->addWidget(b);

    about_widget *c = new about_widget;
    right_widget->addWidget(c);

    QWidget *down_back_widget = new QWidget;
    down_back_widget->setAutoFillBackground(true);
    down_back_widget->setPalette(text_palette);
    down_layout = new QHBoxLayout(down_back_widget);
    down_layout->addStretch();
    down_layout->addWidget(btn_save);
    down_layout->addWidget(btn_exit);

    QVBoxLayout *right_layout = new QVBoxLayout;
    right_layout->addWidget(right_widget);
    right_layout->addWidget(down_back_widget);

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    main_layout->addWidget(left_widget);
    main_layout->addLayout(right_layout);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0,0,0,0);
}

void down_widget_::be_selected(QString name, QString)
{
    right_widget->setCurrentIndex(setting_strs.indexOf(name));
}

void down_widget_::btn_selected(QString name, QString)
{
    emit save_it_or_not( name == setting_strs[3] ? true:false );
}

/******************************************************************************************/

base_set_weidget::base_set_weidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::base_set_weidget)
{
    ui->setupUi(this);
    connect(ui->radioButton_min,SIGNAL(clicked()),SLOT(radioButtons_clicked()));
    connect(ui->radioButton_exit,SIGNAL(clicked()),SLOT(radioButtons_clicked()));

    ui->checkBox_start->setChecked(conf::settings.start_when_pc_on);
    ui->checkBox_close->setChecked(conf::settings.close_all);
    ui->radioButton_exit->setChecked(!conf::settings.min_or_close);
}

base_set_weidget::~base_set_weidget()
{
    delete ui;
}

void base_set_weidget::on_checkBox_start_clicked(bool checked)
{
    conf::settings.start_when_pc_on = checked;

}
void base_set_weidget::on_checkBox_close_clicked(bool checked)
{
    conf::settings.close_all = checked;
}

void base_set_weidget::radioButtons_clicked()
{
    if(ui->radioButton_min->isChecked()){
        conf::settings.min_or_close = true;
    }else{
        conf::settings.min_or_close = false;
    }

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


