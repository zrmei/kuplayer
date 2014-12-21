/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (0)131-5898-7498
*       Date : 2014/10/29
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "ui_control_classes.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

base_set_weidget::base_set_weidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::base_set_weidget)
{
    ui->setupUi(this);
}

base_set_weidget::~base_set_weidget()
{
    delete ui;
}

/*******************************************************************************************/

play_set_widget::play_set_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::play_set_widget)
{
    ui->setupUi(this);    
}
play_set_widget::~play_set_widget()
{
    delete ui;
}
/*******************************************************************************************/

about_widget::about_widget( QWidget *parent) :
    QWidget(parent),
    ui(new Ui::about_widget)
{
    ui->setupUi(this);
}

about_widget::~about_widget()
{
    delete ui;
}

#include <QDesktopServices>
#include <QUrl>
void about_widget::on_label_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

