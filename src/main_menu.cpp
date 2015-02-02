/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/24
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "gloal_func.h"
#include "main_menu.h"
#include "select_label.h"
#include "push_button.h"
#include "ui_control_classes.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

struct NAMESPACE_KUPLAYER::MenuWidget::MenuWidget_Impl
{
    PushButton *btn_close;
    down_widget_*down_widget;
    
    MenuWidget_Impl()
        : btn_close(new PushButton)
        , down_widget(new down_widget_)
    {}
    ~MenuWidget_Impl()
    {
        delete btn_close;
        delete down_widget;
    }
};

MenuWidget::MenuWidget(QWidget *parent)
    : ShadowWidget(parent)
    , pImpl(new MenuWidget_Impl())
{
    setAttribute(Qt::WA_QuitOnClose,false);
    setWindowModality(Qt::ApplicationModal);
    setMinimumSize(600,400);
    
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::Background,QColor(255,255,255,50));
    setPalette(text_palette);
    
    QHBoxLayout *up_title_layout = new QHBoxLayout;
    set_no_margin(up_title_layout);
    
    pImpl->btn_close->setPicName(":/sysbutton/close");
    connect(pImpl->btn_close,SIGNAL(clicked()),this,SLOT(hide()));
    up_title_layout->addWidget(pImpl->btn_close,0,Qt::AlignTop);
    up_title_layout->addStretch();
    
    pImpl->down_widget->setAutoFillBackground(true);
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addLayout(up_title_layout);    
    main_layout->addWidget(pImpl->down_widget);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(5,5,5,5);
}

MenuWidget::~MenuWidget()
{ }

void MenuWidget::on_showed()
{
    if(isHidden()){
        QPoint pos_ = QCursor::pos();
        move(pos_.x()+120,pos_.y()+100);
        pImpl->down_widget->be_selected(QObject::tr("Basic Settings"),"");
        show();
    }else{
        hide();
    }
}

void MenuWidget::show_about()
{
    pImpl->down_widget->be_selected(QObject::tr("About"),"");
    show();
}

void MenuWidget::init_setting(conf_info* info)
{
    pImpl->down_widget->init_setting(info);
}

/*********************************************************************************/

struct NAMESPACE_KUPLAYER::down_widget_::down_widget_Impl
{
    conf_info *settings;
    SelectLabel *base_set;
    SelectLabel *play_set;
    SelectLabel *about_set;
    SelectLabel *btn_save;
    SelectLabel *btn_exit;
    QList<SelectLabel*> *label_sores;
    base_set_weidget *base_set_widget;
    play_set_widget *play_set_widget_;
    about_widget *about_widget_;
    Ui::base_set_weidget *base_ui;
    Ui::play_set_widget  *play_ui;
    
    const QStringList setting_strs{
        QObject::tr("Basic Settings"),QObject::tr("Play Settings"),
                QObject::tr("About"),QObject::tr("Save"),
                QObject::tr("Cancel")};
    
    down_widget_Impl()
        : base_set(new SelectLabel(QObject::tr("Basic Settings")))
        , play_set(new SelectLabel(QObject::tr("Play Settings")))
        , about_set(new SelectLabel(QObject::tr("About")))
        , btn_save(new SelectLabel(QObject::tr("Save")))
        , btn_exit(new SelectLabel(QObject::tr("Cancel")))
        , label_sores(new QList<SelectLabel*>)
        , base_set_widget(new base_set_weidget)
        , play_set_widget_(new play_set_widget)
        , about_widget_(new about_widget)
    {
        base_ui = base_set_widget->ui;
        play_ui = play_set_widget_->ui;
        
        base_set->setFixedSize(125,32);
        label_sores->append(base_set);
        base_set->set_selected(true);
        
        play_set->setFixedHeight(32);
        label_sores->append(play_set);
        
        about_set->setFixedHeight(32);
        label_sores->append(about_set);
        
        btn_save->setFixedSize(80,32);
        btn_exit->setFixedSize(80,32);
    }
    
    ~down_widget_Impl()
    {
        delete_list(label_sores);
        delete base_set_widget;
        delete play_set_widget_;
        delete about_widget_;  
    }
};

