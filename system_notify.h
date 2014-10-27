/**************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/22
**************************************/
#ifndef SYSTEM_NOTIFY_H
#define SYSTEM_NOTIFY_H

#include <QApplication>

#if defined(Q_OS_LINUX)
#include <libnotify/notify.h>

bool notification_show(QString title,QString body,QString icon)
{
    GError *error = NULL;
    NotifyNotification *notify_p;
    if (title == NULL)
        return false;
    if(notify_is_initted() == FALSE){
        if(notify_init("kuplayer") == FALSE){
            return false;
        }
    }
    notify_p = notify_notification_new(
                title.toStdString().c_str(),
                body.toStdString().c_str(),
                icon.toStdString().c_str());
    notify_notification_set_timeout(notify_p, 500);
    notify_notification_set_urgency(notify_p,NOTIFY_URGENCY_NORMAL);

    if (notify_notification_show(notify_p, &error) == FALSE){
        return false;
    }
    notify_uninit();
    return true;
}
#endif //Q_OS_LINUX
#endif // SYSTEM_NOTIFY_H
