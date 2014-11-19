/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "common.h"
#include "kuplayer.h"
#include "title_widget.h"
#include "list_widget.h"
#include "mplayer_widget.h"
#include "loadimage.h"
#include "control_widget.h"
#include "mthread.h"
#include "detail_label.h"
#include "skin_widget.h"
#include "pyscript.h"

#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QTranslator>
#include <QSettings>

#define WINDOW_WIDTH  1002
#define WINDOW_HEIGHT 657

#define SHOW_WINDOW_NORMAL \
this->showNormal();\
stacked_widget->setContentsMargins(3,0,3,3);\
this->layout()->setContentsMargins(5,5,5,5);
#define SHOW_WINDOW_MAXSIZE \
stacked_widget->setContentsMargins(0,0,0,0);\
this->layout()->setContentsMargins(0,0,0,0);

#if defined(Q_OS_LINUX)
    #ifdef signals     //主要是以下包括的头文件中有signals的变量名不这
        #undef signals //样的话不能通过编译因为在QT中signals中关键字。
        #include "system_notify.h"
    #endif
    #define signals public //恢复关键字定义,信好是个宏定义
    #define SHOW_MSG(name)  notification_show(name,tr("kuplayer")\
    ,qApp->applicationDirPath()+"/sources/img/kuplayer.ico");
#elif defined(Q_OS_WIN32)
    #define SHOW_MSG(name)  trayicon->showMessage(tr("kuplayer")\
    ,QString(name),QSystemTrayIcon::Information);
#endif // define Q_OS_LINUX

#define Control_Widget (*player_widget)
#define SHOW_PAGE "http://www.youku.com/show_page/"

kuplayer::kuplayer(PyScript *pyinit,QWidget *parent)
#ifdef CAN_RESIZE
    : MainWidget(parent)
#else
    : ShadowWidget(parent)
#endif
    , pyinit(pyinit)
    , main_menu(new MenuWidget)
    , trayicon(new QSystemTrayIcon)
    , setting(new conf_info)
{    
    setAttribute(Qt::WA_QuitOnClose,true);

    title_widget = new TitleWidget;
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addWidget(title_widget);
    setMinimumSize(WINDOW_WIDTH,WINDOW_HEIGHT);

    player_widget = new MPlayerWidget;
    player = new MPlayer;
    player->setRenderer(player_widget->renderer);

    stacked_widget = new QStackedWidget;
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::Window,QColor(240,240,240));
    stacked_widget->setPalette(text_palette);
    for(CLASS i=0;i<5;++i){
        ListWidget *l = new ListWidget(i);
        connect(l,SIGNAL(clicked(CLASS,int,QString)),this,SLOT(change_url(CLASS,int,QString)));
        connect(l,SIGNAL(load_next_page(CLASS)),this,SLOT(on_nextPage_loaded(CLASS)));
        stacked_widget->addWidget(l);
    }
    stacked_widget->addWidget(player_widget);
    main_layout->addWidget(stacked_widget);
    main_layout->setSpacing(0);

    connect(&xuan_ji_widget,SIGNAL(click(QString,QString)),this,SLOT(url_ji_triggered(QString,QString)));
    connect(&skin_widget,SIGNAL(skin_change_clicked(QString)),this,SLOT(on_skin_changed(QString)));
    connect(player,SIGNAL(mFinished(bool)),this,SLOT(on_play_finished(bool)));
    connect(player,SIGNAL(positionChanged(qint64)),player_widget->control_widget,SLOT(on_time_changed(qint64)));
    connect(player,SIGNAL(mSetDuration(qint64)),player_widget->control_widget,SLOT(on_douration_changed(qint64)));
    connect(player_widget->control_widget,SIGNAL(play_pause_clicked(bool)),player,SLOT(pause(bool)));
    connect(player_widget->control_widget,SIGNAL(stop_clicked()),player,SLOT(mStop()));
    connect(player_widget->control_widget,SIGNAL(backward_clicked()),player,SLOT(mSeekBack()));
    connect(player_widget->control_widget,SIGNAL(foreward_clicked()),player,SLOT(mSeekFore()));
    connect(player_widget->control_widget,SIGNAL(xuan_ji_clcked(QString,QString)),
            &xuan_ji_widget,SLOT(on_xuan_ji_show(QString,QString)));
    connect(player_widget->control_widget,SIGNAL(vol_up_clicked()),player,SLOT(vol_up()));
    connect(player_widget->control_widget,SIGNAL(vol_down_clicked()),player,SLOT(vol_down()));
    connect(player_widget,SIGNAL(escape_clicked()),this,SLOT(on_Fullscreen_changed()));
    connect(player_widget->renderer,SIGNAL(double_clicked()),this,SLOT(on_Fullscreen_changed()));
    connect(title_widget,SIGNAL(skin_clicked()),&skin_widget,SLOT(on_this_show()));
    connect(title_widget,SIGNAL(close_clicked()),this,SLOT(show_normal_or_close()));
    connect(title_widget,SIGNAL(min_clicked()),this,SLOT(on_showMin_clicked()));
    connect(title_widget,SIGNAL(ture_page(int)),stacked_widget,SLOT(setCurrentIndex(int)));
    connect(title_widget,SIGNAL(menu_clicked()),main_menu,SLOT(on_this_show()));

    if(pyinit->show_list.size()){
        for(int i=0;i<5;++i){
            auto *tmp = new mThread(i,
                 bind(&PyScript::connect_img_url,pyinit,pyinit->show_list[i], name[i]));
            connect(tmp,SIGNAL(mfinished(int,const QStringList&)),
                    this,SLOT(on_loadImage_started(int,const QStringList&)));
            tmp->start();
        }
    }
    addActions(Control_Widget->init_action());
    addActions(xuan_ji_widget.init_action());
    SHOW_MSG(tr("kuplayer: free for youku!"));
    SHOW_WINDOW_NORMAL
    title_widget->turepage(PLAYER);
}

