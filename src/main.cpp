/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/

#include "common.h"
#include "gloal_func.h"
#include "kuplayer.h"
#include "program_options.h"
#include "pyscript.h"
#include "ui_control_classes.h"

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
    if(iniFile->value("setting/language",true).toBool()){
        translator.load(QString(":/qm/kuplayer_zn"));
    }else{
        translator.load(QString(":/qm/kuplayer_en"));
    }
    a.installTranslator(&translator);
    
    QString ico_path = QDir::homePath()+"/.kuplayer/kuplayer.ico";
    if(!QFileInfo(ico_path).isFile()){
        QPixmap(":/logo/logo").save(ico_path);
    }
    
    std::shared_ptr<NAMESPACE_KUPLAYER::PyScript> pyinit = 
            std::make_shared<NAMESPACE_KUPLAYER::PyScript>();
    if( !pyinit.get()->getShowList() ){
        QMessageBox::warning(NULL,QObject::tr("Error"),
        NAMESPACE_KUPLAYER::msg_font_style(QObject::tr("Network error,Please try later !")));
        a.quit();
        return -1;
    }

    QDesktopWidget *desk = new QDesktopWidget;
    QSplashScreen *splash = new QSplashScreen(QPixmap(":/logo/welcome"),Qt::WindowStaysOnTopHint);

    int x = (desk->screen(0)->width() - splash->width()) >> 1;
    int y = (desk->screen(0)->height() - splash->height()) >> 1;

    splash->move(x,y);
    splash->show();

    NAMESPACE_KUPLAYER::MainWidget w(pyinit.get(),ico_path);
    
    a.connect(&w,&NAMESPACE_KUPLAYER::MainWidget::send_status,[&](int index)
    {
        static int  time_ = 0;
        static auto showMessage = bind(&QSplashScreen::showMessage,
                                       splash,
                                       _1,
                                       Qt::AlignBottom|Qt::AlignRight,
                                       Qt::black);
        switch (index) {
        case 0:
            showMessage(QObject::tr("Initializing the TV module ..."));
            ++ time_;
            break;
        case 1:
            showMessage(QObject::tr("Initializing the Movie module..."));
            ++ time_;
            break;
        case 2:
            showMessage(QObject::tr("Initializing the Zongyi module..."));
            ++ time_;
            break;
        case 3:
            showMessage(QObject::tr("Initializing the Music module..."));
            ++ time_;
            break;
        case 4:
            ++ time_;
            showMessage(QObject::tr("Initializing the Comic module..."));
            break;
        default:
            break;
        }
        if(time_ == 5){ 
            w.showNormal(); delete splash; delete desk; 
            w.disconnect(SIGNAL(send_status(int)));
        }
        qDebug() <<"if can use , this will be see";
    });
    w.move(x,y);
    w.setIniFile(iniFile.get());
    a.processEvents();
    a.processEvents();
    a.processEvents();
    a.processEvents();
    w.hide();
    
    return a.exec();
}
