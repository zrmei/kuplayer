/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/
#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H

#include <QPushButton>

class PushButton final : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = 0);
    ~PushButton();
    void setPicName(QString pic_name);

protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *);

private:
    //枚举按钮的几种状态
    enum ButtonStatus{NORMAL, ENTER, PRESS, NOSTATUS};
    ButtonStatus status;
    QString pic_name;

    int btn_width; //按钮宽度
    int btn_height; //按钮高度
    bool is_left_pressed; //按钮左键是否按下
};

#endif // PUSH_BUTTON_H
