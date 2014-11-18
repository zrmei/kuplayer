/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/21
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "common.h"
#include "detail_label.h"
#include "skin_widget.h"
#include "push_button.h"

#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include <QDir>

SkinWidget::SkinWidget(QWidget *parent)
    : ShadowWidget(parent)
{
    setAttribute(Qt::WA_QuitOnClose,false);
    setWindowModality(Qt::ApplicationModal);

    QHBoxLayout *up_title_layout = new QHBoxLayout;
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
    scroll_layout->setContentsMargins(0,0,0,0);
    scroll_layout->setSpacing(2);
    viewWidgetContents->setLayout(scroll_layout);
    view->setWidget(viewWidgetContents);

    main_layout->addLayout(up_title_layout);
    main_layout->addWidget(view);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(5,5,5,5);
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::Background, QColor(230, 230, 230));
    viewWidgetContents->setPalette(text_palette);

    setMinimumSize(800,430);
    find_file(PIC_PATH);
}

void SkinWidget::find_file(QString path)
{
    QDir picdir(path);
    QStringList filters;
    filters << "*.bmp" << "*.jpg" << "*.png"<<"*.gif";
    picdir.setNameFilters(filters);
    if (!picdir.exists())
        return;

    picdir.setFilter(QDir::Dirs|QDir::Files);
    picdir.setSorting(QDir::DirsFirst);
    QFileInfoList list = picdir.entryInfoList();

    for_each(list.begin(),list.end(),
             [&](QFileInfoList::value_type fileInfo){
        if(pic_list.count(fileInfo.fileName())
                || (fileInfo.fileName()==".")
                || (fileInfo.fileName()=="..") ){
            return;
        }else{
            pic_list.append(fileInfo.fileName());
            init_skin(fileInfo.fileName());
        }
    });
}

void SkinWidget::init_skin(QString name)
{
    static int row{0},col{0};
    QString pic = PIC_PATH + name;
    auto *l = new DetailLabel(QPixmap(pic).scaled(QSize(170,96)),QString(),name);
    connect(l,SIGNAL(url_triggered(QString,QString)),
            this,SLOT(on_url_triggered(QString,QString)));
    label_store.append(l);
    scroll_layout->addWidget(l,row,col);
    ++col;
    if(col == 4){
        ++row;
        col = 0;
    }
}

void SkinWidget::on_url_triggered(QString, QString index)
{
    change_skin(index);
    emit skin_change_clicked(index);
}

void SkinWidget::on_this_show()
{
    if(isHidden()){
        QPoint pos_ = QCursor::pos();
        move(pos_.x()+30,pos_.y()+120);
        show();
        find_file(PIC_PATH);
    }else{
        hide();
    }
}
