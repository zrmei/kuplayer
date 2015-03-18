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
#ifndef COMMON_H
#define COMMON_H

#if defined(__GNUC__) && !defined(__clang__)
    #if (__GNUC__ < 4) || (__GNUC__ == 4 && __GNUC_MINOR__ < 6)
        #error Please use gcc4.6.0 or higher to support some new feature.
    #endif
#elif defined(__clang__)
    #if (__clang_major__ < 3) || (__clang_major__ == 3 && __clang_minor__ < 4)
        #error Please use clang3.5.0 or higher to support some new feature.
    #endif
#endif

#ifndef NO_KUPLAYER_NAMESPACE
    #define KUPLAYER_NAMESPACE_BEGIN namespace mei { namespace kuplayer {
    #define KUPLAYER_NAMESPACE_END } }
    #define USR_NAMESPACE_KUPLAYER using namespace mei::kuplayer;
    #define NAMESPACE_KUPLAYER mei::kuplayer
#else
    #define KUPLAYER_NAMESPACE_BEGIN
    #define KUPLAYER_NAMESPACE_END
    #define USR_NAMESPACE_KUPLAYER
    #define NAMESPACE_KUPLAYER
#endif

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QPainter>
#include <QApplication>

#include <algorithm>
#include <functional>
#include <memory>

using std::for_each;
using std::find_if;
using std::find_if_not;
using std::bind;
using std::function;

KUPLAYER_NAMESPACE_BEGIN //namespace begin


extern int SHADOW_WIDTH;
extern int SHADOW_HEIGHT;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern QString SHOW_PAGE;

typedef unsigned short CLASS;
const CLASS TV{0}, MOVIE{1}, ZONGYI{2}, MUSIC{3}, COMIC{4}, PLAYER{5}, NONE{5};


KUPLAYER_NAMESPACE_END // namespace end
#endif
