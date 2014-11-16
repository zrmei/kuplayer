/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "common.h"
#include "list_widget.h"
#include "select_label.h"
#include "detail_label.h"

#include <QMouseEvent>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QScrollBar>

/****************************************************************************/
mScrollArea::mScrollArea(QWidget *parent)
    : QScrollArea(parent)
{}
void mScrollArea::wheelEvent(QWheelEvent *ev)
{
    QScrollArea::wheelEvent(ev);
    if(verticalScrollBar()->value() > 100){
        int mid_min = verticalScrollBar()->maximum() * 0.95;
        if(verticalScrollBar()->value() >= mid_min){
            emit load_next_page();
        }
    }
}
/*****************************************************************************/
AreaWidget::AreaWidget(QWidget *parent)
    : QWidget(parent)
    , label_stores(new QList<DetailLabel*>)
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    view = new mScrollArea;
    connect(view,SIGNAL(load_next_page()),this,SIGNAL(load_next_page()));
    view->setWidgetResizable(true);
    view->setContentsMargins(0,0,0,0);

    viewWidgetContents = new QWidget(view);
    scroll_layout = new QGridLayout();
    scroll_layout->setContentsMargins(0,0,0,0);
    viewWidgetContents->setLayout(scroll_layout);
    view->setWidget(viewWidgetContents);

    main_layout->addWidget(view);
    main_layout->setSpacing(0);
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::Background, QColor(230, 230, 230));
    viewWidgetContents->setPalette(text_palette);
}
AreaWidget::~AreaWidget()
{
    reset();
    delete label_stores;
    delete scroll_layout;
    delete viewWidgetContents;
    delete view;
}

void AreaWidget::addDetailLabel(DetailLabel *label)
{
    label_stores->append(label);
    scroll_layout->addWidget(label,row,col);
    ++col;
    if(col == 4){
        ++row;
        col = 0;
    }
}

void AreaWidget::reset()
{
    row = col = 0;
    while(label_stores->size()){
        delete label_stores->at(0);
        label_stores->removeAt(0);
    }
}

/*****************************************************************************/
ListWidget::ListWidget(CLASS type, QWidget *parent)
    : QWidget(parent)
    , type_(type)
{
    down_list_widget = new AreaWidget;
    connect(down_list_widget,SIGNAL(load_next_page()),this,SLOT(load_next_page_()));
    /*以下初始化函数顺序不能改*/
    init_locate(type);
    init_type(type);
    init_time();

    main_vlayout = new QVBoxLayout(this);
    main_vlayout->addLayout(locate_layout);
    main_vlayout->addLayout(type_layout);
    main_vlayout->addLayout(time_layout);
    main_vlayout->addWidget(down_list_widget);
    main_vlayout->setContentsMargins(0,0,0,0);
    setAutoFillBackground(true);
}

ListWidget::~ListWidget()
{
    delete_list(locate_labels);
    delete_list(type_labels);
    delete_list(time_labels);
    delete locate_layout;
    delete type_layout;
    delete time_layout;
    delete down_list_widget;
    delete main_vlayout;
}

void ListWidget::init_locate(CLASS type)
{
    locate_name<<"全部"<<"大陆"<<"香港"<<"台湾"<<"韩国"<<"美国"<<"英国";
    switch (type) {
    case TV:
        locate_name<<"泰国"<<"新加坡";
        break;
    case MOVIE:
        locate_name<<"法国"<<"印度"<<"泰国"<<"其他";
        break;
    case ZONGYI:
        locate_name<<"澳大利亚";
        break;
    case MUSIC:
        locate_name.clear();
        locate_name<<"全部"<<"大陆"<<"港台"<<"日韩"<<"欧美";
        break;
    default:
        locate_name<<"日本";
        break;
    }
    locate_layout = new QHBoxLayout;
    locate_labels = new QList<SelectLabel*>;
    for(int i=0; i<locate_name.size(); ++i){
        SelectLabel *label = new SelectLabel(locate_name[i],"");
        label->setFixedSize(60,25);
        locate_labels->append(label);
        locate_layout->addWidget(label,0,Qt::AlignTop);
        connect(label,SIGNAL(be_selected(QString,QString)),this,SLOT(locate_only_one(QString,QString)));
    }
    if(locate_labels->size()){
        locate_labels->at(0)->set_selected(true);
    }
    locate_layout->addStretch();
    locate_layout->setContentsMargins(5,5,0,0);
}

