/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#include "common.h"
#include "gloal_func.h"
#include "shadow_widget.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <QMouseEvent>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QDir>

ShadowWidget::ShadowWidget(QWidget *parent)
    : QWidget(parent)
    , desk(new QDesktopWidget)
    , pixmaps(new QList<QPixmap>)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Widget);
    setAttribute(Qt::WA_TranslucentBackground);
    pixmaps->append(QPixmap(":/shadow/shadow_left"));
    pixmaps->append(QPixmap(":/shadow/shadow_right"));
    pixmaps->append(QPixmap(":/shadow/shadow_top"));
    pixmaps->append(QPixmap(":/shadow/shadow_bottom"));
    pixmaps->append(QPixmap(":/shadow/shadow_left_top"));
    pixmaps->append(QPixmap(":/shadow/shadow_right_top"));
    pixmaps->append(QPixmap(":/shadow/shadow_left_bottom"));
    pixmaps->append(QPixmap(":/shadow/shadow_right_bottom"));
}

ShadowWidget::~ShadowWidget()
{
}


void ShadowWidget::change_skin(QString pic_name)
{
    static const QString PIC_PATH {qApp->applicationDirPath() + "/../sources/img/skin/"};
    skin = PIC_PATH + pic_name;

    if (!QFileInfo(skin).isFile()) {
        pic_name = ":/skin/0";
    }

    update();
}

void ShadowWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
    QWidget::mouseDoubleClickEvent(ev);

    if (ev->pos().y() < TITLE_CONTROL_HEIGHT) {
        if (is_max) {
            setGeometry(original_rect);
        } else {
            original_rect = geometry();
            setGeometry(desk->geometry());
        }

        is_max = !is_max;
    }
}

void ShadowWidget::mousePressEvent(QMouseEvent *ev)
{
    QWidget::mousePressEvent(ev);

    if (ev->button() == Qt::LeftButton
        && ev->pos().y() < TITLE_CONTROL_HEIGHT) {
        isLeftPressDown = true;
        m_ptPressGlobal = ev->globalPos() - pos();
    }
}

void ShadowWidget::mouseMoveEvent(QMouseEvent *ev)
{
    QWidget::mouseMoveEvent(ev);

    if (isLeftPressDown) {
        move(ev->globalPos() - m_ptPressGlobal);
    }
}

void ShadowWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    QWidget::mouseReleaseEvent(ev);
    isLeftPressDown = false;
}

void ShadowWidget::paintEvent(QPaintEvent *)
{
    if (!is_full_screen) {
        QPainter painter(this);
        drawWindowShadow(painter);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawPixmap(QRect(SHADOW_WIDTH, SHADOW_WIDTH,
                                 width() - 2 * SHADOW_WIDTH,
                                 height() - 2 * SHADOW_WIDTH),
                           QPixmap(skin));
    }
}

void ShadowWidget::drawWindowShadow(QPainter &painter)
{
    //绘制左上角、左下角、右上角、右下角、上、下、左、右边框
    painter.drawPixmap(0, 0, SHADOW_WIDTH, SHADOW_WIDTH, (*pixmaps)[4]);
    painter.drawPixmap(width() - SHADOW_WIDTH, 0, SHADOW_WIDTH, SHADOW_WIDTH, (*pixmaps)[5]);
    painter.drawPixmap(0, height() - SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, (*pixmaps)[6]);
    painter.drawPixmap(width() - SHADOW_WIDTH, height() - SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, (*pixmaps)[7]);
    painter.drawPixmap(0, SHADOW_WIDTH, SHADOW_WIDTH, height() - 2 * SHADOW_WIDTH, (*pixmaps)[0].scaled(SHADOW_WIDTH, height() - 2 * SHADOW_WIDTH));
    painter.drawPixmap(width() - SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, height() - 2 * SHADOW_WIDTH, (*pixmaps)[1].scaled(SHADOW_WIDTH, height() - 2 * SHADOW_WIDTH));
    painter.drawPixmap(SHADOW_WIDTH, 0, width() - 2 * SHADOW_WIDTH, SHADOW_WIDTH, (*pixmaps)[2].scaled(width() - 2 * SHADOW_WIDTH, SHADOW_WIDTH));
    painter.drawPixmap(SHADOW_WIDTH, height() - SHADOW_WIDTH, width() - 2 * SHADOW_WIDTH, SHADOW_WIDTH, (*pixmaps)[3].scaled(width() - 2 * SHADOW_WIDTH, SHADOW_WIDTH));
}


