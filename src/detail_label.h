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

#ifndef DETAILLABEL_H
#define DETAILLABEL_H
#include "common.h"

#include <QLabel>
class QMouseEvent;

KUPLAYER_NAMESPACE_BEGIN //namespace begin

class Label : public QLabel
{
    Q_OBJECT
signals:
    void clicked();
public:
    explicit Label(QWidget *parent = 0);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
};

class DetailLabel : public QWidget
{
    Q_OBJECT
signals:
    void url_triggered(QString/*name*/, QString/*url*/);
    
public:
    DetailLabel(QWidget *parent = 0);
    DetailLabel(QPixmap, const QString &, const QString &, QWidget *parent = 0);
    virtual ~DetailLabel();

    inline void set_Pixmap(QPixmap &&img);
    inline void set_Url(const QString &url);
    inline void set_Title(const QString &title);
    QString text() const;

private slots:
    void this_url_triggered();

private:
    struct DetailLabel_Impl;
    std::unique_ptr<DetailLabel_Impl> pImpl;
};


KUPLAYER_NAMESPACE_END //namespace end
#endif // DETAILLABEL_H
