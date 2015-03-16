/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/14
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef DETAILLABEL_H
#define DETAILLABEL_H
#include "common.h"

#include <QLabel>
class QMouseEvent;

KUPLAYER_NAMESPACE_BEGIN //namespace begin

class Label : public QLabel
{
    Q_OBJECT
signals:
    void clicked();
public:
    explicit Label(QWidget *parent = 0);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
};

class DetailLabel : public QWidget
{
    Q_OBJECT
signals:
    void url_triggered(QString/*name*/, QString/*url*/);
public:
    DetailLabel(QWidget *parent = 0);
    DetailLabel(QPixmap, const QString &, const QString &, QWidget *parent = 0);
    virtual ~DetailLabel();

    inline void set_Pixmap(QPixmap &&img);
    inline void set_Url(const QString &url);
    inline void set_Title(const QString &title);
    QString text() const;

private slots:
    void this_url_triggered();

private:
    struct DetailLabel_Impl;
    std::unique_ptr<DetailLabel_Impl> pImpl;
};


KUPLAYER_NAMESPACE_END //namespace end
#endif // DETAILLABEL_H
