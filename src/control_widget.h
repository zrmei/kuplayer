/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef CONTROL_WIDGET_H
#define CONTROL_WIDGET_H

#include <QLabel>
class QMouseEvent;
class SelectLabel;
class QAction;

class ControlLabel final : public QLabel
{
    Q_OBJECT
signals:
    void clicked();
public:
    ControlLabel(QString name,QWidget *parent=0);
protected:
    void mousePressEvent(QMouseEvent *);
};

class ControlWidget : public QWidget
{
    Q_OBJECT
signals:
    void backward_clicked();
    void stop_clicked();
    void play_pause_clicked(bool);
    void foreward_clicked();
    void xuan_ji_clcked(QString,QString);
    void vol_up_clicked();
    void vol_down_clicked();
    void vol_mute_clicked();
public slots:
    void trigger_play_pause(bool);
    void on_play_pause();
    void setTime(qint64);
    void setDuration(qint64);
public:
    explicit ControlWidget(QWidget *parent = 0);
    virtual ~ControlWidget();
    bool isRuning{false};
    inline QList<QAction*> init_action()
    {
        return {{backward_key,stop_key,pause_key,
                        foreward_key,vol_up_key,vol_down_key}};
    }
private:
    void init_actions();
    QAction *backward_key;
    QAction *stop_key;
    QAction *pause_key;
    QAction *foreward_key;
    QAction *vol_up_key;
    QAction *vol_down_key;
    QAction *mute_key;
          QLabel *time_current;
    ControlLabel *backward_;
    ControlLabel *stop_;
    ControlLabel *play_pause;
    ControlLabel *foreward_;
          QLabel *time_all;
     SelectLabel *xuan_ji;
    //:FIXME
    /*
     * ControlLabel *vol_up;
     * ControlLabel *vol_down;
     */

};

#endif // CONTROL_WIDGET_H
