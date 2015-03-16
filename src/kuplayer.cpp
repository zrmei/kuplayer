/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "common.h"
#include "gloal_func.h"
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
#include "play_list_widget.h"
#include "main_menu.h"
#include "system_notify.h"

USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <QStackedWidget>
#include <QBitArray>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QTranslator>
#include <QSettings>
#include <QMenu>
#include <QDesktopWidget>

#define SHOW_WINDOW_NORMAL \
    this->showNormal();\
    pImpl->stacked_widget->setContentsMargins(3,0,3,3);\
    this->layout()->setContentsMargins(5,5,5,5);
#define SHOW_WINDOW_MAXSIZE \
    setWindowState( Qt::WindowFullScreen );\
    pImpl->stacked_widget->setContentsMargins(0,0,0,0);\
    this->layout()->setContentsMargins(0,0,0,0);

#if defined(Q_OS_LINUX) && defined(USE_NOTIFY)
#define SHOW_MSG(name)  notification_show(name,tr("kuplayer"),pImpl->ico_path);
#elif defined(Q_OS_WIN32)
#define SHOW_MSG(name)  trayicon->showMessage(tr("kuplayer"),QString(name),QSystemTrayIcon::Information);
#endif // define Q_OS_LINUX

#define Control_Widget (*(pImpl->player_widget))

struct NAMESPACE_KUPLAYER::MainWidget::MainWidget_Impl {
    Q_DISABLE_COPY(MainWidget_Impl)

    PyScript        *pyinit;
    PlayListWidget  *xuan_ji_widget;
    SkinWidget      *skin_widget;
    QSettings       *iniFile;
    TitleWidget     *title_widget;
    QStackedWidget  *stacked_widget;
    MPlayerWidget   *player_widget;
    MPlayer         *player;
    MenuWidget      *main_menu;
    QSystemTrayIcon *trayicon;
    conf_info       *setting;
    ControlWidget   *control_widget;

    const QStringList   name;
    const    QString   &ico_path;
    QBitArray   can_update {5, true};
    std::array<int, 5>   pages { {2, 2, 2, 2, 2} };
    std::array<std::tuple<QString, QString, QString>, 5> locate_class_time;

    MainWidget_Impl(PyScript *pyinit, const QString &ico_path)
        : pyinit(pyinit)
        , xuan_ji_widget(new PlayListWidget)
        , skin_widget(new SkinWidget)
        , title_widget(new TitleWidget)
        , stacked_widget(new QStackedWidget)
        , player_widget(new MPlayerWidget)
        , player(new MPlayer)
        , main_menu(new MenuWidget)
        , trayicon(new QSystemTrayIcon)
        , setting(new conf_info)
        , name({"tv", "movice", "zy", "music", "comic"})
    , ico_path(ico_path) {
        control_widget = player_widget->control_widget;
        player->setRenderer(player_widget->renderer);
    }
    ~MainWidget_Impl() {
        delete setting;
        delete trayicon;
        delete player;
        delete title_widget;
        delete player_widget;

        while (stacked_widget->count()) {
            delete stacked_widget->widget(0);
        }

        delete stacked_widget;
    }
};

MainWidget::MainWidget(PyScript *pyinit, const QString &ico_path, QWidget *parent)
#ifdef CAN_RESIZE
    : ResizedWidget(parent)
#else
    : ShadowWidget(parent)
