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
public:
    MainWidget(PyScript *pyinit, const QString&, QWidget *parent = 0);
    virtual ~MainWidget();
    void setIniFile(QSettings *);
public slots:
    void on_showMin_clicked();
    void on_skin_changed(QString);
    void on_Fullscreen_changed();
    void on_play_finished(bool);
    void on_loadImage_started(int,const QStringList&);
    void on_nextPage_loaded(CLASS);
    void loadImageFinished(CLASS, QPixmap, QString, QString);
    void on_url_triggered(QString, QString);
    void url_ji_triggered(QString, QString);
    void change_url(CLASS, int, QString);
private slots:
    void trayIcon_clicked(QSystemTrayIcon::ActivationReason);
    void show_normal_or_close();
private:
    void init_setting();
    void to_inifile();
    void init_trayicon();
    
private:
    std::shared_ptr<MainWidget_Impl> pImpl;
};

KUPLAYER_NAMESPACE_END //namespace end
#endif // KUPLAYER_H
