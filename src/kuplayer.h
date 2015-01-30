/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef KUPLAYER_H
#define KUPLAYER_H
#include "common.h"
#include "shadow_widget.h"

#include <QSystemTrayIcon>
class QSettings;

KUPLAYER_NAMESPACE_BEGIN //namespace begin
class PyScript;
struct MainWidget_Impl;

class MainWidget        
#ifdef CAN_RESIZE
 : public ResizedWidget
#else
 : public ShadowWidget
#endif
{
    Q_OBJECT
signals:
    void load_finished(int);
public:
    MainWidget(PyScript *pyinit, const QString&, QWidget *parent = 0);
    virtual ~MainWidget();
    void setIniFile(QSettings *);
public slots:
    void on_showMin_clicked();
    void on_skin_changed(QString);
    void on_Fullscreen_changed();
    void on_play_finished(bool);
    void on_loadImage_started(int, QStringList);
    void on_nextPage_loaded(CLASS);
    void on_loadImage_finished(CLASS, QPixmap, QString, QString);
    void on_url_triggered(QString, QString);
    void on_url_ji_triggered(QString, QString);
    void on_url_changed(CLASS, int, QString);
private slots:
    void on_trayIcon_clicked(QSystemTrayIcon::ActivationReason);
    void on_title_widget_closed();
private:
    void init_setting();
    void to_inifile();
    void init_trayicon();
    
private:
    std::shared_ptr<MainWidget_Impl> pImpl;
};

KUPLAYER_NAMESPACE_END //namespace end
#endif // KUPLAYER_H
