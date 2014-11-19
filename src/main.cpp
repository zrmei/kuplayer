/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/

#include "common.h"
#include "kuplayer.h"
#include "program_options.h"
#include "pyscript.h"
#include "ui_control_classes.h"

#include <QObject>
#include <QApplication>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QMessageBox>
#include <QTranslator>
#include <QTextCodec>
#include <QDir>
#include <QSettings>

int main(int argc, char *argv[])
{
    if( !opt::program_options(argc,argv,VERSION) ){
        return -1;
    }
    QApplication a(argc, argv);
    a.connect(&a,SIGNAL(lastWindowClosed()),&a,SLOT(quit()));
    

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("System")); 
    QTranslator   translator;
    std::shared_ptr<QSettings> iniFile(new QSettings(
                                           QDir::homePath()+"/.kuplayer/kuplayer.conf",
                                           QSettings::IniFormat));
    if(iniFile->value("setting/language",true).toBool())
        translator.load(QString(":/qm/kuplayer_zn"));
    else
        translator.load(QString(":/qm/kuplayer_en"));
    a.installTranslator(&translator);
    
    
    std::shared_ptr<PyScript> pyinit = std::make_shared<PyScript>();
    if( !pyinit.get()->getShowList() ){
        QMessageBox::warning(NULL,QObject::tr("Error"),
                             msg_font_style(QObject::tr("Network error,Please try later !")));
        a.quit();
        return -1;
    }

    QDesktopWidget *desk = new QDesktopWidget;
    QSplashScreen *splash = new QSplashScreen(QPixmap(":/logo/welcome"),Qt::WindowStaysOnTopHint);

    int x = (desk->screen(0)->width() - splash->width()) /2;
    int y = (desk->screen(0)->height() - splash->height()) /2;

    splash->move(x,y);
    splash->show();

    kuplayer w(pyinit.get());
    w.move(x,y);
    w.setIniFile(iniFile.get());
    splash->finish(&w);
    w.show();

    delete desk;
    delete splash;

    return a.exec();
}
