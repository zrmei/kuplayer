/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef LIST_WIDGET_H
#define LIST_WIDGET_H
#include "common.h"
#include <QScrollArea>
class QHBoxLayout;

KUPLAYER_NAMESPACE_BEGIN //namespace begin
class DetailLabel;
class SelectLabel;

struct AreaWidget_Impl;
struct ListWidget_Impl;

class mScrollArea : public QScrollArea
{
    Q_OBJECT
signals:
    void load_next_page();
public:
    mScrollArea(QWidget *parent=0);
protected:
    virtual void wheelEvent(QWheelEvent *);
};

class AreaWidget : public QWidget
{
    Q_OBJECT
signals:
    void load_next_page();
public:
    explicit AreaWidget(QWidget *parent=0);
    virtual ~AreaWidget();
    void addDetailLabel(DetailLabel *);
    void reset();

private:
    std::shared_ptr<AreaWidget_Impl> pImpl;
};

class ListWidget final : public QWidget
{
    Q_OBJECT
signals:
    /* 某一个标签被触发，返回当前视频类型（如电影）、类型（如时间）、具体的类型（如2014）*/
    void clicked(CLASS,int,QString);
    /*滚动区域的触发，返回的是当前的页面的类型（如电影）*/
    void load_next_page(CLASS);
public:
    explicit ListWidget(CLASS type,QWidget *parent = 0);
    ~ListWidget();
    inline void addDetailLabel(DetailLabel *label){
        down_list_widget->addDetailLabel(label);
    }
    inline void reset(){ down_list_widget->reset(); }
private slots:
    void on_locate_clicked(QString,QString);
    void on_type_clicked(QString,QString);
    void on_time_clicked(QString,QString);

    inline void on_nextpage_clicked(){ emit load_next_page(type_); }

private:
    void init_locate(QHBoxLayout *locate_layout);
    void init_type(QHBoxLayout *type_layout);
    void init_time(QHBoxLayout *time_layout);

private:
     AreaWidget *down_list_widget;
     CLASS type_;
     std::shared_ptr<ListWidget_Impl> pImpl;
};

KUPLAYER_NAMESPACE_END //namespace end
#endif // LIST_layout_H
