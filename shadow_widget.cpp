/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/
#include "common.h"
#include "shadow_widget.h"

#include <QMouseEvent>
#include <QDesktopWidget>

ShadowWidget::ShadowWidget(QWidget *parent)
    : QWidget(parent)
    ,desk(new QDesktopWidget)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Widget);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);
}

ShadowWidget::~ShadowWidget()
{

}
void ShadowWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
    QWidget::mouseDoubleClickEvent(ev);
    if(ev->pos().y() < TITLE_CONTROL_HEIGHT){
        if(is_max){
            this->setGeometry(original_rect);
        }else{
            original_rect = this->geometry();
            this->setGeometry(desk->geometry());
        }
        is_max = !is_max;
    }
}

void ShadowWidget::mousePressEvent(QMouseEvent *ev)
{
    QWidget::mousePressEvent(ev);
    if(ev->button() == Qt::LeftButton
            && ev->pos().y() < TITLE_CONTROL_HEIGHT){
        isLeftPressDown = true;
        m_ptPressGlobal = ev->globalPos() - this->pos();
    }
}

void ShadowWidget::mouseMoveEvent(QMouseEvent *ev)
{
    QWidget::mouseMoveEvent(ev);
    if(isLeftPressDown){
        this->move(ev->globalPos() - m_ptPressGlobal);
    }
}

void ShadowWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    QWidget::mouseReleaseEvent(ev);
    isLeftPressDown = false;
}
#include <QFileInfo>
void ShadowWidget::paintEvent(QPaintEvent *)
{
    if(!is_full_screen){
        if(!QFileInfo(skin).isFile()){
            skin = ":/skin/0";
        }
        QPainter painter(this);
        drawWindowShadow(painter);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawPixmap(QRect(SHADOW_WIDTH, SHADOW_WIDTH,
                                 this->width()-2*SHADOW_WIDTH,
                                 this->height()-2*SHADOW_WIDTH),
                           QPixmap(skin));
    }
}

void ShadowWidget::drawWindowShadow(QPainter &painter)
{
    //绘制左上角、左下角、右上角、右下角、上、下、左、右边框
    QList<QPixmap> pixmaps;
    pixmaps.append(QPixmap(":/shadow/shadow_left"));
    pixmaps.append(QPixmap(":/shadow/shadow_right"));
    pixmaps.append(QPixmap(":/shadow/shadow_top"));
    pixmaps.append(QPixmap(":/shadow/shadow_bottom"));
    pixmaps.append(QPixmap(":/shadow/shadow_left_top"));
    pixmaps.append(QPixmap(":/shadow/shadow_right_top"));
    pixmaps.append(QPixmap(":/shadow/shadow_left_bottom"));
    pixmaps.append(QPixmap(":/shadow/shadow_right_bottom"));

    painter.drawPixmap(0, 0, SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[4]);
    painter.drawPixmap(width()-SHADOW_WIDTH, 0, SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[5]);
    painter.drawPixmap(0,height()-SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[6]);
    painter.drawPixmap(width()-SHADOW_WIDTH, height()-SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[7]);
    painter.drawPixmap(0, SHADOW_WIDTH, SHADOW_WIDTH, height()-2*SHADOW_WIDTH, pixmaps[0].scaled(SHADOW_WIDTH, height()-2*SHADOW_WIDTH));
    painter.drawPixmap(width()-SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, height()-2*SHADOW_WIDTH, pixmaps[1].scaled(SHADOW_WIDTH, height()- 2*SHADOW_WIDTH));
    painter.drawPixmap(SHADOW_WIDTH, 0, width()-2*SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[2].scaled(width()-2*SHADOW_WIDTH, SHADOW_WIDTH));
    painter.drawPixmap(SHADOW_WIDTH, height()-SHADOW_WIDTH, width()-2*SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[3].scaled(width()-2*SHADOW_WIDTH, SHADOW_WIDTH));

}

#ifdef CAN_RESIZE
MainWidget::MainWidget(QWidget *parent)
    : ShadowWidget(parent)
{
}

MainWidget::~MainWidget()
{

}
#include <QApplication>
void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    ShadowWidget::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton) {
        isLeftPressDown = false;
        this->releaseMouse();
        dir = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    ShadowWidget::mousePressEvent(event);
    switch(event->button()) {
    case Qt::LeftButton:
        isLeftPressDown = true;
        m_ptPressGlobal = event->globalPos();
    default:
        break;
    }
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    ShadowWidget::mouseMoveEvent(event);
    QPoint currentGlobalPoint = event->globalPos();
    if(!isLeftPressDown) {
        this->region(currentGlobalPoint);
    } else if(dir != NONE){
        SetDrayMove(currentGlobalPoint);
        m_ptPressGlobal  = currentGlobalPoint;
    }else{
        setCursor(QCursor(Qt::ArrowCursor));
    }
}

void MainWidget::SetDrayMove(QPoint gloPoint)
{
    //计算偏差
    int ndX = gloPoint.x() - m_ptPressGlobal.x();
    int ndY = gloPoint.y() - m_ptPressGlobal.y();
    //获得主窗口位置信息
    QRect rectWindow = geometry();
    //判别方向
    if(dir & TOP){
        rectWindow.setTop(rectWindow.top()+ndY);
    }else if(dir & BOTTOM){
        rectWindow.setBottom(rectWindow.bottom()+ndY);
    }
    if(dir & RIGHT)
    {
        rectWindow.setRight(rectWindow.right()+ndX);
    }else if(dir & LEFT){
        rectWindow.setLeft(rectWindow.left()+ndX);
    }
    if(rectWindow.width()< minimumWidth() ||
            rectWindow.height() < minimumHeight()){
        return;
    }
    //重新设置窗口位置为新位置信息
    setGeometry(rectWindow);
}
#define PADDING 5
void MainWidget::region(const QPoint &cursorGlobalPoint)
{
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());
    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();
    this->setCursor(QCursor(Qt::ArrowCursor));
    if(tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
        // 左上角
        dir = LEFTTOP;
//        qDebug()<<"this->setCursor(QCursor(Qt::SizeFDiagCursor));";
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        // 右下角
        dir = RIGHTBOTTOM;
//        qDebug()<<"this->setCursor(QCursor(Qt::SizeFDiagCursor));";
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        //左下角
        dir = LEFTBOTTOM;
//        qDebug()<<"this->setCursor(QCursor(Qt::SizeBDiagCursor));";
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
        // 右上角
        dir = RIGHTTOP;
//          qDebug()<<"this->setCursor(QCursor(Qt::SizeBDiagCursor));";
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x()) {
        // 左边
        dir = LEFT;
//          qDebug()<<"this->setCursor(QCursor(Qt::SizeHorCursor));";
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= rb.x() && x >= rb.x() - PADDING) {
        // 右边
        dir = RIGHT;
//        qDebug()<<"this->setCursor(QCursor(Qt::SizeHorCursor));";
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if(y >= tl.y() && y <= tl.y() + PADDING){
        // 上边
        dir = TOP;
//        qDebug()<<"this->setCursor(QCursor(Qt::SizeVerCursor));";
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= rb.y() && y >= rb.y() - PADDING) {
        // 下边
        dir = BOTTOM;
//        qDebug()<<"this->setCursor(QCursor(Qt::SizeVerCursor));";
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }else {
        // 默认
        dir = NONE;
 //       qDebug()<<"this->setCursor(QCursor(Qt::ArrowCursor));";
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}
#endif
