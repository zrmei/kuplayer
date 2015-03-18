/*
   Copyright (C) 2015 MeiZhaorui(Mason) <listener_mei@163.com>
   
   The File is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   The File is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with the Library; if not, see
   <http://www.gnu.org/licenses/>.
*/

#include "common.h"
#include "gloal_func.h"
#include "detail_label.h"
#include "skin_widget.h"
#include "push_button.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include <QDir>

struct SkinWidget::SkinWidget_Impl {
    QList<DetailLabel *> *label_store;
    QScrollArea *view;
    QGridLayout *scroll_layout;
    PushButton *btn_close;

    SkinWidget_Impl()
        : label_store(new QList<DetailLabel *>)
        , view(new QScrollArea)
        , scroll_layout(new QGridLayout)
        , btn_close(new PushButton)
    {}
    ~SkinWidget_Impl() {
        delete_list(label_store);
        delete view;
        delete scroll_layout;
        delete btn_close;
    }
};

SkinWidget::SkinWidget(QWidget *parent)
    : ShadowWidget(parent)
    , PIC_PATH(qApp->applicationDirPath() + "/../sources/img/skin/")
    , pImpl(new SkinWidget_Impl())
{
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowModality(Qt::ApplicationModal);
    QHBoxLayout *up_title_layout = new QHBoxLayout;
    set_no_margin(up_title_layout);
    pImpl->btn_close->setPicName(":/sysbutton/close");
    connect(pImpl->btn_close, SIGNAL(clicked()), this, SLOT(hide()));
    up_title_layout->addWidget(pImpl->btn_close, 0, Qt::AlignTop);
    up_title_layout->addStretch();
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    pImpl->view->setWidgetResizable(true);
    pImpl->view->setContentsMargins(0, 0, 0, 0);
    QWidget *viewWidgetContents = new QWidget(pImpl->view);
    pImpl->scroll_layout->setContentsMargins(0, 0, 0, 0);
    pImpl->scroll_layout->setSpacing(2);
    viewWidgetContents->setLayout(pImpl->scroll_layout);
    pImpl->view->setWidget(viewWidgetContents);
    main_layout->addLayout(up_title_layout);
    main_layout->addWidget(pImpl->view);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(5, 5, 5, 5);
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::Background, QColor(255, 255, 255, 50));
    setPalette(text_palette);
    setMinimumSize(800, 430);
    find_file(PIC_PATH);
}

void SkinWidget::find_file(QString path)
{
    QDir picdir(path);
    QStringList filters;
    filters << "*.bmp" << "*.jpg" << "*.png" << "*.gif";
    picdir.setNameFilters(filters);

    if (!picdir.exists()) {
        return;
    }

    picdir.setFilter(QDir::Dirs | QDir::Files);
    picdir.setSorting(QDir::DirsFirst);
    QFileInfoList list = picdir.entryInfoList();
    for_each(list.begin(), list.end(),
    [&](QFileInfoList::value_type fileInfo) {
        if (pic_list.count(fileInfo.fileName())
            || (fileInfo.fileName() == ".")
            || (fileInfo.fileName() == "..")) {
            return;
        } else {
            pic_list.append(fileInfo.fileName());
            init_skin(fileInfo.fileName());
        }
    });
}

void SkinWidget::init_skin(QString name)
{
    static int row {0}, col {0};
    QString pic = PIC_PATH + name;
    auto *l = new DetailLabel(QPixmap(pic).scaled(QSize(170, 96)), QString(), name);
    connect(l, SIGNAL(url_triggered(QString, QString)),
            this, SLOT(on_url_triggered(QString, QString)));
    pImpl->label_store->append(l);
    pImpl->scroll_layout->addWidget(l, row, col);
    ++col;

    if (col == 4) {
        ++row;
        col = 0;
    }
}

void SkinWidget::on_url_triggered(QString, QString index)
{
    change_skin(index);
    emit skin_change_clicked(index);
}

void SkinWidget::on_showed()
{
    if (isHidden()) {
        QPoint pos_ = QCursor::pos();
        move(pos_.x() + 30, pos_.y() + 120);
        show();
        find_file(PIC_PATH);
    } else {
        hide();
    }
}
