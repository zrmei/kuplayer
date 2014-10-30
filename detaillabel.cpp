/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/14
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "detaillabel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QApplication>

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
    QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
}

void Label::leaveEvent(QEvent *ev)
{
    QLabel::leaveEvent(ev);
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

/****************************************************************************/

DetailLabel::DetailLabel(QWidget *parent)
    : QWidget(parent)
{
    horizontalLayout = new QHBoxLayout(this);
    verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(0);

    lblImg_ = new Label(this);
    lblImg_->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(lblImg_);

    lblTitle_= new Label(this);
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
    set_Pixmap(img);
    set_Title(title);
    set_Url(url);
}

DetailLabel::~DetailLabel()
{
    delete lblImg_;
    delete lblTitle_;
    delete verticalLayout;
    delete horizontalLayout;
}
void DetailLabel::this_url_triggered()
{
    emit url_triggered(lblTitle_->text(),url_);
}

void DetailLabel::set_Pixmap(const QPixmap &img)
{
    lblImg_->setPixmap(img);
    lblImg_->setFixedSize(img.size());
    setFixedSize(img.width()+20,img.height()+36);
}

void DetailLabel::set_Title(const QString &title)
{
    lblTitle_->setText(title);
}

void DetailLabel::set_Url(const QString &url)
{
    url_ = url;
}
