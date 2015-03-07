/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/22
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef PLAY_LIST_WIDGET_H
#define PLAY_LIST_WIDGET_H

#include "shadow_widget.h"
#include <QList>

KUPLAYER_NAMESPACE_BEGIN //namespace begin
class PushButton;
class SelectLabel;

typedef QList<std::tuple<QString,QString,QString>> list_map;

class PlayListWidget : public ShadowWidget
{
    Q_OBJECT
signals:
    void click(QString,QString);
public:
    explicit PlayListWidget(QWidget *parent = 0);
    ~PlayListWidget();
    void sort(const QStringList &list);
    QList<QAction*> init_action();
    bool IsEnd{false};
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
    int col{1};
    int currentIndex{0};
    list_map  play_list;
    struct PlayListWidget_Impl;
    std::shared_ptr<PlayListWidget_Impl> pImpl;
};

KUPLAYER_NAMESPACE_END // namespace end
#endif // PLAY_LIST_WIDGET_H
