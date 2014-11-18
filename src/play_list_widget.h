/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/22
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef PLAY_LIST_WIDGET_H
#define PLAY_LIST_WIDGET_H

#include "shadow_widget.h"
#include <QList>
class QGridLayout;
class QHBoxLayout;
class QScrollArea;
class PushButton;
class SelectLabel;

class PlayListWidget : public ShadowWidget
{
    Q_OBJECT
signals:
    void click(QString,QString);
public:
    explicit PlayListWidget(QWidget *parent = 0);
    ~PlayListWidget();
    typedef QList<std::tuple<QString,QString,QString>> list_map;
    void sort(const QStringList &list);
    inline QList<QAction*> init_action()
    {
        return {{play_next_key,play_prev_key}};
    }
public slots:
    void on_list_changed(int, const QStringList &);
    void on_xuan_ji_show(QString, QString);
    void on_playNext_clicked();
    void on_playPrev_clicked();
protected:
    void leaveEvent(QEvent *);
private slots:
    void this_click(QString name, QString url);
private:
            QAction *play_next_key;
            QAction *play_prev_key;
        QGridLayout *scroll_layout;
        QScrollArea *view;
            QWidget *viewWidgetContents;
         PushButton *btn_close;
        QHBoxLayout *up_title_layout;
QList<SelectLabel*> *label_store;
           list_map  play_list;
    int col{1};
    int currentIndex{0};
};

#endif // PLAY_LIST_WIDGET_H
