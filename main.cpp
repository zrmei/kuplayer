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

#include <QApplication>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    if( !opt::complie(argc,argv,VERSION) ){
        return -1;
    }
    printf("%s",VERSION);
    QApplication a(argc, argv);
    std::shared_ptr<PyScript> pyinit = std::make_shared<PyScript>();
    if( !pyinit.get()->getShowList() ){
        QMessageBox::warning(NULL,"错误",
                             "<span style=\" font-family:'SimSun'; color:#ff0000;\">\
                             <h2>网络未连接或网络慢，请检查网络再重试！</h2></span>");
        a.quit();
        return -1;
    }

    a.connect(&a,SIGNAL(lastWindowClosed()),&a,SLOT(quit()));

    QDesktopWidget *desk = new QDesktopWidget;

    QPixmap pixmap(":/logo/welcome");
    QSplashScreen *splash = new QSplashScreen(pixmap,Qt::WindowStaysOnTopHint);

    int x = (desk->screen(0)->width() - splash->width()) /2;
    int y = (desk->screen(0)->height() - splash->height()) /2;

    splash->move(x,y+2);
    splash->show();

    kuplayer w(pyinit.get());
    w.move(x,y);
    splash->finish(&w);
    w.show();

    delete desk;
    delete splash;

    return a.exec();
}