#ifdef CAN_RESIZE
ResizedWidget::ResizedWidget(QWidget *parent)
    : ShadowWidget(parent)
{
    setMouseTracking(true);
}

ResizedWidget::~ResizedWidget()
{
}
#include <QApplication>
void ResizedWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isLeftPressDown = false;
        dir = M_NONE;
        setCursor(QCursor(Qt::ArrowCursor));
    }

    ShadowWidget::mouseReleaseEvent(event);
}

void ResizedWidget::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
        case Qt::LeftButton:
            m_isLeftPressDown = true;
            m_ptPressGlobal = event->globalPos();

        default:
            break;
    }

    ShadowWidget::mousePressEvent(event);
}

void ResizedWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentGlobalPoint = event->globalPos();
    qDebug() << "m_isLeftPressDown: " << m_isLeftPressDown;

    if (!m_isLeftPressDown) {
        setCursorStyle(currentGlobalPoint);
    } else if (dir != M_NONE) {
        setDrayMove(currentGlobalPoint);
        m_ptPressGlobal  = currentGlobalPoint;
    }

    ShadowWidget::mouseMoveEvent(event);
}

void ResizedWidget::setDrayMove(QPoint gloPoint)
{
    //计算偏差
    int ndX = gloPoint.x() - m_ptPressGlobal.x();
    int ndY = gloPoint.y() - m_ptPressGlobal.y();
    //获得主窗口位置信息
    QRect rectWindow = geometry();

    //判别方向
    if (dir & TOP) {
        rectWindow.setTop(rectWindow.top() + ndY);
    } else if (dir & BOTTOM) {
        rectWindow.setBottom(rectWindow.bottom() + ndY);
    }

    if (dir & RIGHT) {
        rectWindow.setRight(rectWindow.right() + ndX);
    } else if (dir & LEFT) {
        rectWindow.setLeft(rectWindow.left() + ndX);
    }

    if (rectWindow.width() < minimumWidth() ||
        rectWindow.height() < minimumHeight()) {
        return;
    }

    //重新设置窗口位置为新位置信息
    setGeometry(rectWindow);
}
#define PADDING 5
#include <QMessageBox>
void ResizedWidget::setCursorStyle(const QPoint &cursorGlobalPoint)
{
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());
    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();
    setCursor(QCursor(Qt::ArrowCursor));

    if (tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
        // 左上角
        dir = LEFTTOP;
        qDebug() << "setCursor(QCursor(Qt::SizeFDiagCursor));";
        setCursor(QCursor(Qt::SizeFDiagCursor));
        return;
    } else if (x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        // 右下角
        dir = RIGHTBOTTOM;
        qDebug() << "setCursor(QCursor(Qt::SizeFDiagCursor));";
        setCursor(QCursor(Qt::SizeFDiagCursor));
        return;
    } else if (x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        //左下角
        dir = LEFTBOTTOM;
        qDebug() << "setCursor(QCursor(Qt::SizeBDiagCursor));";
        setCursor(QCursor(Qt::SizeBDiagCursor));
        return;
    } else if (x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
        // 右上角
        dir = RIGHTTOP;
        qDebug() << "setCursor(QCursor(Qt::SizeBDiagCursor));";
        setCursor(QCursor(Qt::SizeBDiagCursor));
        return;
    } else if (x <= tl.x() + PADDING && x >= tl.x()) {
        // 左边
        dir = LEFT;
        qDebug() << "setCursor(QCursor(Qt::SizeHorCursor));";
        setCursor(QCursor(Qt::SizeHorCursor));
        return;
    } else if (x <= rb.x() && x >= rb.x() - PADDING) {
        // 右边
        dir = RIGHT;
        qDebug() << "setCursor(QCursor(Qt::SizeHorCursor));";
        setCursor(QCursor(Qt::SizeHorCursor));
        return;
    } else if (y >= tl.y() && y <= tl.y() + PADDING) {
        // 上边
        dir = TOP;
        qDebug() << "setCursor(QCursor(Qt::SizeVerCursor));";
        setCursor(QCursor(Qt::SizeVerCursor));
        return;
    } else if (y <= rb.y() && y >= rb.y() - PADDING) {
        // 下边
        dir = BOTTOM;
        qDebug() << "setCursor(QCursor(Qt::SizeVerCursor));";
        setCursor(QCursor(Qt::SizeVerCursor));
        return;
    }

    dir = M_NONE;
    QMessageBox::about(this, "AAAAAAAAAAA", "EEEEERRRROOORRRR");
    qDebug() << "setCursor(QCursor(Qt::ArrowCursor));";
    setCursor(QCursor(Qt::ArrowCursor));
}
#endif
