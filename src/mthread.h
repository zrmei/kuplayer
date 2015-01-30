/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/26
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef MTHREAD_H
#define MTHREAD_H

#include "common.h"

#include <QThread>
#include <functional>
#include <python2.7/Python.h>
#include <QStringList>
#include <QDebug>

KUPLAYER_NAMESPACE_BEGIN //namespace begin

class mThread : public QThread
{
    Q_OBJECT
    typedef unsigned int CLASS;
    typedef std::function<QStringList()> F;
signals:
    void load_finished(int,QStringList);
public:
    mThread(const int& page,const F& func,QObject *parent=0)
        : QThread(parent)
        , page_(page)
        , func_(func)
    {}
protected:
    virtual void run()
    {
        emit load_finished(page_,func_());
        deleteLater();
    }
 
private:
    ~mThread()
    {
        qDebug() << "thread over" << page_;
    }
    
    CLASS page_;
    F func_;

};

KUPLAYER_NAMESPACE_END // namespace end
#endif // MTHREAD_H
