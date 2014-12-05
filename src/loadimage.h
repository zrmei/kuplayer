/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/14
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
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

typedef unsigned int CLASS;

class LoadImage  : public QObject
{
    Q_OBJECT

signals:
    void loadImageFinished(CLASS/*视频种类*/,QPixmap/*图片*/,
                           QString/*图片名称*/,QString/*所指url*/);
public:
    LoadImage(CLASS index,QObject *parent = 0)
        : index(index)
        , currentIndex(0)
        , list_(new QList<QStringList>)
        , request(new QNetworkRequest)
        , manager(new QNetworkAccessManager)
    {
        Q_UNUSED(parent);
        connect(manager,SIGNAL(finished(QNetworkReply*)),
                this,SLOT(replyFinished(QNetworkReply*)));
    }

    void setFileName(const QStringList& listname,QString separator="$$")
    {
        for(int i=0; i<listname.size(); ++i){
            list_->append(listname[i].split(separator));
        }
    }
    inline void start()
    {
        if(currentIndex == list_->length()){
            emit loadImageFinished(index,QPixmap(),"","");
            currentIndex = 0;
            deleteLater();
            return;
        }else{
            request->setUrl(QUrl(list_->at(currentIndex)[1]));
            manager->get(*request);
        }
    }

private slots:
    void replyFinished(QNetworkReply *reply)
    {
        QPixmap pix;
        pix.loadFromData(reply->readAll());
        emit loadImageFinished(index,std::move(pix),
                               list_->at(currentIndex)[0],list_->at(currentIndex)[2]);
        ++currentIndex;
        start();
    }
    
private:
    virtual ~LoadImage()
    {
        qDebug() << "delete loadimage:"<<index;
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

KUPLAYER_NAMESPACE_END //namespace end
#endif // LOADIMAGE_H