#endif
    , pImpl(new MainWidget_Impl(pyinit, ico_path))
{
    setAttribute(Qt::WA_QuitOnClose, true);
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addWidget(pImpl->title_widget);
    setMinimumSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::Window, QColor(240, 240, 240));
    text_palette.setColor(QPalette::Background, QColor(255, 255, 255, 100));
    pImpl->stacked_widget->setPalette(text_palette);

    for (auto && class_index : {TV, MOVIE, ZONGYI, MUSIC, COMIC}) {
        ListWidget *l = new ListWidget(class_index);
        connect(l, SIGNAL(clicked(CLASS, int, QString)), this, SLOT(on_url_changed(CLASS, int, QString)));
        connect(l, SIGNAL(emit_next_page(CLASS)), this, SLOT(on_nextPage_loaded(CLASS)));
        pImpl->stacked_widget->addWidget(l);
    }

    pImpl->stacked_widget->addWidget(pImpl->player_widget);
    main_layout->addWidget(pImpl->stacked_widget);
    main_layout->setSpacing(0);
    connect(pImpl->xuan_ji_widget, SIGNAL(click(QString, QString)), this, SLOT(on_url_ji_triggered(QString, QString)));
    connect(pImpl->skin_widget, SIGNAL(skin_change_clicked(QString)), this, SLOT(on_skin_changed(QString)));
    connect(pImpl->player, SIGNAL(mFinished()), this, SLOT(on_play_finished()));
    connect(pImpl->player, SIGNAL(positionChanged(qint64)), pImpl->control_widget, SLOT(on_time_changed(qint64)));
    connect(pImpl->player, SIGNAL(mSetDuration(qint64)), pImpl->control_widget, SLOT(on_douration_changed(qint64)));
    connect(pImpl->control_widget, SIGNAL(play_pause_clicked(bool)), pImpl->player, SLOT(play_pause(bool)));
    connect(pImpl->control_widget, SIGNAL(stop_clicked()), this, SLOT(on_stop_clicked()));
    connect(pImpl->control_widget, SIGNAL(backward_clicked()), pImpl->player, SLOT(mSeekBack()));
    connect(pImpl->control_widget, SIGNAL(foreward_clicked()), pImpl->player, SLOT(mSeekFore()));
    connect(pImpl->control_widget, SIGNAL(xuan_ji_clcked(QString, QString)), pImpl->xuan_ji_widget, SLOT(on_xuan_ji_show(QString, QString)));
    connect(pImpl->control_widget, SIGNAL(vol_up_clicked()), pImpl->player, SLOT(vol_up()));
    connect(pImpl->control_widget, SIGNAL(vol_down_clicked()), pImpl->player, SLOT(vol_down()));
    connect(pImpl->player_widget, SIGNAL(escape_clicked()), this, SLOT(on_Fullscreen_changed()));
    connect(pImpl->player_widget->renderer, SIGNAL(double_clicked()), this, SLOT(on_Fullscreen_changed()));
    connect(pImpl->title_widget, SIGNAL(skin_clicked()), pImpl->skin_widget, SLOT(on_showed()));
    connect(pImpl->title_widget, SIGNAL(close_clicked()), this, SLOT(on_title_widget_closed()));
    connect(pImpl->title_widget, SIGNAL(min_clicked()), this, SLOT(on_showMin_clicked()));
    connect(pImpl->title_widget, SIGNAL(ture_page(int)), pImpl->stacked_widget, SLOT(setCurrentIndex(int)));
    connect(pImpl->title_widget, SIGNAL(menu_clicked()), pImpl->main_menu, SLOT(on_showed()));
#ifndef NO_WIFI_TEST
    init_module();
#endif
    addActions(Control_Widget->reg_actions());
    addActions(pImpl->xuan_ji_widget->init_action());
    SHOW_MSG(tr("kuplayer: free for youku!"));
    SHOW_WINDOW_NORMAL
    pImpl->title_widget->on_turepage_triggered(PLAYER);
}

void MainWidget::init_module()
{
    if (pImpl->pyinit->show_list.size()) {
        for (auto && class_index : {TV, MOVIE, ZONGYI, MUSIC, COMIC}) {
            auto *tmp = new mThread(class_index,
                                    bind(&PyScript::connect_img_url,
                                         pImpl->pyinit,
                                         pImpl->pyinit->show_list[class_index],
                                         pImpl->name[class_index]));
            connect(tmp, SIGNAL(load_finished(int, QStringList)),
                    this, SLOT(on_loadImage_started(int, QStringList)));
            tmp->start();
        }
    }
}

MainWidget::~MainWidget()
{
    to_inifile();
}

void MainWidget::setIniFile(QSettings *iniFile)
{
    pImpl->iniFile = iniFile;
    init_setting();
    init_trayicon();
}

void MainWidget::on_trayIcon_clicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::DoubleClick:
            if (isHidden() || isMinimized()) {
                setWindowState(Qt::WindowActive);
                showNormal();

                if (pImpl->player->isPaused()) {
                    pImpl->player->pause(false);
                }
            }

            break;

        default:
            break;
    }
}
#include <QMessageBox>
void MainWidget::on_title_widget_closed()
{
    if (pImpl->setting->min_or_close) {
        on_showMin_clicked();
        return;
    }

    auto button = QMessageBox::question(
                      this,
                      tr("Exit"),
                      msg_font_style(tr("Really want to quit?")));

    if (button == QMessageBox::StandardButton::Yes) {
        close();
    }
}

void MainWidget::on_showMin_clicked()
{
    qDebug() << "on_showMin_clicked triggered";
    hide();

    if (pImpl->player->isPlaying()) {
        pImpl->player->pause(true);
    }

    pImpl->trayicon->showMessage(
        tr("kuplayer"),
        tr("Showing by double-click !"),
        QSystemTrayIcon::Information);
}

