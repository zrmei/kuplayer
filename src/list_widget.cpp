/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "common.h"
#include "gloal_func.h"
#include "list_widget.h"
#include "select_label.h"
#include "detail_label.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <QMouseEvent>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QScrollBar>

struct NAMESPACE_KUPLAYER::AreaWidget::AreaWidget_Impl
{
    QGridLayout *scroll_layout;
    mScrollArea *view;
        QWidget *viewWidgetContents;

    int row{0},col{0};
    QList<DetailLabel*> *label_stores;
    
    AreaWidget_Impl()
        : scroll_layout(new QGridLayout)
        , view(new mScrollArea)
        , viewWidgetContents(new QWidget)
        , label_stores(new QList<DetailLabel*>)
    {
        view->setWidgetResizable(true);
        view->setContentsMargins(0,0,0,0);
    
        scroll_layout->setContentsMargins(0,0,0,0);
        viewWidgetContents->setLayout(scroll_layout);
        view->setWidget(viewWidgetContents);
    }
    ~AreaWidget_Impl()
    {
        delete_list(label_stores);
        delete scroll_layout;
        delete viewWidgetContents;
        delete view;
    }
};

struct NAMESPACE_KUPLAYER::ListWidget::ListWidget_Impl
{
    /*存储指针，退出时释放资源*/
    QList<SelectLabel*> *locate_labels;
    QList<SelectLabel*> *type_labels;
    QList<SelectLabel*> *time_labels;

    QStringList locate_name;
    QStringList type_name;
    QStringList time_name;
    
    ListWidget_Impl()
        : locate_labels(new QList<SelectLabel*>)
        , type_labels(new QList<SelectLabel*>)
        , time_labels(new QList<SelectLabel*>)
        , time_name({"全部","2014","2013","2012","2010","00年代","90年代","80年代","70年代"})
    {
    }
    
    ~ListWidget_Impl()
    {
        delete_list(locate_labels);
        delete_list(type_labels);
        delete_list(time_labels);
    }
};

/****************************************************************************/
mScrollArea::mScrollArea(QWidget *parent)
    : QScrollArea(parent)
{
    connect(verticalScrollBar(),SIGNAL(valueChanged(int)),SLOT(valueChanged(int)));
}
void mScrollArea::wheelEvent(QWheelEvent *ev)
{
    QScrollArea::wheelEvent(ev);
    valueChanged(verticalScrollBar()->value());
}

void mScrollArea::valueChanged(int value)
{
    static int mid_min;
    mid_min = verticalScrollBar()->maximum() * 0.95;
    if(value > 100 && value >= mid_min){
            qDebug() << "emit load_next_page: value= " << value
                        << "  mid_min=" << mid_min;
            emit load_next_page_();
    }
}

/*****************************************************************************/
AreaWidget::AreaWidget(QWidget *parent)
    : QWidget(parent)
    , pImpl(new AreaWidget_Impl())
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addWidget(pImpl->view);
    main_layout->setSpacing(0);
    
    connect(pImpl->view,SIGNAL(load_next_page_()),this,SIGNAL(load_next_page()));
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::Background, QColor(230, 230, 230));
    text_palette.setColor(QPalette::Background,QColor(255,255,255,0));
    pImpl->viewWidgetContents->setPalette(text_palette);
}
AreaWidget::~AreaWidget()
{
}

void AreaWidget::addDetailLabel(DetailLabel *label)
{
    pImpl->label_stores->append(label);
    label->setParent(qobject_cast<QWidget*>(pImpl->scroll_layout));
    pImpl->scroll_layout->addWidget(label,pImpl->row,pImpl->col);
    ++(pImpl->col);
    if(pImpl->col == 4){
        ++(pImpl->row);
        pImpl->col = 0;
    }
}

void AreaWidget::reset()
{
    pImpl->row = pImpl->col = 0;
    while(pImpl->label_stores->size()){
        delete pImpl->label_stores->at(0);
        pImpl->label_stores->removeAt(0);
    }
}

/*****************************************************************************/
ListWidget::ListWidget(CLASS type, QWidget *parent)
    : QWidget(parent)
    , down_list_widget(new AreaWidget)
    , type_(type)
    , pImpl(new ListWidget_Impl())
{
    connect(down_list_widget,&AreaWidget::load_next_page,[&](){ emit emit_next_page(type_);});
    /*以下初始化函数顺序不能改*/    
    QHBoxLayout *locate_layout = new QHBoxLayout;
    init_locate(locate_layout);
    QHBoxLayout *type_layout = new QHBoxLayout;
    init_type(type_layout);
    QHBoxLayout *time_layout = new QHBoxLayout;
    init_time(time_layout);
    
    QVBoxLayout *main_vlayout = new QVBoxLayout(this);
    main_vlayout->addLayout(locate_layout);
    main_vlayout->addLayout(type_layout);
    main_vlayout->addLayout(time_layout);
    main_vlayout->addWidget(down_list_widget);
    main_vlayout->setContentsMargins(0,0,0,0);
    setAutoFillBackground(true);
}

ListWidget::~ListWidget()
{
    delete down_list_widget;
}

