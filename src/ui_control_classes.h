/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/29
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef UI_CONTROL_CLASSES_H
#define UI_CONTROL_CLASSES_H

#include "common.h"

#include <QWidget>
#include "shadow_widget.h"
#include "ui_base_set_weidget.h"
#include "ui_play_set_widget.h"
#include "ui_about_widget.h"

KUPLAYER_NAMESPACE_BEGIN //namespace begin

/**********************************************/
class base_set_weidget : public QWidget
{
    Q_OBJECT
public:
    explicit base_set_weidget(QWidget *parent = 0);
    ~base_set_weidget();

    Ui::base_set_weidget *ui;
};

/**********************************************/

class play_set_widget : public QWidget
{
    Q_OBJECT
    friend class down_widget_;
public:
    explicit play_set_widget(QWidget *parent = 0);
    ~play_set_widget();

    Ui::play_set_widget *ui;
};

/**********************************************/

class about_widget : public QWidget
{
    Q_OBJECT

public:
    explicit about_widget(QWidget *parent = 0);
    ~about_widget();
    Ui::about_widget *ui;
private slots:
    void on_label_linkActivated(const QString &link);
};

KUPLAYER_NAMESPACE_END // namespace end
#endif // UI_CONTROL_CLASSES_H
