/*************************************************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/14
**************************************************************************/

#include "common.h"

#include "push_button.h"
#include "play_list_widget.h"
#include "select_label.h"

#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>


PlayListWidget::PlayListWidget(QWidget *parent)
    : ShadowWidget(parent)
    , label_store(new QList<SelectLabel*>())
{
    setAttribute(Qt::WA_QuitOnClose,false);
    up_title_layout = new QHBoxLayout;
    set_no_margin(up_title_layout);

    btn_close = new PushButton;
    btn_close->setPicName(":/sysbutton/close");
    connect(btn_close,SIGNAL(clicked()),this,SLOT(this_hide()));
    up_title_layout->addWidget(btn_close,0,Qt::AlignTop);
    up_title_layout->addStretch();
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    view = new QScrollArea;
    view->setWidgetResizable(true);
    view->setContentsMargins(0,0,0,0);

    viewWidgetContents = new QWidget(view);
    scroll_layout = new QGridLayout();
    QVBoxLayout *tmp_layout = new QVBoxLayout(viewWidgetContents);
    scroll_layout->setContentsMargins(0,0,0,0);
    tmp_layout->addLayout(scroll_layout);
    tmp_layout->addStretch();

    view->setWidget(viewWidgetContents);
    main_layout->addLayout(up_title_layout);
    main_layout->addWidget(view);
    main_layout->setSpacing(0);
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::Background, QColor(230, 230, 230));
    viewWidgetContents->setPalette(text_palette);
    main_layout->setContentsMargins(SHADOW_WIDTH,SHADOW_HEIGHT,SHADOW_WIDTH,SHADOW_HEIGHT);

    viewWidgetContents->setFixedWidth(380);
    this->setMinimumSize(400,300);
}
PlayListWidget::~PlayListWidget()
{
    delete_list(label_store);
    delete btn_close;
    delete up_title_layout;
    delete scroll_layout;
    delete viewWidgetContents;
    delete view;
}

void PlayListWidget::sort(QStringList& list)
{
    QString first;
    QString second;
    QStringList tmp;
    auto comp = [](QString::value_type it){return it.isDigit();};
    for(int i=0; i<list.size();++i){
        second.clear();
        tmp = list[i].split("$$");
        first = tmp[0];

        auto j = find_if(first.begin(),first.end(),comp);
        auto k = find_if_not(j,first.end(),comp);
        second.insert(0,j,k-j);
        play_list.append(list_map{std::make_tuple<QString,QString,QString>(
                                      QString("%1").arg(second.toInt(),10,10,QChar('0')),
                                      std::move(first),
                                      std::move(tmp[1]))});
    }
    std::sort(play_list.begin(),play_list.end(),
              [](list_map::const_iterator::value_type _first,
              list_map::const_iterator::value_type _second){
        return std::get<0>(_first) < std::get<0>(_second);
    });
}

void PlayListWidget::setList(int,QStringList list)
{
    play_list.clear();
    while (label_store->size()){
        delete label_store->at(0);
        label_store->removeAt(0);
    }
    if(list.isEmpty()){
        return;
    }
    sort(list);
    int row{0},ccol{0};

    auto f = [&](list_map::value_type i){
        if(ccol > col){ ++row; ccol = 0; }
        SelectLabel *sl = new SelectLabel(std::get<1>(i),std::get<2>(i));
        connect(sl,SIGNAL(be_selected(QString,QString)),this,SLOT(this_click(QString,QString)));
        label_store->append(sl);
        scroll_layout->addWidget(sl,row,ccol);
        sl->setFixedHeight(32);
        ++ccol;
    };

    f( *(play_list.begin()) );

    label_store->at(0)->adjustSize();
    if(label_store->at(0)->width() > 50)
        col = 0;
    else
        col = 360 / (label_store->at(0)->width()+15)-1;

    for_each(play_list.begin()+1,play_list.end(),f);

}

void PlayListWidget::show_xuan_ji(QString,QString)
{
    if(this->isHidden()){
        QPoint pos_ = QCursor::pos();
        this->move(pos_.x()-400,pos_.y()-300);
        this->show();
    }else{
        this->hide();
    }
}

void PlayListWidget::this_click(QString name,QString url)
{
    for(int i=0;i<play_list.size();++i){
        if(std::get<1>(play_list[i]) == name){
            label_store->at(i)->set_selected(true);
            emit click(name,url);
            return;
        }
    }

}

void PlayListWidget::this_hide()
{
    this->hide();
}

void PlayListWidget::change_skin(QString index)
{
    skin=index;
    update();
}

void PlayListWidget::leaveEvent(QEvent *)
{
    QTimer::singleShot(800,this,SLOT(hide()));
}