void ListWidget::init_type(CLASS type)
{
    type_name <<"全部";
    switch(type){
    case TV:
        type_name<<"古装"<<"武侠"<<"警匪"<<"军事"<<"神话"<<"科幻"<<"悬疑"<<"历史"<<"都市"
                <<"搞笑"<<"偶像"<<"言情";
        break;
    case MOVIE:
        type_name<<"武侠"<<"警匪"<<"犯罪"<<"科幻"<<"战争"<<"恐怖"<<"惊悚"<<"动作"<<"喜剧"
                <<"爱情"<<"剧情"<<"冒险"<<"悬疑"<<"历史";
        break;
    case ZONGYI:
        type_name<<"优酷出品"<<"优酷牛人"<<"脱口秀"<<"真人秀"<<"选秀"<<"美食"<<"搞笑"<<"访谈"<<"纪实";
        break;
    case MUSIC:
        type_name<<"流行"<<"摇滚"<<"舞曲"<<"电子"<<"民谣"<<"拉丁"<<"爵士"<<"古典"<<"轻音乐";
        break;
    case COMIC:
        type_name<<"热血"<<"格斗"<<"恋爱"<<"美少女"<<"校园"<<"搞笑"<<"机战"<<"真人"<<"青春"<<"魔法"<<"神话";
        break;
    default:
        break;
    }
    type_labels = new QList<SelectLabel*>;
    type_layout = new QHBoxLayout;
    for(int i=0; i<type_name.size(); ++i){
        SelectLabel *label = new SelectLabel(type_name.at(i),"");
        label->setFixedSize(60,25);
        type_labels->append(label);
        type_layout->addWidget(label);
        connect(label,SIGNAL(be_selected(QString,QString)),this,SLOT(type_only_one(QString,QString)));
    }
    if(type_labels->size()){
        type_labels->at(0)->set_selected(true);
    }
    type_layout->addStretch();
    type_layout->setContentsMargins(5,0,0,0);
}

void ListWidget::init_time()
{
    time_labels = new QList<SelectLabel*>;
    time_layout = new QHBoxLayout;
    for(int i=0; i<time_name.size(); ++i){
        SelectLabel *label = new SelectLabel(time_name[i],"");
        label->setFixedSize(60,25);
        time_labels->append(label);
        time_layout->addWidget(label,0,Qt::AlignBottom);
        connect(label,SIGNAL(be_selected(QString,QString)),this,SLOT(time_only_one(QString,QString)));
    }
    if(time_labels->size()){
        time_labels->at(0)->set_selected(true);
    }
    time_layout->addStretch();
    time_layout->setContentsMargins(5,0,0,0);
}

void ListWidget::locate_only_one(QString locate,QString)
{
    int index = locate_name.indexOf(locate);
    for(int i=0; i<locate_name.size(); ++i){
        locate_labels->at(i)->set_selected(false);
    }
    locate_labels->at(index)->set_selected(true);

    emit clicked(type_,0,locate);
}

void ListWidget::type_only_one(QString type,QString)
{
    int index = type_name.indexOf(type);
    for(int i=0; i<type_name.size(); ++i){
        type_labels->at(i)->set_selected(false);
    }
    type_labels->at(index)->set_selected(true);

    emit clicked(type_,1,type);
}

void ListWidget::time_only_one(QString time,QString)
{
    int index = time_name.indexOf(time);
    for(int i=0; i<time_name.size(); ++i){
        time_labels->at(i)->set_selected(false);
    }
    time_labels->at(index)->set_selected(true);

    emit clicked(type_,2,time);
}