kuplayer::~kuplayer()
{
    to_inifile();
    delete setting;
    delete trayicon;
    delete player;
    delete title_widget;
    delete player_widget;
    while(stacked_widget->count()){
        delete stacked_widget->widget(0);
    }
    delete stacked_widget;
}

void kuplayer::setIniFile(QSettings *iniFile)
{
    this->iniFile = iniFile;
    init_setting();
    init_trayicon();
}

void kuplayer::trayIcon_clicked(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::DoubleClick:
        if(isHidden() || isMinimized()){
            setWindowState(Qt::WindowActive);
            showNormal();
            if(player->isPaused()){
                player->pause(false);
            }
        }
        break;
    default:
        break;
    }
}
#include <QMessageBox>
void kuplayer::show_normal_or_close()
{
    if(setting->min_or_close){
        trayicon->showMessage(tr("kuplayer"),
                              tr("Showing by double-click !"),
                              QSystemTrayIcon::Information);
        hide();
        if(player->isPlaying()){
            Control_Widget->on_play_pause_triggered(true);
            player->pause(true);
        }
    }else if( QMessageBox::question(this,tr("Exit"),
                                    msg_font_style(tr("Really want to quit?")))
              == QMessageBox::StandardButton::Yes)
        close();
}

void kuplayer::on_showMin_clicked()
{
    showMinimized();
    if(player->isPlaying()){
        Control_Widget->on_play_pause_triggered(true);
        player->pause(true);
    }
}

void kuplayer::on_skin_changed(QString index)
{
    change_skin(index);
    xuan_ji_widget.change_skin(index);
    main_menu->change_skin(index);
}

void kuplayer::on_Fullscreen_changed()
{
    if(is_full_screen){
        title_widget->show();
        Control_Widget->show();
        SHOW_WINDOW_NORMAL
    }else{
        title_widget->hide();
        Control_Widget->hide();
        SHOW_WINDOW_MAXSIZE
    }
    is_full_screen = !is_full_screen;
}

void kuplayer::on_play_finished(bool real)//false 播放正常结束 true 中断播放
{
    if(real){
        Control_Widget->isRuning = false;
    }else if(player_widget->control_widget->isRuning
             && setting->auto_play_next ){
        xuan_ji_widget.on_playNext_clicked();
    }
    if(!player->isPlaying() && is_full_screen){
        on_Fullscreen_changed();
    }
    return;
}

void kuplayer::on_loadImage_started(int page, const QStringList& list)
{
    LoadImage *download = new LoadImage(page);
    download->setFileName(list);
    connect(download,SIGNAL(loadImageFinished(CLASS,QPixmap,QString,QString)),
            this,SLOT(loadImageFinished(CLASS,QPixmap,QString,QString)));
    download->start();
}


void kuplayer::on_nextPage_loaded(CLASS type)
{
    if(can_update[type]){
        QString url = pyinit->gotoNextPage(name[type],pages[type]++)[0];
        auto *tmp = new mThread(type,bind(&PyScript::connect_img_url,
                                          pyinit,url,name[type]));
        connect(tmp,SIGNAL(mfinished(int,const QStringList&)),
                this,SLOT(on_loadImage_started(int,const QStringList&)));
        tmp->start();
        can_update[type] = false;
    }
}

