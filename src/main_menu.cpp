/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/24
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "main_menu.h"
#include "select_label.h"
#include "push_button.h"
#include "ui_control_classes.h"

#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

MenuWidget::MenuWidget(QWidget *parent)
    : ShadowWidget(parent)
{
    setAttribute(Qt::WA_QuitOnClose,false);
    setWindowModality(Qt::ApplicationModal);
    setMinimumSize(600,430);

    up_title_layout = new QHBoxLayout;
    set_no_margin(up_title_layout);
    btn_close = new PushButton;
    btn_close->setPicName(":/sysbutton/close");
    connect(btn_close,SIGNAL(clicked()),this,SLOT(hide()));
    up_title_layout->addWidget(btn_close,0,Qt::AlignTop);
    up_title_layout->addStretch();

    down_widget = new down_widget_;
    down_widget->setAutoFillBackground(true);
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addLayout(up_title_layout);
    main_layout->addWidget(down_widget);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(5,5,5,5);
}

void MenuWidget::this_show()
{
    qDebug() << "menu show \t" << tr("Basic Settings");
    if(isHidden()){
        QPoint pos_ = QCursor::pos();
        move(pos_.x()+120,pos_.y()+120);
        down_widget->be_selected(tr("Basic Settings"),"");
        show();
    }else{
        hide();
    }
}

void MenuWidget::show_about()
{
    down_widget->be_selected(tr("About"),"");
    show();
}

void MenuWidget::init_setting(conf_info* info)
{
    down_widget->init_setting(info);
}

/*********************************************************************************/

down_widget_::down_widget_(QWidget *parent)
    : QWidget(parent)
    , settings(new conf_info)
    , label_sores(new QList<SelectLabel*>)
{
    base_set = new SelectLabel(setting_strs[0]);
    base_set->setFixedSize(125,32);
    label_sores->append(base_set);
    base_set->set_selected(true);
    connect(base_set,SIGNAL(be_selected(QString,QString)),
            this,SLOT(be_selected(QString,QString)));

    play_set = new SelectLabel(setting_strs[1]);
    play_set->setFixedHeight(32);
    label_sores->append(play_set);
    connect(play_set,SIGNAL(be_selected(QString,QString)),
            this,SLOT(be_selected(QString,QString)));

    about_set = new SelectLabel(setting_strs[2]);
    about_set->setFixedHeight(32);
    label_sores->append(about_set);
    connect(about_set,SIGNAL(be_selected(QString,QString)),
            this,SLOT(be_selected(QString,QString)));

    btn_save = new SelectLabel(setting_strs[3]);
    btn_save->setFixedSize(80,32);
    connect(btn_save,SIGNAL(be_selected(QString,QString)),
            this,SLOT(btn_selected(QString,QString)));

    btn_exit = new SelectLabel(setting_strs[4]);
    btn_exit->setFixedSize(80,32);
    connect(btn_exit,SIGNAL(be_selected(QString,QString)),
            this,SLOT(btn_selected(QString,QString)));

    QWidget *left_widget = new QWidget;
    left_widget->setAutoFillBackground(true);
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::Window, QColor(225, 225, 225));
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

    base_set_widget = new base_set_weidget;
    connect(base_set_widget->ui->comboBox,SIGNAL(currentIndexChanged(int))
            ,SLOT(LanguageChanged(int)));
    right_widget->addWidget(base_set_widget);
    right_widget->setCurrentIndex(0);

    play_set_widget_ = new play_set_widget;
    right_widget->addWidget(play_set_widget_);

    about_widget_ = new about_widget;
    right_widget->addWidget(about_widget_);

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
down_widget_::~down_widget_()
{
    delete_list(label_sores);
    delete base_set_widget;
    delete play_set_widget_;
    delete about_widget_;
    delete left_layout;
    delete down_layout;
}

void down_widget_::init_setting(conf_info* info)
{
    if(info->default_video_format.isEmpty()) return;
    settings = info;
    play_set_widget_->ui->checkBox_auto_play_next->setChecked(settings->auto_play_next);
    base_set_widget->ui->checkBox_close->setChecked(settings->close_all);
    base_set_widget->ui->radioButton_min->setChecked(settings->min_or_close);
    base_set_widget->ui->checkBox_start->setChecked(settings->start_when_pc_on);
    if(settings->default_video_format == "high")
        play_set_widget_->ui->format_high->setChecked(true);
    else if(settings->default_video_format == "normal")
        play_set_widget_->ui->format_normal->setChecked(true);
    else
        play_set_widget_->ui->format_spuer->setChecked(true);
    base_set_widget->ui->comboBox->setCurrentIndex(settings->language? 0 : 1 );
}

void down_widget_::be_selected(QString name, QString)
{
    auto index = setting_strs.indexOf(name);
    for_each(label_sores->begin(),label_sores->end(),
             [](QList<SelectLabel*>::value_type item){
        item->set_selected(false);});
    label_sores->at(index)->set_selected(true);
    right_widget->setCurrentIndex(index);
}

void down_widget_::LanguageChanged(int index)
{
    settings->language = ( index == 0 );
}

void down_widget_::btn_selected(QString name, QString)
{
    if(name == setting_strs[3]){
        settings->auto_play_next = play_set_widget_->ui->checkBox_auto_play_next->isChecked();
        settings->close_all = base_set_widget->ui->checkBox_close->isChecked();
        settings->min_or_close = base_set_widget->ui->radioButton_min->isChecked();
        settings->start_when_pc_on = base_set_widget->ui->checkBox_start->isChecked();
        if(play_set_widget_->ui->format_high->isChecked())
            settings->default_video_format = "high";
        else if(play_set_widget_->ui->format_normal->isChecked())
            settings->default_video_format = "normal";
        else
            settings->default_video_format = "super";
    }else{
        init_setting(settings);
    }
    qobject_cast<MenuWidget*>(parent())->hide();
}

