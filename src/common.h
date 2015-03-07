/*********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*      Phone : (+86)131-5898-7498
*       Date : 2014/10/13
*       host : Ubuntu x86_64 3.13.0-37
 *********************************************/
#ifndef COMMON_H
#define COMMON_H

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

KUPLAYER_NAMESPACE_BEGIN //namespace begin

const int SHADOW_WIDTH = 5;
const int SHADOW_HEIGHT = 5;
const int WINDOW_WIDTH = 1002;
const int WINDOW_HEIGHT = 657;

const QString SHOW_PAGE = "http://www.youku.com/show_page/";

#if defined(__GNUC__) && !defined(__clang__) 
    #if (__GNUC__ < 4) || (__GNUC__ == 4 && __GNUC_MINOR__ < 6)
        #error Please use gcc4.6.0 or higher to support some new feature.
    #endif
#elif defined(__clang__)
    #if (__clang_major__ < 3) || (__clang_major__ == 3 && __clang_minor__ < 4)
        #error Please use clang3.5.0 or higher to support some new feature.
    #endif
#endif

typedef unsigned int CLASS;
const CLASS TV = 0,MOVIE = 1,ZONGYI = 2,MUSIC = 3,COMIC = 4,PLAYER = 5,NONE = 6;

KUPLAYER_NAMESPACE_END // namespace end
#endif
