/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/
#ifndef LOADIMAGE_H
#define LOADIMAGE_H
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>
#include <QPixmap>
#include <QWaitCondition>
#include <QMutex>
#include <QDebug>
typedef unsigned int CLASS;

class LoadImage final : public QObject
{
    Q_OBJECT

signals:
    void loadImageFinished(CLASS/*视频种类*/,QPixmap/*图片*/,QString/*图片名称*/,QString/*所指url*/);

public:
    LoadImage(CLASS index,QObject *parent = 0)
        : index(index)
        , currentIndex(0)
        , list_(new QList<QStringList>)
        , manager(new QNetworkAccessManager)
    {
        Q_UNUSED(parent);
        connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    }
    virtual ~LoadImage()
    {
        delete manager;
        delete list_;
    }
    void setFileName(QStringList listname,QString separator="$$")
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
            return;
        }
        manager->get(QNetworkRequest(list_->at(currentIndex)[1]));
    }

private slots:
    void replyFinished(QNetworkReply *reply)
    {
        QPixmap pix;
        pix.loadFromData(reply->readAll());
        emit loadImageFinished(index,pix,list_->at(currentIndex)[0],list_->at(currentIndex)[2]);
        ++currentIndex;
        start();
    }

private:
    int index;
    int currentIndex;
    QList<QStringList>    *list_;
    QNetworkAccessManager *manager;
};

#endif // LOADIMAGE_H
