/*
   Copyright (C) 2015 MeiZhaorui(Mason) <listener_mei@163.com>
   
   The File is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   The File is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with the Library; if not, see
   <http://www.gnu.org/licenses/>.
*/


#include "common.h"
#include "gloal_func.h"
#include "kuplayer.h"
#include "program_options.h"
#include "pyscript.h"
#include "ui_control_classes.h"
USR_NAMESPACE_KUPLAYER


#include <QDesktopWidget>
#include <QSplashScreen>
#include <QMessageBox>
#include <QTranslator>
#include <QTextCodec>
#include <QDir>
#include <QSettings>

#include <QtSingleApplication>


int main(int argc, char *argv[])
{
    if (!opt::program_options(argc, argv, VERSION)) {
        return -1;
    }

    QtSingleApplication a("listener_mei@kuplayer",argc, argv);
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

    if (a.isRunning()) {
        a.sendMessage("", 800);
        return 0;
    }
    qDebug() << a.id() << ": is running";

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("System"));
    QTranslator   translator;
    std::shared_ptr<QSettings> iniFile(new QSettings(
                                           QDir::homePath() + "/.kuplayer/kuplayer.conf",
                                           QSettings::IniFormat));

    if (iniFile->value("setting/language", true).toBool()) {
        translator.load(QString(":/qm/kuplayer_zn"));
    } else {
        translator.load(QString(":/qm/kuplayer_en"));
    }

    a.installTranslator(&translator);
    QString ico_path = QDir::homePath() + "/.kuplayer/kuplayer.ico";

    if (!QFileInfo(ico_path).isFile()) {
        QPixmap(":/logo/logo").save(ico_path);
    }

    std::shared_ptr<PyScript> pyinit = std::make_shared<PyScript>();

    if (!pyinit.get()->getShowList()) {
        QMessageBox::warning(NULL,
                             QObject::tr("Error"),
                             msg_font_style(QObject::tr("Network error,Please try later !")));
        a.quit();
        return -1;
    }

    QDesktopWidget *desk = new QDesktopWidget;
    QSplashScreen *splash = new QSplashScreen(QPixmap(":/logo/welcome"), Qt::WindowStaysOnTopHint);
    int x = (desk->screen(0)->width() - splash->width()) >> 1;
    int y = (desk->screen(0)->height() - splash->height()) >> 1;
    splash->move(x, y);
    splash->show();

    MainWidget w(pyinit.get(), ico_path);
    a.setActivationWindow(&w, false);
    a.connect(&a, &QtSingleApplication::messageReceived, [&](const QString &) {
        w.setWindowState(Qt::WindowActive);
        w.showNormal();
        qDebug() << "The first instance activated !";
    });

    a.connect(&w, &MainWidget::send_status, [&](int index) {
        static int  time_ = 0;
        static auto showMessage = bind(&QSplashScreen::showMessage,
                                       splash,
                                       _1,
                                       Qt::AlignBottom | Qt::AlignRight,
                                       Qt::black);

        switch (index) {
            case TV:
                showMessage(QObject::tr("Initializing the TV module ..."));
                a.processEvents();
                ++ time_;
                break;

            case MOVIE:
                showMessage(QObject::tr("Initializing the Movie module..."));
                a.processEvents();
                ++ time_;
                break;

            case ZONGYI:
                showMessage(QObject::tr("Initializing the Zongyi module..."));
                a.processEvents();
                ++ time_;
                break;

            case MUSIC:
                showMessage(QObject::tr("Initializing the Music module..."));
                a.processEvents();
                ++ time_;
                break;

            case COMIC:
                showMessage(QObject::tr("Initializing the Comic module..."));
                a.processEvents();
                ++ time_;
                break;

            default:
                break;
        }

        if (5 == time_) {
            w.showNormal();
            w.disconnect(SIGNAL(send_status(int)));
            delete splash; delete desk;
        }
    });
    
    w.move(x, y);
    w.setIniFile(iniFile.get());
    w.hide();
    
    return a.exec();
}