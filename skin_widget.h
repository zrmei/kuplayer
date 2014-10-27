/**************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/21
**************************************/
#ifndef SKIN_WIDGET_H
#define SKIN_WIDGET_H

#include "shadow_widget.h"
class QScrollArea;
class QGridLayout;
class DetailLabel;
class PushButton;

class SkinWidget final : public ShadowWidget
{
    Q_OBJECT
signals:
    void change_skin(QString);
public:
    explicit SkinWidget(QWidget *parent = 0);
public slots:
    void this_show();
    void url_triggered(QString,QString);

private:
    void init_skin(QString name);
    void find_file(QString path);
QList<DetailLabel*> label_store;
        QStringList pic_list;
            QWidget *viewWidgetContents;
        QScrollArea *view;
        QGridLayout *scroll_layout;
         PushButton *btn_close;
const QString PIC_PATH = qApp->applicationDirPath()+"/sources/img/skin/";
};

#endif // SKIN_WIDGET_H