void ListWidget::init_locate(QHBoxLayout *locate_layout)
{
    pImpl->locate_name<<"全部"<<"大陆"<<"香港"<<"台湾"<<"韩国"<<"美国"<<"英国";
    switch (type_) {
    case TV:
        pImpl->locate_name<<"泰国"<<"新加坡";
        break;
    case MOVIE:
        pImpl->locate_name<<"法国"<<"印度"<<"泰国"<<"其他";
        break;
    case ZONGYI:
        pImpl->locate_name<<"澳大利亚";
        break;
    case MUSIC:
        pImpl->locate_name.clear();
        pImpl->locate_name<<"全部"<<"大陆"<<"港台"<<"日韩"<<"欧美";
        break;
    default:
        pImpl->locate_name<<"日本";
        break;
    }
    for_each(pImpl->locate_name.begin(),pImpl->locate_name.end(),
             [&](QStringList::value_type locate_name){
        SelectLabel *label = new SelectLabel(locate_name,"");
        label->setFixedSize(60,25);
        pImpl->locate_labels->append(label);
        locate_layout->addWidget(label,0,Qt::AlignTop);
        connect(label,SIGNAL(be_selected(QString,QString)),
                this,SLOT(on_locate_clicked(QString,QString)));
    });
    if(pImpl->locate_labels->size()){
        pImpl->locate_labels->at(0)->set_selected(true);
    }
    locate_layout->addStretch();
    locate_layout->setContentsMargins(5,5,0,0);
}

void ListWidget::init_type(QHBoxLayout *type_layout)
{
    pImpl->type_name <<"全部";
    switch(type_){
    case TV:
        pImpl->type_name<<"古装"<<"武侠"<<"警匪"<<"军事"<<"神话"<<"科幻"<<"悬疑"
                       <<"历史"<<"都市"<<"搞笑"<<"偶像"<<"言情";
        break;
    case MOVIE:
        pImpl->type_name<<"武侠"<<"警匪"<<"犯罪"<<"科幻"<<"战争"<<"恐怖"<<"惊悚"
                       <<"动作"<<"喜剧"<<"爱情"<<"剧情"<<"冒险"<<"悬疑"<<"历史";
        break;
    case ZONGYI:
        pImpl->type_name<<"优酷出品"<<"优酷牛人"<<"脱口秀"<<"真人秀"<<"选秀"
                       <<"美食"<<"搞笑"<<"访谈"<<"纪实";
        break;
    case MUSIC:
        pImpl->type_name<<"流行"<<"摇滚"<<"舞曲"<<"电子"<<"民谣"<<"拉丁"
                       <<"爵士"<<"古典"<<"轻音乐";
        break;
    case COMIC:
        pImpl->type_name<<"热血"<<"格斗"<<"恋爱"<<"美少女"<<"校园"<<"搞笑"
                       <<"机战"<<"真人"<<"青春"<<"魔法"<<"神话";
        break;
    default:
        break;
    }
    for_each(pImpl->type_name.begin(),pImpl->type_name.end(),
             [&](QStringList::value_type type_name){
        SelectLabel *label = new SelectLabel(type_name,"");
        label->setFixedSize(60,25);
        pImpl->type_labels->append(label);
        type_layout->addWidget(label);
        connect(label,SIGNAL(be_selected(QString,QString)),
                this,SLOT(on_type_clicked(QString,QString)));
    });
    if(pImpl->type_labels->size()){
        pImpl->type_labels->at(0)->set_selected(true);
    }
    type_layout->addStretch();
    type_layout->setContentsMargins(5,0,0,0);
}

void ListWidget::init_time(QHBoxLayout *time_layout)
{
    for_each(pImpl->time_name.cbegin(),pImpl->time_name.cend(),
             [&](const QStringList::value_type& time_name){
        SelectLabel *label = new SelectLabel(time_name,"");
        label->setFixedSize(60,25);
        pImpl->time_labels->append(label);
        time_layout->addWidget(label,0,Qt::AlignBottom);
        connect(label,SIGNAL(be_selected(QString,QString)),
                this,SLOT(on_time_clicked(QString,QString)));
    });
    if(pImpl->time_labels->size()){
        pImpl->time_labels->at(0)->set_selected(true);
    }
    time_layout->addStretch();
    time_layout->setContentsMargins(5,0,0,0);
}

void ListWidget::on_locate_clicked(QString locate,QString)
{
    int index = pImpl->locate_name.indexOf(locate);
    for(int i=0; i<pImpl->locate_name.size(); ++i){
        pImpl->locate_labels->at(i)->set_selected(false);
    }
    pImpl->locate_labels->at(index)->set_selected(true);

    emit clicked(type_,0,locate);
}

void ListWidget::on_type_clicked(QString type,QString)
{
    int index = pImpl->type_name.indexOf(type);
    for(int i=0; i<pImpl->type_name.size(); ++i){
        pImpl->type_labels->at(i)->set_selected(false);
    }
    pImpl->type_labels->at(index)->set_selected(true);

    emit clicked(type_,1,type);
}

void ListWidget::on_time_clicked(QString time,QString)
{
    int index = pImpl->time_name.indexOf(time);
    for(int i=0; i<pImpl->time_name.size(); ++i){
        pImpl->time_labels->at(i)->set_selected(false);
    }
    pImpl->time_labels->at(index)->set_selected(true);

    emit clicked(type_,2,time);
}
