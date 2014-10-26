/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/
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
public slots:
    void trigger_play_pause(bool);
    void on_play_pause();
    void setTime(qint64);
public:
    explicit ControlWidget(QWidget *parent = 0);
    virtual ~ControlWidget();
    bool isRuning{false};
    inline QList<QAction*> init_action()
    {
        return{{backward_key,stop_key,pause_key,foreward_key}};
    }
private:
    QAction *backward_key;
    QAction *stop_key;
    QAction *pause_key;
    QAction *foreward_key;
          QLabel *time_;
    ControlLabel *backward_;
    ControlLabel *stop_;
    ControlLabel *play_pause;
    ControlLabel *foreward_;
     SelectLabel *xuan_ji;
    //:FIXME
    /*
     * ControlLabel *vol_up;
     * ControlLabel *vol_down;
     */

};

#endif // CONTROL_WIDGET_H
