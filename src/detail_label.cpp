/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/14
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "detail_label.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QApplication>
#include <QPixmap>
#include <QDebug>


struct NAMESPACE_KUPLAYER::DetailLabel::DetailLabel_Impl
{
    Label *lblImg_;
    Label *lblTitle_;
    QString  url_;
    QVBoxLayout *verticalLayout;
    
    DetailLabel_Impl()
        : lblImg_(new Label)
        , lblTitle_(new Label)
        , verticalLayout(new QVBoxLayout)

    {
        verticalLayout->setSpacing(0);
        lblImg_->setAlignment(Qt::AlignCenter);
        lblTitle_->setMaximumHeight(36);
        lblTitle_->setAlignment(Qt::AlignHCenter| Qt::AlignBottom);
        
        verticalLayout->addWidget(lblImg_);
        verticalLayout->addWidget(lblTitle_);
    }
    ~DetailLabel_Impl()
    {
        delete lblImg_;
        delete lblTitle_;
        delete verticalLayout;
    }
};

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
    , pImpl(new DetailLabel_Impl())
{
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->addLayout(pImpl->verticalLayout);

    connect(pImpl->lblImg_,SIGNAL(clicked()),this,SLOT(this_url_triggered()));
    connect(pImpl->lblTitle_,SIGNAL(clicked()),this,SLOT(this_url_triggered()));
}

DetailLabel::DetailLabel(QPixmap img, const QString &title, const QString &url, QWidget *parent)
    : DetailLabel(parent)
{
    set_Pixmap(std::move(img));
    set_Title(title);
    set_Url(url);
}

DetailLabel::~DetailLabel()
{
//    qDebug() <<"DetailLabel("<<lblTitle_->text()<<") deleted";
}
void DetailLabel::this_url_triggered()
{
    emit url_triggered(pImpl->lblTitle_->text(),pImpl->url_);
}

void DetailLabel::set_Pixmap(QPixmap &&img)
{
    pImpl->lblImg_->setPixmap(img);
    setFixedSize(img.width()+20,img.height()+36);
}

void DetailLabel::set_Title(const QString &title)
{
    pImpl->lblTitle_->setText(title);
    pImpl->lblTitle_->setToolTip(title);
    pImpl->lblImg_->setToolTip(title);
}

void DetailLabel::set_Url(const QString &url)
{
    pImpl->url_ = url;
}

QString DetailLabel::text() const
{
    return pImpl->lblTitle_->text();
}
