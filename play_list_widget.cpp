/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/22
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/

#include "common.h"

#include "push_button.h"
#include "play_list_widget.h"
#include "select_label.h"

#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QAction>

PlayListWidget::PlayListWidget(QWidget *parent)
    : ShadowWidget(parent)
    , label_store(new QList<SelectLabel*>())
{
    setAttribute(Qt::WA_QuitOnClose,false);
    setWindowModality(Qt::ApplicationModal);

    up_title_layout = new QHBoxLayout;
    set_no_margin(up_title_layout);

    btn_close = new PushButton;
    btn_close->setPicName(":/sysbutton/close");
    connect(btn_close,SIGNAL(clicked()),this,SLOT(hide()));
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
    main_layout->setContentsMargins(5,5,5,5);

    viewWidgetContents->setFixedWidth(380);
    setFixedSize(400,300);

    play_next_key = new QAction(this);
    play_next_key->setShortcut(QKeySequence(Qt::Key_PageDown));
    connect(play_next_key,SIGNAL(triggered()),SLOT(play_next_video()));

    play_prev_key = new QAction(this);
    play_prev_key->setShortcut(QKeySequence(Qt::Key_PageUp));
    connect(play_prev_key,SIGNAL(triggered()),SLOT(play_prev_video()));

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
              [](list_map::const_iterator::value_type lvalue,
              list_map::const_iterator::value_type rvalue){
        return std::get<0>(lvalue) < std::get<0>(rvalue);
    });
}

void PlayListWidget::setList(int,QStringList list)
{
    play_list.clear();
    currentIndex = 0;
    while (label_store->size()){
        delete label_store->at(0);
        label_store->removeAt(0);
    }
    if(list.isEmpty()){
        return;
    }
    sort(list);
    int row{0},ccol{0};

    auto func_add_label = [&](list_map::value_type i){
        if(ccol > col){ ++row; ccol = 0; }
        SelectLabel *sl = new SelectLabel(std::get<1>(i),std::get<2>(i));
        connect(sl,SIGNAL(be_selected(QString,QString)),
                this,SLOT(this_click(QString,QString)));
        label_store->append(sl);
        scroll_layout->addWidget(sl,row,ccol);
        sl->setFixedHeight(32);
        ++ccol;
    };
    func_add_label( *(play_list.begin()) );

    label_store->at(0)->adjustSize();
    if(label_store->at(0)->width() > 50){
        col = 0;
    }else{
        col = 360 / (label_store->at(0)->width()+15)-1;
    }
    for_each(play_list.begin()+1,play_list.end(),func_add_label);
}

void PlayListWidget::show_xuan_ji(QString,QString)
{
    if(isHidden()){
        QPoint pos_ = QCursor::pos();
        move(pos_.x()-400,pos_.y()-300);
        show();
    }else{
        hide();
    }
}

void PlayListWidget::this_click(QString name,QString url)
{
    for(int i=0;i<play_list.size();++i){
        if(std::get<1>(play_list[i]) == name){
            currentIndex = i;
            label_store->at(i)->set_selected(true);
            emit click(name,url);
            return;
        }
    }
}
void PlayListWidget::play_next_video()
{
    ++currentIndex;
    if(currentIndex >= play_list.size()){
        currentIndex = play_list.size()-1;
        return;
    }
    label_store->at(currentIndex)->set_selected(true);
    emit click(std::get<1>(play_list[currentIndex]),
               std::get<2>(play_list[currentIndex]));
}

void PlayListWidget::play_prev_video()
{
    --currentIndex;
    if(currentIndex < 0){
        currentIndex = 0;
        return;
    }
    label_store->at(currentIndex)->set_selected(true);
    emit click(std::get<1>(play_list[currentIndex]),
               std::get<2>(play_list[currentIndex]));
}


void PlayListWidget::leaveEvent(QEvent *)
{
    QTimer::singleShot(800,this,SLOT(hide()));
}
