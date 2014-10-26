/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/
#ifndef KUPLAYER_H
#define KUPLAYER_H
#include "common.h"
#include "shadow_widget.h"
#include "play_list_widget.h"
#include "skin_widget.h"
#include "main_menu.h"

#include <QBitArray>
#include <array>
#include <QSystemTrayIcon>
#include <QSettings>

class TitleWidget;
class QStackedWidget;
class MPlayerWidget;
class MPlayer;
class PyScript;

class kuplayer final
#ifdef CAN_RESIZE
 : public MainWidget
#else
 : public ShadowWidget
#endif
{
    Q_OBJECT
public:
    kuplayer(PyScript *pyinit,QWidget *parent = 0);
    virtual ~kuplayer();

public slots:
    void change_url(CLASS, int, QString);
    void load_next_page(CLASS);
    void loadImageFinished(CLASS,QPixmap,QString,QString);
    void start_loadImage(int, QStringList);
    void url_triggered(QString, QString);
    void url_ji_triggered(QString, QString url);
    void play_finished();
    void on_Fullscreen_changed();
    void change_skin(QString);
    void show_minimized();
private slots:
    void trayIcon_clicked(QSystemTrayIcon::ActivationReason);
public:
    PyScript *pyinit;
    const QStringList name{"tv","movice","zy","music","comic"};
private:
         TitleWidget  *title_widget;
      QStackedWidget  *stacked_widget;
       MPlayerWidget  *player_widget;
             MPlayer  *player;
      PlayListWidget   xuan_ji_widget;
          SkinWidget   skin_widget;
          MenuWidget   main_menu;
         QStringList   type_stores;
     QSystemTrayIcon   *trayicon;
           QBitArray   can_update{5,true};
             QString   format{"normal"};
    std::array<int,5>  pages{ {2,2,2,2,2} };
    QSettings   setting{qApp->applicationDirPath()+"/kuplayer.ini",QSettings::IniFormat};
    std::array<std::tuple<QString,QString,QString>,5> locate_class_time;
};

#endif // KUPLAYER_H
