/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/

#include "common.h"
#include "kuplayer.h"
#include "title_widget.h"
#include "list_widget.h"
#include "mplayer_widget.h"
#include "loadimage.h"
#include "control_widget.h"
#include "mthread.h"
#include "detaillabel.h"
#include "skin_widget.h"
#include "pyscript.h"

#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#define WINDOW_WIDTH  1002
#define WINDOW_HEIGHT 657

#define SHOW_WINDOW_NORMAL \
stacked_widget->setContentsMargins(3,0,3,3);\
this->layout()->setContentsMargins(5,5,5,5);
#define SHOW_WINDOW_MAXSIZE \
stacked_widget->setContentsMargins(0,0,0,0);\
this->layout()->setContentsMargins(0,0,0,0);

#if defined(Q_OS_LINUX)
    #ifdef signals
        #undef signals
        #include "system_notify.h"
    #endif // define signal
    #define signals public
    #define SHOW_MSG(name)  notification_show(name,"kuplayer",\
                                                    qApp->applicationDirPath()+"/sources/img/kuplayer.ico");
#elif defined(Q_OS_WIN32)
    #define SHOW_MSG(name)  trayicon->showMessage("kuplayer",QString(name),QSystemTrayIcon::Information);
#endif // define Q_OS_LINUX

#define SHOW_PAGE "http://www.youku.com/show_page/"

kuplayer::kuplayer(PyScript *pyinit,QWidget *parent)
#ifdef CAN_RESZE
    : MainWidget
#else
    : ShadowWidget
#endif
      (parent)
    , pyinit(pyinit)
    , trayicon(new QSystemTrayIcon)
{
    setAttribute(Qt::WA_QuitOnClose,true);

    trayicon->setIcon(QIcon(":logo/logo_icon"));
    trayicon->show();
    trayicon->setToolTip("kuplayer");
    connect(trayicon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayIcon_clicked(QSystemTrayIcon::ActivationReason)));

    title_widget = new TitleWidget;
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addWidget(title_widget);
    this->setMinimumSize(WINDOW_WIDTH,WINDOW_HEIGHT);

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
        connect(l,SIGNAL(load_next_page(CLASS)),this,SLOT(load_next_page(CLASS)));
        stacked_widget->addWidget(l);
    }
    stacked_widget->addWidget(player_widget);
    main_layout->addWidget(stacked_widget);
    main_layout->setSpacing(0);

    connect(&xuan_ji_widget,SIGNAL(click(QString,QString)),this,SLOT(url_ji_triggered(QString,QString)));
    connect(&skin_widget,SIGNAL(change_skin(QString)),this,SLOT(change_skin(QString)));
    connect(player,SIGNAL(mFinished()),this,SLOT(play_finished()));
    connect(player,SIGNAL(positionChanged(qint64)),player_widget->control_widget,SLOT(setTime(qint64)));
    connect(player_widget->control_widget,SIGNAL(play_pause_clicked(bool)),player,SLOT(pause(bool)));
    connect(player_widget->control_widget,SIGNAL(stop_clicked()),player,SLOT(mStop()));
    connect(player_widget->control_widget,SIGNAL(backward_clicked()),player,SLOT(mSeekBack()));
    connect(player_widget->control_widget,SIGNAL(foreward_clicked()),player,SLOT(mSeekFore()));
    connect(player_widget->control_widget,SIGNAL(xuan_ji_clcked(QString,QString)),&xuan_ji_widget,SLOT(show_xuan_ji(QString,QString)));
    connect(player_widget,SIGNAL(escape_clicked()),this,SLOT(play_finished()));
    connect(player_widget->renderer,SIGNAL(double_clicked()),this,SLOT(on_Fullscreen_changed()));
    connect(title_widget,SIGNAL(skin_clicked()),&skin_widget,SLOT(this_show()));
    connect(title_widget,SIGNAL(close_clicked()),this,SLOT(close()));
    connect(title_widget,SIGNAL(min_clicked()),this,SLOT(show_minimized()));
    connect(title_widget,SIGNAL(ture_page(int)),stacked_widget,SLOT(setCurrentIndex(int)));
    connect(title_widget,SIGNAL(menu_clicked()),&main_menu,SLOT(this_show()));
    title_widget->turepage("播放器");
    skin = setting.value("setting/skin",":/kin/0").toString();
    skin_widget.url_triggered("",skin);
    format = setting.value("setting/format","normal").toString();

    if(pyinit->show_list.size()){
        for(int i=0;i<5;++i){
            auto *tmp = new mThread(i,
                 bind(&PyScript::connect_img_url,pyinit,pyinit->show_list[i], name[i]));
            connect(tmp,SIGNAL(mfinished(int,QStringList)),
                                                this,SLOT(start_loadImage(int,QStringList)));
            tmp->start();
        }
    }
    this->addActions(player_widget->control_widget->init_action());
    SHOW_MSG("kuplayer: free for youku");
    SHOW_WINDOW_NORMAL
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
void kuplayer::show_minimized()
{
    this->showMinimized();
    if(player->isPlaying()){
        player_widget->control_widget->trigger_play_pause(true);
        player->pause(true);
    }
}

