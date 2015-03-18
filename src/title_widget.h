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

#ifndef TITLE_WIDGET_H
#define TITLE_WIDGET_H
#include "common.h"
#include <QList>
#include <QLabel>

#define TITLE_WIDGET_HEIGHT 72
#define TITLE_WIDGET_WIDTH 100
#define TITLE_CONTROL_HEIGHT 29
class QMouseEvent;

KUPLAYER_NAMESPACE_BEGIN //namespace begin

class TypeLabel final : public QLabel
{
    Q_OBJECT
public:
    explicit TypeLabel(QString title, QWidget *paren = 0);
    virtual ~TypeLabel();
    void setMousePress(bool mouse_press);
signals:
    void clicked(QString);
protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *event);
private:
    void painterInfo(int, int, int);

private:
    bool mouse_hover {false};
    bool mouse_pressed {false};
};

class TitleWidget : public QWidget
{
    Q_OBJECT
signals:
    void skin_clicked();
    void min_clicked();
    void close_clicked();
    void menu_clicked();
    void ture_page(int);
public:
    explicit TitleWidget(QWidget *parent_ = 0);
    virtual ~TitleWidget();
    void set_text(QString);
    QString get_text() const;
public slots:
    void on_turepage_triggered(int);
    void on_turepage_triggered(QString);
private:
    QStringList down_title {
        tr("TVShow"), tr("Movies"), tr("Variety"),
        tr("Music"), tr("Cartoon"), tr("Player")
    };
    struct TitleWidget_Impl;
    std::unique_ptr<TitleWidget_Impl> pImpl;

};

KUPLAYER_NAMESPACE_END // namespace end
#endif // TITLE_WIDGET_H
