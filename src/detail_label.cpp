/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/14
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "detail_label.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QApplication>
#include <QPixmap>
#include <QDebug>

/****************************************************************************/

Label::Label(QWidget *parent)
    :   QLabel(parent)
{
}

void Label::mouseDoubleClickEvent(QMouseEvent *ev)
{
    QLabel::mouseDoubleClickEvent(ev);
    if (ev->button() == Qt::LeftButton)
        emit clicked();
}
void Label::enterEvent(QEvent *ev)
{
    QLabel::enterEvent(ev);
    this->setCursor(QCursor(Qt::PointingHandCursor));
}

void Label::leaveEvent(QEvent *ev)
{
    QLabel::leaveEvent(ev);
    this->setCursor(QCursor(Qt::ArrowCursor));
}

/****************************************************************************/

DetailLabel::DetailLabel(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    verticalLayout = new QVBoxLayout;
    verticalLayout->setSpacing(0);

    lblImg_ = new Label;
    lblImg_->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(lblImg_);

    lblTitle_= new Label;
    lblTitle_->setMaximumHeight(36);
    lblTitle_->setAlignment(Qt::AlignHCenter| Qt::AlignBottom);

    verticalLayout->addWidget(lblTitle_);
    horizontalLayout->addLayout(verticalLayout);

    connect(lblImg_,SIGNAL(clicked()),this,SLOT(this_url_triggered()));
    connect(lblTitle_,SIGNAL(clicked()),this,SLOT(this_url_triggered()));
}

DetailLabel::DetailLabel(QPixmap img, QString title, QString url, QWidget *parent)
    : DetailLabel(parent)
{
    set_Pixmap(std::move(img));
    set_Title(title);
    set_Url(url);
}

DetailLabel::~DetailLabel()
{
//    qDebug() <<"DetailLabel("<<lblTitle_->text()<<") deleted";
    delete lblImg_;
    delete lblTitle_;
    delete verticalLayout;
}
void DetailLabel::this_url_triggered()
{
    emit url_triggered(lblTitle_->text(),url_);
}

void DetailLabel::set_Pixmap(QPixmap &&img)
{
    lblImg_->setPixmap(img);
    setFixedSize(img.width()+20,img.height()+36);
}

void DetailLabel::set_Title(const QString &title)
{
    lblTitle_->setText(title);
    lblTitle_->setToolTip(title);
    lblImg_->setToolTip(title);
}

void DetailLabel::set_Url(const QString &url)
{
    url_ = url;
}
