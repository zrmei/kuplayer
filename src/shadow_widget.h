/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef SHADOW_WIDGET_H
#define SHADOW_WIDGET_H

#include "common.h"

#include <QWidget>
#define VALUE_DIS 5
class QDesktopWidget;
#define TITLE_CONTROL_HEIGHT 29
class ShadowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShadowWidget(QWidget *parent = 0);
    virtual ~ShadowWidget();
public slots:
    void change_skin(QString);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *event);
    bool is_full_screen{false};
    inline QString get_skin()const{return skin.split('/').last();}
private:
    void drawWindowShadow(QPainter &painter);
private:
    QString skin;
    bool is_max{false};
    QRect original_rect;
    bool isLeftPressDown{false};
    QPoint m_ptPressGlobal;
    QDesktopWidget *desk;
    std::shared_ptr<QList<QPixmap>> pixmaps;
};

#ifdef CAN_RESIZE
class MainWidget : public ShadowWidget
{
    Q_OBJECT
    typedef unsigned char Direction;
public:
    explicit MainWidget(QWidget *parent = 0);
    virtual ~MainWidget();

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
private:
    void region(const QPoint &cursorPoint);
    void SetCursorStyle(const QPoint &cursorGlobalPoint);
    void SetDrayMove(QPoint gloPoint);
private:
    bool isLeftPressDown{false};
    QPoint m_ptPressGlobal{0,0};
    Direction dir{NONE};
    const Direction
    TOP = 0x01,BOTTOM = 0x02,LEFT = 0x04,RIGHT = 0x08,LEFTTOP = 0x05,
    LEFTBOTTOM = 0x06,RIGHTBOTTOM = 0x0A,RIGHTTOP = 0x09,NONE = 0x00;

};
#endif// CAN_RESIZE
#endif // SHADOW_WIDGET_H
