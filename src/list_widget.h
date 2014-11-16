/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef LIST_WIDGET_H
#define LIST_WIDGET_H

#include <QScrollArea>

class DetailLabel;
class QStackedWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class SelectLabel;
typedef unsigned int CLASS;

class mScrollArea final : public QScrollArea
{
    Q_OBJECT
signals:
    void load_next_page();
public:
    mScrollArea(QWidget *parent=0);
protected:
    virtual void wheelEvent(QWheelEvent *);
};

class AreaWidget final : public QWidget
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
    QGridLayout *scroll_layout;
    mScrollArea *view;
        QWidget *viewWidgetContents;

    int row{0},col{0};
    QList<DetailLabel*> *label_stores;
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
    void locate_only_one(QString,QString);
    void type_only_one(QString,QString);
    void time_only_one(QString,QString);

    inline void load_next_page_(){ emit load_next_page(type_); }

private:
    void init_locate(CLASS);
    void init_type(CLASS);
    void init_time();

private:
    QHBoxLayout *locate_layout;
    QHBoxLayout *type_layout;
    QHBoxLayout *time_layout;
     AreaWidget *down_list_widget;
    QVBoxLayout *main_vlayout;
    CLASS type_;
    /*存储指针，退出时释放资源*/
    QList<SelectLabel*> *locate_labels;
    QList<SelectLabel*> *type_labels;
    QList<SelectLabel*> *time_labels;

    QStringList locate_name;
    QStringList type_name;
    QStringList time_name{"全部","2014","2013","2012","2010","00年代","90年代","80年代","70年代"};
};

#endif // LIST_layout_H
