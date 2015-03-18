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
    void load_finished(int, QStringList);
public:
    mThread(const int &page, const F &func, QObject *parent = 0)
        : QThread(parent)
        , page_(page)
        , func_(func)
    {}
protected:
    virtual void run() {
        emit load_finished(page_, func_());
        deleteLater();
    }

private:
    ~mThread() {
        qDebug() << "thread over" << page_;
    }

    CLASS page_;
    F func_;

};

KUPLAYER_NAMESPACE_END // namespace end
#endif // MTHREAD_H
