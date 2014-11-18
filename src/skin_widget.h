/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/21
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
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
    void skin_change_clicked(QString);
public:
    explicit SkinWidget(QWidget *parent = 0);
public slots:
    void on_this_show();
    void on_url_triggered(QString,QString);

private:
    void init_skin(QString name);
    void find_file(QString path);
QList<DetailLabel*> label_store;
        QStringList pic_list;
            QWidget *viewWidgetContents;
        QScrollArea *view;
        QGridLayout *scroll_layout;
         PushButton *btn_close;
      const QString PIC_PATH =\
            qApp->applicationDirPath()+"/sources/img/skin/";
};

#endif // SKIN_WIDGET_H