void kuplayer::change_skin(QString index)
{
    skin = index;
    xuan_ji_widget.change_skin(skin);
    update();
}

void kuplayer::on_Fullscreen_changed()
{
    if(is_full_screen){
        title_widget->show();
        player_widget->control_widget->show();
        SHOW_WINDOW_NORMAL
    }else{
        title_widget->hide();
        player_widget->control_widget->hide();
        SHOW_WINDOW_MAXSIZE
    }
    is_full_screen = !is_full_screen;
}

kuplayer::~kuplayer()
{
    setting.setValue("setting/skin",skin);
    setting.setValue("setting/format",format);
    delete trayicon;
    delete player;
    delete title_widget;
    delete player_widget;
    delete stacked_widget;
}

void kuplayer::start_loadImage(int page,QStringList list)
{
    LoadImage *download = new LoadImage(page);
    download->setFileName(list);
    connect(download,SIGNAL(loadImageFinished(CLASS,QPixmap,QString,QString)),
            this,SLOT(loadImageFinished(CLASS,QPixmap,QString,QString)));
    download->start();
}

void kuplayer::load_next_page(CLASS type)
{
    if(can_update[type]){
        QString url = pyinit->gotoNextPage(name[type],pages[type]++)[0];
        auto *tmp = new mThread(type,
                                bind(&PyScript::connect_img_url,pyinit,url,name[type]));
        connect(tmp,SIGNAL(mfinished(int,QStringList)),
                this,SLOT(start_loadImage(int,QStringList)));
        tmp->start();
        can_update[type] = false;
    }
}

void kuplayer::play_finished()
{
    if(is_full_screen){
        on_Fullscreen_changed();
        this->showNormal();
    }
    player_widget->control_widget->isRuning = false;
}

void kuplayer::loadImageFinished(CLASS index,QPixmap pix, QString name, QString url)
{
    if(name=="" && url == ""){
        can_update[index] = true;
        return;
    }
    DetailLabel *l = new DetailLabel(pix,name,url);
    connect(l,SIGNAL(url_triggered(QString,QString)),this,SLOT(url_triggered(QString,QString)));
    qobject_cast<ListWidget*>(stacked_widget->widget(index))->addDetailLabel(l);
}

void kuplayer::url_triggered(QString name,QString url)
{
    if(url.startsWith(SHOW_PAGE)){
        url = pyinit->getplayUrl(url)[0];
    }
    if(!url.isEmpty() && pyinit->GetVideoUrls(url,format)){
        auto *tmp = new mThread(::NONE
                                ,bind(&PyScript::getAll,pyinit
                                          ,stacked_widget->currentIndex(),url));
        connect(tmp,SIGNAL(mfinished(int,QStringList)),
                &xuan_ji_widget,SLOT(setList(int,QStringList)));
        tmp->start();
        title_widget->turepage("播放器");
        player->mPlay();
        player_widget->control_widget->isRuning = true;
        SHOW_MSG("正在播放："+name);
        title_widget->set_text(name);
    }
}
void kuplayer::url_ji_triggered(QString name,QString url)
{
    if(pyinit->GetVideoUrls(url,format)){
        stacked_widget->setCurrentIndex(5);
        player->mPlay();
        QString msg("正在播放：");
        msg.insert(msg.size(),title_widget->get_text());
        msg.insert(msg.size(),"  ");
        msg.insert(msg.size(),name);
        SHOW_MSG(msg);
    }
}

void kuplayer::change_url(CLASS classes,int type,QString name)
{
    if(name == "全部"){
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
    auto *tmp = new mThread(classes,bind(&PyScript::connect_img_url,
                                              pyinit, url, this->name[classes]));
    connect(tmp,SIGNAL(mfinished(int,QStringList)),
            this,SLOT(start_loadImage(int,QStringList)));
    tmp->start();
    qobject_cast<ListWidget*>(stacked_widget->widget(classes))->reset();
}
