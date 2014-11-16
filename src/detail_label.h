/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/14
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef DETAILLABEL_H
#define DETAILLABEL_H

#include <QLabel>
class QHBoxLayout;
class QVBoxLayout;
class QMouseEvent;

class Label final : public QLabel
{
    Q_OBJECT
signals:
    void clicked();
public:
    explicit Label(QWidget * parent = 0);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
};


class DetailLabel : public QWidget
{
    Q_OBJECT
signals:
    void url_triggered(QString/*name*/,QString/*url*/);
public:
    DetailLabel(QWidget *parent = 0);
    DetailLabel(QPixmap, QString, QString, QWidget *parent = 0);
    virtual ~DetailLabel();

    inline void set_Pixmap(const QPixmap& img);
    inline void set_Url(const QString& url);
    inline void set_Title(const QString& title);
    inline QString text(){return lblTitle_->text();}

private slots:
    void this_url_triggered();

private:
          Label *lblImg_;
          Label *lblTitle_;
        QString  url_;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
};



#endif // DETAILLABEL_H
