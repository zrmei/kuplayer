/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef TITLE_WIDGET_H
#define TITLE_WIDGET_H
#include "common.h"
#include <QList>
#include <QLabel>

#define TITLE_WIDGET_HEIGHT 72
#define TITLE_WIDGET_WIDTH 100
#define TITLE_CONTROL_HEIGHT 29
class QMouseEvent;

KUPLAYER_NAMESPACE_BEGIN //namespace begin

class TypeLabel final : public QLabel
{
    Q_OBJECT
public:
    explicit TypeLabel(QString title, QWidget *paren = 0);
    virtual ~TypeLabel();
    void setMousePress(bool mouse_press);
signals:
    void clicked(QString);
protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *event);
private:
    void painterInfo(int, int, int);

private:
    bool mouse_hover {false};
    bool mouse_pressed {false};
};

class TitleWidget : public QWidget
{
    Q_OBJECT
signals:
    void skin_clicked();
    void min_clicked();
    void close_clicked();
    void menu_clicked();
    void ture_page(int);
public:
    explicit TitleWidget(QWidget *parent_ = 0);
    virtual ~TitleWidget();
    void set_text(QString);
    QString get_text() const;
public slots:
    void on_turepage_triggered(int);
    void on_turepage_triggered(QString);
private:
    QStringList down_title {
        tr("TVShow"), tr("Movies"), tr("Variety"),
        tr("Music"), tr("Cartoon"), tr("Player")
    };
    struct TitleWidget_Impl;
    std::shared_ptr<TitleWidget_Impl> pImpl;

};

KUPLAYER_NAMESPACE_END // namespace end
#endif // TITLE_WIDGET_H
