/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/22
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
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
    NotifyNotification *notify_p = nullptr;

    if (title == NULL) {
        return false;
    }

    if (notify_is_initted() == FALSE || notify_init("kuplayer") == FALSE) {
        return false;
    }

    notify_p = notify_notification_new(
                   title.toStdString().c_str(),
                   body.toStdString().c_str(),
                   icon.toStdString().c_str());
    notify_notification_set_timeout(notify_p, 8000);
    notify_notification_set_urgency(notify_p, NOTIFY_URGENCY_NORMAL);

    if (notify_notification_show(notify_p, &error) == FALSE) {
        return false;
    }

    notify_uninit();
    return true;
}
#endif //Q_OS_LINUX
#endif // SYSTEM_NOTIFY_H
