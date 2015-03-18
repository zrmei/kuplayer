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


#ifndef LOADIMAGE_H
#define LOADIMAGE_H

#include "common.h"

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>
#include <QPixmap>
#include <QWaitCondition>
#include <QMutex>
#include <QDebug>

KUPLAYER_NAMESPACE_BEGIN //namespace begin

#define KUPLAYER_RES_NAME   0
#define KUPLAYER_PIC_URL    1
#define KUPLAYER_ADDR_URL   2

class LoadImage final : public QObject
{
    Q_OBJECT

signals:
    void loadImageFinished(CLASS/*视频种类*/, QPixmap/*图片*/, QString/*图片名称*/, QString/*所指url*/);
public:
    LoadImage(CLASS index, QObject *parent = 0)
        : index(index)
        , currentIndex(0)
        , list_(new QList<QStringList>)
        , request(new QNetworkRequest)
        , manager(new QNetworkAccessManager)
    {
        Q_UNUSED(parent);
        connect(manager, SIGNAL(finished(QNetworkReply *)),this, SLOT(replyFinished(QNetworkReply *)));
    }

    void setFileName(const QStringList &listname, QString const& separator = "$$") 
    {
        for (const QStringList::value_type & item : listname) {
            list_->append(item.split(separator));
        }
    }
    void start() 
    {
        if (currentIndex == list_->length()) {
            emit loadImageFinished(index, QPixmap(), "", "");
            currentIndex = 0;
            deleteLater();
            return;
        } else {
            request->setUrl(QUrl(list_->at(currentIndex)[KUPLAYER_PIC_URL]));
            manager->get(*request);
        }
    }

private slots:
    void replyFinished(QNetworkReply *reply) 
    {
        QPixmap pix;
        pix.loadFromData(reply->readAll());
        emit loadImageFinished(
            index,
            std::move(pix)
            , list_->at(currentIndex)[KUPLAYER_RES_NAME]
            , list_->at(currentIndex)[KUPLAYER_ADDR_URL]
        );
        ++currentIndex;
        start();
    }

private:
    virtual ~LoadImage() 
    {
        qDebug() << "delete loadimage:" << index;
        delete request;
        delete manager;
        delete list_;
    }

    int index;
    int currentIndex;
    QList<QStringList>    *list_;
    QNetworkRequest       *request;
    QNetworkAccessManager *manager;
};

#undef KUPLAYER_RES_NAME
#undef KUPLAYER_RES_PIC_URL
#undef KUPLAYER_RES_ADDR_URL

KUPLAYER_NAMESPACE_END //namespace end
#endif // LOADIMAGE_H