void MainWidget::on_skin_changed(QString index)
{
    change_skin(index);
    pImpl->xuan_ji_widget->change_skin(index);
    pImpl->main_menu->change_skin(index);
}

void MainWidget::on_Fullscreen_changed()
{
    if (is_full_screen) {
        pImpl->title_widget->show();
        Control_Widget->show();
        SHOW_WINDOW_NORMAL
    } else {
        pImpl->title_widget->hide();
        Control_Widget->hide();
        SHOW_WINDOW_MAXSIZE
    }

    is_full_screen = !is_full_screen;
}

bool MainWidget::is_fullscreen_can_changed()
{
    return is_full_screen
           && (!pImpl->setting->auto_play_next
               || pImpl->xuan_ji_widget->IsEnd) ;
}

bool MainWidget::is_can_auto_play_next()
{
    return pImpl->setting->auto_play_next
           && (*(pImpl->player_widget))->isRuning ;
}

void MainWidget::on_play_finished()
{
    pImpl->control_widget->on_time_changed(0);
    pImpl->control_widget->on_douration_changed(0);

    if (is_can_auto_play_next()) {
        pImpl->xuan_ji_widget->on_playNext_clicked();
    }

    if (is_fullscreen_can_changed()) {
        on_Fullscreen_changed();
    }
}

void MainWidget::on_stop_clicked()
{
    pImpl->player->mStop();
    Control_Widget->isRuning = false;
}

void MainWidget::on_loadImage_started(int page, QStringList list)
{
    LoadImage *download = new LoadImage(page);
    download->setFileName(list);
    connect(download, SIGNAL(loadImageFinished(CLASS, QPixmap, QString, QString)),
            this, SLOT(on_loadImage_finished(CLASS, QPixmap, QString, QString)));
    download->start();
    emit send_status(page);
}


void MainWidget::on_nextPage_loaded(CLASS type)
{
    if (pImpl->can_update[type]) {
        QString url = pImpl->pyinit->gotoNextPage(
                          pImpl->name[type],
                          pImpl->pages[type]++)[0];
        auto *tmp = new mThread(
            type,
            bind(&PyScript::connect_img_url,
                 pImpl->pyinit,
                 url,
                 pImpl->name[type]
                )
        );
        connect(tmp, SIGNAL(load_finished(int, QStringList)),
                this, SLOT(on_loadImage_started(int, QStringList)));
        tmp->start();
        pImpl->can_update[type] = false;
    }
}

void MainWidget::on_loadImage_finished(
    CLASS index,
    QPixmap pix,
    QString name,
    QString url
)
{
    if (name.isEmpty() && url.isEmpty()) {
        pImpl->can_update[index] = true;
        return;
    }

    if (index == MUSIC && pix.width() < 170) {
        pix = pix.scaled(200, 112);
    }

    DetailLabel *l = new DetailLabel(pix, name, url);
    connect(l, SIGNAL(url_triggered(QString, QString)),
            this, SLOT(on_url_triggered(QString, QString)));
    static QWidget *index_widget = nullptr;
    index_widget = pImpl->stacked_widget->widget(index);
    qobject_cast<ListWidget *>(index_widget)->addDetailLabel(l);
}

void MainWidget::on_url_triggered(QString name, QString url)
{
    static bool good {false};

    if (url.startsWith(SHOW_PAGE)) {
        url = pImpl->pyinit->getplayUrl(url)[0];
    }

    good = pImpl->pyinit->GetVideoUrls(url, pImpl->setting->default_video_format);

    if (!url.isEmpty() && good) {
        auto *tmp = new mThread(NONE, bind(&PyScript::getAll,
                                           pImpl->pyinit,
                                           pImpl->stacked_widget->currentIndex(),
                                           url)
                               );
        connect(tmp, SIGNAL(load_finished(int, QStringList)),
                pImpl->xuan_ji_widget, SLOT(on_list_changed(int, QStringList)));
        pImpl->title_widget->on_turepage_triggered(PLAYER);
        tmp->start();
        pImpl->player->mPlay();
        Control_Widget->isRuning = true;
        SHOW_MSG(tr("Currently playing:").append(name));
        pImpl->title_widget->set_text(name);
    }
}
void MainWidget::on_url_ji_triggered(QString name, QString url)
{
    static bool good {false};

    if (name == url) {
        if (is_full_screen) {
            on_Fullscreen_changed();
        }

        return;
    }

    good = pImpl->pyinit->GetVideoUrls(url, pImpl->setting->default_video_format);

    if (good) {
        pImpl->stacked_widget->setCurrentIndex(PLAYER);
        Control_Widget->isRuning = true;
        pImpl->player->mPlay();
        SHOW_MSG(tr("Currently playing:")
                 .append(pImpl->title_widget->get_text())
                 .append("   ")
                 .append(name));
    }
}

