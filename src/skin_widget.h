/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/21
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef SKIN_WIDGET_H
#define SKIN_WIDGET_H
#include "common.h"
#include "shadow_widget.h"

KUPLAYER_NAMESPACE_BEGIN //namespace begin

class SkinWidget final : public ShadowWidget
{
    Q_OBJECT
signals:
    void skin_change_clicked(QString);
public:
    explicit SkinWidget(QWidget *parent = 0);
    ~SkinWidget(){}

public slots:
    void on_showed();
    void on_url_triggered(QString,QString);

private:
    void init_skin(QString name);
    void find_file(QString path);
    
    QStringList pic_list;
    QString PIC_PATH{qApp->applicationDirPath()+"/../sources/img/skin/"};
    
    struct SkinWidget_Impl;
    std::shared_ptr<SkinWidget_Impl> pImpl;
};

KUPLAYER_NAMESPACE_END // namespace end
#endif // SKIN_WIDGET_H
