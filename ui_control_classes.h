/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/29
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/

#ifndef UI_CONTROL_CLASSES_H
#define UI_CONTROL_CLASSES_H
#include <QWidget>
#include "shadow_widget.h"
#include "ui_base_set_weidget.h"
#include "ui_play_set_widget.h"
#include "ui_about_widget.h"

/**********************************************/
class base_set_weidget : public QWidget
{
    Q_OBJECT
    friend class down_widget_;
public:
    explicit base_set_weidget(QWidget *parent = 0);
    ~base_set_weidget();
private:
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
private:
    Ui::play_set_widget *ui;
};

/**********************************************/

class about_widget : public QWidget
{
    Q_OBJECT
    friend class down_widget_;
public:
    explicit about_widget(QWidget *parent = 0);
    ~about_widget();
private slots:
    void on_label_linkActivated(const QString &link);

private:
    Ui::about_widget *ui;
};


#endif // UI_CONTROL_CLASSES_H
