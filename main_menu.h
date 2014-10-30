/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/24
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include "common.h"
#include "shadow_widget.h"

class PushButton;
class SelectLabel;
class QHBoxLayout;
class QVBoxLayout;
class QStackedWidget;

class base_set_weidget;
class play_set_widget;
class about_widget;
class down_widget_;

struct conf_info
{
 QString default_video_format;
    bool start_when_pc_on;
    bool close_all;
    bool min_or_close;
    bool auto_play_next;
};


class MenuWidget : public ShadowWidget
{
    Q_OBJECT
signals:
    void setting_changed(conf_info);
public:
    explicit MenuWidget(QWidget *parent = 0);

public slots:
    void this_show();
    void show_about();
    void init_setting(conf_info &);
private:
     PushButton *btn_close;
    QHBoxLayout *up_title_layout;
    down_widget_*down_widget;
};

/**********************************************/
class down_widget_ : public QWidget
{
    Q_OBJECT
signals:
    void setting_changed(conf_info);
public:
    explicit down_widget_(QWidget *parent = 0);
    ~down_widget_();
    QStackedWidget* operator->(){ return right_widget;}
public slots:
    void init_setting(const conf_info &info= conf_info());
    void be_selected(QString,QString);
    void btn_selected(QString,QString);
public:
    QStackedWidget *right_widget;
    conf_info settings;
private:
         SelectLabel *base_set;
         SelectLabel *play_set;
         SelectLabel *about_set;
         SelectLabel *btn_save;
         SelectLabel *btn_exit;
 QList<SelectLabel*> *label_sores;
    base_set_weidget *base_set_widget;
     play_set_widget *play_set_widget_;
        about_widget *about_widget_;

       QVBoxLayout *left_layout;
       QHBoxLayout *down_layout;
    const QStringList setting_strs{"基本设置","播放设置","关于","保存","退出"};
};


#endif // MAIN_MENU_H