void kuplayer::loadImageFinished(CLASS index,QPixmap pix, QString name, QString url)
{
    if(name.isEmpty() && url.isEmpty()){
        can_update[index] = true;
        return;
    }
    if(index == MUSIC && pix.width() < 170){
        pix = pix.scaled(200,112);
    }
    DetailLabel *l = new DetailLabel(pix,name,url);
    connect(l,SIGNAL(url_triggered(QString,QString)),
            this,SLOT(on_url_triggered(QString,QString)));
    qobject_cast<ListWidget*>(stacked_widget->widget(index))->addDetailLabel(l);
}

void kuplayer::on_url_triggered(QString name,QString url)
{
    if(url.startsWith(SHOW_PAGE)){
        url = pyinit->getplayUrl(url)[0];
    }
    if(!url.isEmpty() && pyinit->GetVideoUrls(url,setting->default_video_format)){
        auto *tmp = new mThread(::NONE
                                ,bind(&PyScript::getAll,pyinit
                                          ,stacked_widget->currentIndex(),url));
        connect(tmp,SIGNAL(mfinished(int,const QStringList&)),
                &xuan_ji_widget,SLOT(on_list_changed(int,const QStringList&)));
        title_widget->turepage(PLAYER);
        tmp->start();
        player->mPlay();
        Control_Widget->isRuning = true;
        SHOW_MSG(tr("Currently playing:")+name);
        title_widget->set_text(name);
    }
}
void kuplayer::url_ji_triggered(QString name,QString url)
{
    if(pyinit->GetVideoUrls(url,setting->default_video_format)){
        stacked_widget->setCurrentIndex(5);
        Control_Widget->isRuning = true;
        player->mPlay();
        QString msg(tr("Currently playing:"));
        msg.insert(msg.size(),title_widget->get_text());
        msg.insert(msg.size(),"  ");
        msg.insert(msg.size(),name);
        SHOW_MSG(msg);
    }
}

void kuplayer::change_url(CLASS classes,int type,QString name)
{
    if(name == tr("All")){
        name = "";
    }
    switch (type) {
    case 0: //locate
        std::get<0>(locate_class_time[classes]) = name;
        break;
    case 1:
        std::get<1>(locate_class_time[classes]) = name;
        break;
    case 2:
        std::get<2>(locate_class_time[classes]) = name;
    default:
        break;
    }
    auto url = pyinit->getUrlByName(classes
                   ,std::get<0>(locate_class_time[classes])
                   ,std::get<1>(locate_class_time[classes])
                   ,std::get<2>(locate_class_time[classes]))[0];
    auto *tmp = new mThread(classes,
                            bind(&PyScript::connect_img_url,
                                              pyinit, url, this->name[classes]));
    connect(tmp,SIGNAL(mfinished(int,const QStringList&)),
            this,SLOT(on_loadImage_started(int,const QStringList&)));
    tmp->start();
    qobject_cast<ListWidget*>(stacked_widget->widget(classes))->reset();
}

#include <QMenu>
void kuplayer::init_trayicon()
{
    trayicon->setIcon(QIcon(":logo/logo_icon"));
    trayicon->show();
    trayicon->setToolTip("kuplayer");
    connect(trayicon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayIcon_clicked(QSystemTrayIcon::ActivationReason)));
    QMenu *tray_menu = new QMenu;
    trayicon->setContextMenu(tray_menu);
    auto*   set_action = tray_menu->addAction(tr("Settings"));
    auto* about_action = tray_menu->addAction(tr("About"));
    auto*  exit_action = tray_menu->addAction(tr("Exit"));
    connect(set_action,SIGNAL(triggered()),main_menu,SLOT(show()));
    connect(about_action,SIGNAL(triggered()),main_menu,SLOT(show_about()));
    connect(exit_action,SIGNAL(triggered()),this,SLOT(close()));
}

void kuplayer::init_setting()
{
    setting->default_video_format = iniFile->value("setting/format","normal").toString();
    setting->close_all = iniFile->value("setting/close_all",true).toBool();
    setting->auto_play_next = iniFile->value("setting/auto_play_next",false).toBool();
    setting->min_or_close = iniFile->value("setting/min_or_close",false).toBool();
    setting->start_when_pc_on = iniFile->value("setting/start_when_pc_on",false).toBool();
    setting->language = iniFile->value("setting/language",true).toBool();
    skin_widget.on_url_triggered("",iniFile->value("setting/skin",":/kin/0").toString());
    
    main_menu->init_setting(setting);
}

void kuplayer::to_inifile()
{
    iniFile->setValue("setting/skin",get_skin());
    iniFile->setValue("setting/format",setting->default_video_format);
    iniFile->setValue("setting/close_all",setting->close_all);
    iniFile->setValue("setting/auto_play_next",setting->auto_play_next);
    iniFile->setValue("setting/min_or_close", setting->min_or_close );
    iniFile->setValue("setting/start_when_pc_on",setting->start_when_pc_on);
    iniFile->setValue("setting/language",setting->language);
}