down_widget_::down_widget_(QWidget *parent)
    : QWidget(parent)
    ,pImpl(new down_widget_Impl())
{
    connect(pImpl->base_set, SIGNAL(be_selected(QString,QString)), this,SLOT(be_selected(QString,QString))); 
    connect(pImpl->play_set, SIGNAL(be_selected(QString,QString)), this,SLOT(be_selected(QString,QString)));   
    connect(pImpl->about_set,SIGNAL(be_selected(QString,QString)), this,SLOT(be_selected(QString,QString)));
    connect(pImpl->btn_save, SIGNAL(be_selected(QString,QString)), this,SLOT(btn_selected(QString,QString)));
    connect(pImpl->btn_exit, SIGNAL(be_selected(QString,QString)), this,SLOT(btn_selected(QString,QString)));
    
    QWidget *left_widget = new QWidget;
    left_widget->setAutoFillBackground(true);
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::Window, QColor(225, 225, 225));
    text_palette.setColor(QPalette::Background,QColor(255,255,255,120));
    left_widget->setPalette(text_palette);
    
    QVBoxLayout *left_layout = new QVBoxLayout(left_widget);
    left_layout->addWidget(pImpl->base_set);
    left_layout->addWidget(pImpl->play_set);
    left_layout->addWidget(pImpl->about_set);
    left_layout->addStretch();
    left_layout->setSpacing(1);
    left_layout->setContentsMargins(0,0,0,0);
    
    text_palette.setColor(QPalette::Window,QColor(240,240,240));
    text_palette.setColor(QPalette::Background,QColor(255,255,255,50));
    
    right_widget = new QStackedWidget;
    right_widget->setAutoFillBackground(true);
    right_widget->setPalette(text_palette);
    
    connect(pImpl->base_ui->comboBox,SIGNAL(currentIndexChanged(int)),
            SLOT(LanguageChanged(int)));
        
    right_widget->addWidget(pImpl->base_set_widget);
    right_widget->setCurrentIndex(0);
    right_widget->addWidget(pImpl->play_set_widget_);
    right_widget->addWidget(pImpl->about_widget_);
    
    QWidget *down_back_widget = new QWidget;
    down_back_widget->setAutoFillBackground(true);
    down_back_widget->setPalette(text_palette);
    QHBoxLayout *down_layout = new QHBoxLayout(down_back_widget);
    down_layout->addStretch();
    down_layout->addWidget(pImpl->btn_save);
    down_layout->addWidget(pImpl->btn_exit);
    
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
    delete right_widget;
}

void down_widget_::init_setting(conf_info* info)
{
    if(info->default_video_format.isEmpty()) return;
    pImpl->settings = info;
    pImpl->play_ui->checkBox_auto_play_next->setChecked(pImpl->settings->auto_play_next);
    pImpl->base_ui->checkBox_close->setChecked(pImpl->settings->close_all);
    pImpl->base_ui->radioButton_min->setChecked(pImpl->settings->min_or_close);
    pImpl->base_ui->checkBox_start->setChecked(pImpl->settings->start_when_pc_on);
    if(pImpl->settings->default_video_format == "high")
        pImpl->play_ui->format_high->setChecked(true);
    else if(pImpl->settings->default_video_format == "normal")
        pImpl->play_ui->format_normal->setChecked(true);
    else
        pImpl->play_ui->format_spuer->setChecked(true);
    pImpl->base_ui->comboBox->setCurrentIndex(pImpl->settings->language? 0 : 1 );
}

void down_widget_::be_selected(QString name, QString)
{
    auto index = pImpl->setting_strs.indexOf(name);
    for_each(pImpl->label_sores->begin(),pImpl->label_sores->end(),
             [](QList<SelectLabel*>::value_type item){item->set_selected(false);});
    pImpl->label_sores->at(index)->set_selected(true);
    right_widget->setCurrentIndex(index);
}

void down_widget_::LanguageChanged(int index)
{
    pImpl->settings->language = ( index == 0 );
}
#define BUTTON_OK_STR 3
void down_widget_::btn_selected(QString name, QString)
{
    if(name == pImpl->setting_strs[BUTTON_OK_STR]){
        pImpl->settings->auto_play_next = pImpl->play_ui->checkBox_auto_play_next->isChecked();
        pImpl->settings->close_all = pImpl->base_ui->checkBox_close->isChecked();
        pImpl->settings->min_or_close = pImpl->base_ui->radioButton_min->isChecked();
        pImpl->settings->start_when_pc_on = pImpl->base_ui->checkBox_start->isChecked();
        if(pImpl->play_ui->format_high->isChecked())
            pImpl->settings->default_video_format = "high";
        else if(pImpl->play_ui->format_normal->isChecked())
            pImpl->settings->default_video_format = "normal";
        else
            pImpl->settings->default_video_format = "super";
    }else{
        init_setting(pImpl->settings);
    }
    qobject_cast<MenuWidget*>(parent())->hide();
}

