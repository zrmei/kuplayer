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

#ifndef CONTROL_LABEL_H
#define CONTROL_LABEL_H
#include "common.h"

#include <QLabel>
class QMouseEvent;


KUPLAYER_NAMESPACE_BEGIN //namespace begin

class ControlLabel final : public QLabel
{
    Q_OBJECT
signals:
    void clicked();
public:
    ControlLabel(const QString &name, QWidget *parent = 0);
protected:
    void mousePressEvent(QMouseEvent *);
};

KUPLAYER_NAMESPACE_END //namespace end

#endif // CONTROL_LABEL_H
