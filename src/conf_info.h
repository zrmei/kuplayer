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

#ifndef CONF_INFO_H
#define CONF_INFO_H
#include "common.h"
#include <QString>
#include <QSettings>

KUPLAYER_NAMESPACE_BEGIN

struct conf_info {
    bool start_when_pc_on;
    bool close_all;
    bool min_or_close;
    bool auto_play_next;
    bool language;
    QString default_video_format;
};

inline void 
conf_info_to_file(conf_info* setting,QSettings* iniFile)
{
    setting->default_video_format = iniFile->value("setting/format", "flv").toString();
    setting->close_all = iniFile->value("setting/close_all", true).toBool();
    setting->auto_play_next = iniFile->value("setting/auto_play_next", false).toBool();
    setting->min_or_close = iniFile->value("setting/min_or_close", false).toBool();
    setting->start_when_pc_on = iniFile->value("setting/start_when_pc_on", false).toBool();
    setting->language = iniFile->value("setting/language", true).toBool();
}

inline void 
conf_info_from_file(conf_info*setting,QSettings* iniFile)
{
    iniFile->setValue("setting/format", setting->default_video_format);
    iniFile->setValue("setting/close_all", setting->close_all);
    iniFile->setValue("setting/auto_play_next", setting->auto_play_next);
    iniFile->setValue("setting/min_or_close", setting->min_or_close);
    iniFile->setValue("setting/start_when_pc_on", setting->start_when_pc_on);
    iniFile->setValue("setting/language", setting->language);
}

KUPLAYER_NAMESPACE_END
#endif // CONF_INFO_H
