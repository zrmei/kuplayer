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

#ifndef SHADOW_WIDGET_H
#define SHADOW_WIDGET_H

#include "common.h"

#include <QWidget>
class QDesktopWidget;

#define TITLE_CONTROL_HEIGHT 29
#define VALUE_DIS 5

KUPLAYER_NAMESPACE_BEGIN //namespace begin

class ShadowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShadowWidget(QWidget *parent = 0);
    virtual ~ShadowWidget();

    void change_skin(QString);
    
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *);
   
    void drawWindowShadow(QPainter &painter);
    
    QString skin;
    inline QString get_skin() const;
    
    bool is_full_screen {false};
    bool is_max {false};
    QRect original_rect;
    bool isLeftPressDown {false};
    QPoint m_ptPressGlobal;
    std::shared_ptr<QDesktopWidget> desk;
    std::shared_ptr<QList<QPixmap>> pixmaps;
};

inline QString ShadowWidget::get_skin() const
{
    return skin.split('/').last();
}

KUPLAYER_NAMESPACE_END // namespace end
#endif // SHADOW_WIDGET_H
