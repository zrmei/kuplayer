#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include "common.h"
#include "shadow_widget.h"
class PushButton;
class QHBoxLayout;
class QVBoxLayout;
class QStackedWidget;

struct conf_info
{
    QString default_format;

};

class down_widget_ : public QWidget
{
    Q_OBJECT
signals:
    void changed(bool);
public:
    explicit down_widget_(QWidget *parent = 0);
public:
    conf_info setting;
private:
    QVBoxLayout *left_layout;
    QStackedWidget *right_layout;
    QHBoxLayout *down_layout;
};

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
};

#endif // MAIN_MENU_H
