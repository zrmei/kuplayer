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

#ifndef SYSTEM_NOTIFY_H
#define SYSTEM_NOTIFY_H

#include <QApplication>

#if defined(Q_OS_LINUX) && defined(USE_NOTIFY)
#ifdef signals //主要是以下包括的头文件中有signals的变量名不这
#undef signals //样的话不能通过编译因为在QT中signals中关键字。
#include <libnotify/notify.h>
#endif
#define signals public //恢复关键字定义,幸好是个宏定义


bool notification_show(const QString &title, const QString &body, const QString &icon)
{
    GError *error = nullptr;
    static NotifyNotification *notify_p = nullptr;

    if (title == NULL) {
        qDebug() << __LINE__ << ": init notify fail ! title is null " ;
        return false;
    }

    if (notify_is_initted() == FALSE) {
        if (notify_init("kuplayer") == FALSE) {
            qDebug() << __LINE__ << ": kuplayer notify init fail ! " ;
            return false;
        }

        notify_p = notify_notification_new(
                       title.toStdString().c_str(),
                       body.toStdString().c_str(),
                       icon.toStdString().c_str());
        notify_notification_set_timeout(notify_p, 8000);
        notify_notification_set_urgency(notify_p, NOTIFY_URGENCY_NORMAL);
    } else {
        notify_notification_update(notify_p,
                                   title.toStdString().c_str(),
                                   body.toStdString().c_str(),
                                   icon.toStdString().c_str());
    }

    if (notify_notification_show(notify_p, &error) == FALSE) {
        qDebug() << "show notify fail !" ;
        return false;
    }

    return true;
}
#endif //Q_OS_LINUX
#endif // SYSTEM_NOTIFY_H
