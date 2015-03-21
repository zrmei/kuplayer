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
#include "shadow_widget.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <QDesktopWidget>
#include <QApplication>
#include <QMouseEvent>
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

    if (ev->button() == Qt::LeftButton && ev->pos().y() < TITLE_CONTROL_HEIGHT) {
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
        static QPixmap qskin(skin);
        
        QPainter painter(this);
        drawWindowShadow(painter);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawPixmap(QRect(SHADOW_WIDTH, SHADOW_WIDTH,
                                 width() - 2 * SHADOW_WIDTH,
                                 height() - 2 * SHADOW_WIDTH),
                           qskin);
    }
}

void ShadowWidget::drawWindowShadow(QPainter &painter)
{
    static int w_height{0},w_width{0};
    w_height = height() - 2 * SHADOW_WIDTH;
    w_width  =  width() - 2 * SHADOW_WIDTH;
    
    static auto pix0 = (*pixmaps)[0].scaled(SHADOW_WIDTH, w_height);
    static auto pix1 = (*pixmaps)[1].scaled(SHADOW_WIDTH, w_height);
    static auto pix2 = (*pixmaps)[2].scaled(w_width, SHADOW_WIDTH);
    static auto pix3 = (*pixmaps)[3].scaled(w_width, SHADOW_WIDTH);
    
    //绘制左上角、左下角、右上角、右下角、上、下、左、右边框
    painter.drawPixmap(0, 0, SHADOW_WIDTH, SHADOW_WIDTH, (*pixmaps)[4]);
    painter.drawPixmap(width() - SHADOW_WIDTH, 0, SHADOW_WIDTH, SHADOW_WIDTH, (*pixmaps)[5]);
    painter.drawPixmap(0, height() - SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, (*pixmaps)[6]);
    painter.drawPixmap(width() - SHADOW_WIDTH, height() - SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, (*pixmaps)[7]);
    painter.drawPixmap(0, SHADOW_WIDTH, SHADOW_WIDTH, w_height,pix0) ;
    painter.drawPixmap(width() - SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, w_height, pix1);
    painter.drawPixmap(SHADOW_WIDTH, 0, w_width, SHADOW_WIDTH, pix2);
    painter.drawPixmap(SHADOW_WIDTH, height() - SHADOW_WIDTH, w_width, SHADOW_WIDTH, pix3);
}