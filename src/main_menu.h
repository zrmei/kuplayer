/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/24
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include "common.h"
#include "shadow_widget.h"

class QStackedWidget;

KUPLAYER_NAMESPACE_BEGIN //namespace begin

struct conf_info
{
    QString default_video_format;
       bool start_when_pc_on;
       bool close_all;
       bool min_or_close;
       bool auto_play_next;
       bool language;
};

class MenuWidget : public ShadowWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = 0);
    ~MenuWidget();
public slots:
    void on_showed();
    void show_about();
    void init_setting(conf_info *);
private:
    struct MenuWidget_Impl;
    std::shared_ptr<MenuWidget_Impl> pImpl;
};

/**********************************************/
class down_widget_ : public QWidget
{
    Q_OBJECT
public:
    explicit down_widget_(QWidget *parent = 0);
    ~down_widget_();
    QStackedWidget* operator->(){ return right_widget;}
public slots:
    void init_setting(conf_info *);
    void be_selected(QString,QString);
    void btn_selected(QString,QString);
    void LanguageChanged(int);

private:
    QStackedWidget *right_widget;
    struct down_widget_Impl;
    std::shared_ptr<down_widget_Impl> pImpl;
};

KUPLAYER_NAMESPACE_END //namespace end
#endif // MAIN_MENU_H
