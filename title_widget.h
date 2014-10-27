/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/
#ifndef TITLE_WIDGET_H
#define TITLE_WIDGET_H

#include <QList>
#include <QLabel>

#define TITLE_WIDGET_HEIGHT 72
#define TITLE_WIDGET_WIDTH 95
#define TITLE_CONTROL_HEIGHT 29
class PushButton;
class QMouseEvent;
class QHBoxLayout;

class TypeLabel final : public QLabel
{
    Q_OBJECT
public:
    TypeLabel(QString title_name,QWidget *paren=0);
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
    void painterInfo(int top_color, int middle_color, int bottom_color);

private:
    bool mouse_hover{false};
    bool mouse_pressed{false};
};

class TitleWidget final : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent_ = 0);
    virtual ~TitleWidget();
    void set_text(QString);
    inline QString get_text(){return title->text();}
signals:
    void skin_clicked();
    void min_clicked();
    void close_clicked();
    void menu_clicked();
    void ture_page(int);



public slots:
    void turepage(QString);
private:

    PushButton *btn_min;
    PushButton *btn_close;
    PushButton *btn_skin;
    PushButton *btn_menu;
    QLabel *title;
    QLabel *logo;
    QStringList  down_title;
    QHBoxLayout *down_title_layout;
    QHBoxLayout *up_title_layout;
#ifdef HAVS_LOGO
    QHBoxLayout *right_title_layout;
#endif
    QList<TypeLabel*> *labels_store;
    QList<PushButton*> *push_button_store;

};
#endif // TITLE_WIDGET_H
