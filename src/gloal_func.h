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

#ifndef GLOAL_H
#define GLOAL_H
#include "common.h"
KUPLAYER_NAMESPACE_BEGIN //namespace begin

template<typename T>
inline void set_no_margin(T *layout)
{
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}
template<typename T>
inline void delete_list(T *store)
{
    while (store->size()) {
        delete store->at(0);
        store->removeAt(0);
    }

    delete store;
}

inline QString msg_font_style(const QString &text)
{
    QString tmp;
    tmp.insert(0, "<span style=\"font-family:'SimSun'; color:#ff0000;\"><h2>");
    tmp.insert(tmp.size(), text);
    tmp.insert(tmp.size(), "</h2></span>");
    return std::move(tmp);
}

KUPLAYER_NAMESPACE_END // namespace end
#endif // GLOAL_H
