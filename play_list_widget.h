/*************************************************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/14
**************************************************************************/
#ifndef PLAY_LIST_WIDGET_H
#define PLAY_LIST_WIDGET_H

#include "shadow_widget.h"
#include <QList>
class QGridLayout;
class QHBoxLayout;
class QScrollArea;
class PushButton;
class SelectLabel;

class PlayListWidget final : public ShadowWidget
{
    Q_OBJECT
signals:
    void click(QString,QString);
public:
    explicit PlayListWidget(QWidget *parent = 0);
    ~PlayListWidget();
    typedef QList<std::tuple<QString,QString,QString>> list_map;
    void sort(QStringList &list);
public slots:
    void setList(int,QStringList);
    void show_xuan_ji(QString, QString);
    void change_skin(QString);
protected:
    void leaveEvent(QEvent *);
private slots:
    void this_click(QString name, QString url);
    void this_hide();
private:

        QGridLayout *scroll_layout;
        QScrollArea *view;
            QWidget *viewWidgetContents;
         PushButton *btn_close;
        QHBoxLayout *up_title_layout;
QList<SelectLabel*> *label_store;
           list_map  play_list;
    int col{1};
};

#endif // PLAY_LIST_WIDGET_H
