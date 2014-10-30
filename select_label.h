/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef SELECT_LABEL_H
#define SELECT_LABEL_H

#include <QLabel>

class SelectLabel final : public QLabel
{
    Q_OBJECT
signals:
    void be_selected(QString,QString);
public:
    SelectLabel(QString name,QString url="",QWidget *parent=0);
    virtual ~SelectLabel();
    void set_selected(bool);
protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *ev);
private:
    bool mouse_pressed{false};
    bool be_select{false};
    QString url_;
};
#endif // SELECT_LABEL_H
