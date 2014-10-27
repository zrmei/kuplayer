#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "common.h"
#include "shadow_widget.h"

class PushButton;
class SelectLabel;
class QHBoxLayout;
class QVBoxLayout;
class QStackedWidget;

namespace Ui {
class base_set_weidget;
class play_set_widget;
class about_widget;
}

class down_widget_;

struct conf_info
{
    QString default_video_format;
       bool start_when_pc_on;
       bool close_all;
       bool min_or_close;

};
namespace  conf {
static conf_info settings{"",false,true,false};
}


class MenuWidget : public ShadowWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = 0);

public slots:
    void this_show();

private:
     PushButton *btn_close;
    QHBoxLayout *up_title_layout;
    down_widget_*down_widget;
};


class down_widget_ : public QWidget
{
    Q_OBJECT
    const QStringList setting_strs{"基本设置","播放设置","关于","保存","退出"};
signals:
    void save_it_or_not(bool);
public:
    explicit down_widget_(QWidget *parent = 0);
    QStackedWidget* operator->(){ return right_widget;}
public:
    QStackedWidget *right_widget;
private slots:
    void be_selected(QString,QString);
    void btn_selected(QString,QString);
private:
    SelectLabel *base_set;
    SelectLabel *play_set;
    SelectLabel *about_set;
    SelectLabel *btn_save;
    SelectLabel *btn_exit;

       QVBoxLayout *left_layout;
       QHBoxLayout *down_layout;
};

/**********************************************/
class base_set_weidget : public QWidget
{
    Q_OBJECT
public:
    explicit base_set_weidget(QWidget *parent = 0);
    ~base_set_weidget();

private slots:
    void on_checkBox_start_clicked(bool checked);
    void on_checkBox_close_clicked(bool checked);
    void radioButtons_clicked();

private:
    Ui::base_set_weidget *ui;
};

/**********************************************/

class play_set_widget : public QWidget
{
    Q_OBJECT
public:
    explicit play_set_widget(QWidget *parent = 0);
    ~play_set_widget();
private:
    Ui::play_set_widget *ui;
};

/**********************************************/

class about_widget : public QWidget
{
    Q_OBJECT
public:
    explicit about_widget(QWidget *parent = 0);
    ~about_widget();
private:
    Ui::about_widget *ui;
};

/**********************************************/


#endif // MAIN_MENU_H