void MainWidget::on_url_changed(CLASS classes, int type, QString name)
{
#define LOCATE  0
#define  CLASS  1
#define   TIME  2

    if (name == "全部") {
        name = "";
    }

    switch (type) {
        case LOCATE: //locate
            std::get<LOCATE>(pImpl->locate_class_time[classes]) = name;
            break;

        case CLASS:
            std::get< CLASS>(pImpl->locate_class_time[classes]) = name;
            break;

        case TIME:
            std::get<  TIME>(pImpl->locate_class_time[classes]) = name;
            break;
    }

    auto url = pImpl->pyinit->getUrlByName(
                   classes,
                   std::get<LOCATE>(pImpl->locate_class_time[classes]),
                   std::get< CLASS>(pImpl->locate_class_time[classes]),
                   std::get<  TIME>(pImpl->locate_class_time[classes])
               )[0];
    auto get_img_url = bind(&PyScript::connect_img_url,
                            pImpl->pyinit,
                            url,
                            pImpl->name[classes]
                           );
    auto *tmp = new mThread(classes, get_img_url);
    connect(tmp, SIGNAL(load_finished(int, QStringList)),
            this, SLOT(on_loadImage_started(int, QStringList)));
    tmp->start();
    qobject_cast<ListWidget *>(pImpl->stacked_widget->widget(classes))->reset();
#undef CLASS
}

void MainWidget::init_trayicon()
{
    pImpl->trayicon->setIcon(QIcon(":logo/logo_icon"));
    pImpl->trayicon->show();
    pImpl->trayicon->setToolTip("kuplayer");
    connect(pImpl->trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(on_trayIcon_clicked(QSystemTrayIcon::ActivationReason)));
    QMenu *tray_menu = new QMenu;
    pImpl->trayicon->setContextMenu(tray_menu);
    auto   *set_action = tray_menu->addAction(tr("Settings"));
    auto *about_action = tray_menu->addAction(tr("About"));
    auto  *exit_action = tray_menu->addAction(tr("Exit"));
    connect(set_action, SIGNAL(triggered()), pImpl->main_menu, SLOT(show()));
    connect(about_action, SIGNAL(triggered()), pImpl->main_menu, SLOT(show_about()));
    connect(exit_action, &QAction::triggered, [&]() {
        auto button = QMessageBox::question(
                          this,
                          tr("Exit"),
                          msg_font_style(tr("Really want to quit?")));

        if (button == QMessageBox::StandardButton::Yes) {
            close();
        }
    });
}

void MainWidget::init_setting()
{
    pImpl->setting->default_video_format = pImpl->iniFile->value("setting/format", "normal").toString();
    pImpl->setting->close_all = pImpl->iniFile->value("setting/close_all", true).toBool();
    pImpl->setting->auto_play_next = pImpl->iniFile->value("setting/auto_play_next", false).toBool();
    pImpl->setting->min_or_close = pImpl->iniFile->value("setting/min_or_close", false).toBool();
    pImpl->setting->start_when_pc_on = pImpl->iniFile->value("setting/start_when_pc_on", false).toBool();
    pImpl->setting->language = pImpl->iniFile->value("setting/language", true).toBool();
    pImpl->skin_widget->on_url_triggered("", pImpl->iniFile->value("setting/skin", ":/kin/0").toString());
    pImpl->main_menu->init_setting(pImpl->setting);
}

void MainWidget::to_inifile()
{
    pImpl->iniFile->setValue("setting/skin", get_skin());
    pImpl->iniFile->setValue("setting/format", pImpl->setting->default_video_format);
    pImpl->iniFile->setValue("setting/close_all", pImpl->setting->close_all);
    pImpl->iniFile->setValue("setting/auto_play_next", pImpl->setting->auto_play_next);
    pImpl->iniFile->setValue("setting/min_or_close", pImpl->setting->min_or_close);
    pImpl->iniFile->setValue("setting/start_when_pc_on", pImpl->setting->start_when_pc_on);
    pImpl->iniFile->setValue("setting/language", pImpl->setting->language);
}
