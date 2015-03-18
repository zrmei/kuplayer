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

#ifndef SKIN_WIDGET_H
#define SKIN_WIDGET_H
#include "common.h"
#include "shadow_widget.h"

KUPLAYER_NAMESPACE_BEGIN //namespace begin

class SkinWidget final : public ShadowWidget
{
    Q_OBJECT
signals:
    void skin_change_clicked(QString);
public:
    explicit SkinWidget(QWidget *parent = 0);
    ~SkinWidget() {}

public slots:
    void on_showed();
    void on_url_triggered(QString, QString);

private:
    void init_skin(QString name);
    void find_file(QString path);

    QStringList pic_list;
    QString PIC_PATH;

    struct SkinWidget_Impl;
    std::shared_ptr<SkinWidget_Impl> pImpl;
};

KUPLAYER_NAMESPACE_END // namespace end
#endif // SKIN_WIDGET_H
