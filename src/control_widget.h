/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef CONTROL_WIDGET_H
#define CONTROL_WIDGET_H
#include "common.h"

#include <QWidget>
class QAction;

KUPLAYER_NAMESPACE_BEGIN //namespace begin


class ControlWidget : public QWidget
{
    Q_OBJECT
signals:
    void backward_clicked();
    void stop_clicked();
    void play_pause_clicked(bool);
    void foreward_clicked();
    void xuan_ji_clcked(QString, QString);
    void vol_up_clicked();
    void vol_down_clicked();
    void vol_mute_clicked();
public:
    explicit ControlWidget(QWidget *parent = 0);
    virtual ~ControlWidget();
    QList<QAction *> reg_actions();
    bool isRuning {false};
public slots:
    void on_play_pause_triggered(bool);
    void on_time_changed(qint64);
    void on_douration_changed(qint64);
private:
    void init_actions();
    struct ControlWidget_Impl;
    std::shared_ptr<ControlWidget_Impl> pImpl;
};

KUPLAYER_NAMESPACE_END //namespace end
#endif // CONTROL_WIDGET_H
