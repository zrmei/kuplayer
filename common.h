/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/
#ifndef COMMON_H
#define COMMON_H
#include <algorithm>
#include <functional>
using std::for_each;
using std::find_if;
using std::find_if_not;
using std::bind;
using std::function;

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QPainter>
#include <QApplication>

#define SHADOW_WIDTH  5
#define SHADOW_HEIGHT 5

typedef unsigned int CLASS;
const CLASS TV = 0,MOVIE = 1,ZONGYI = 2,MUSIC = 3,COMIC = 4,PLAYER = 5,NONE = 6;

template<typename T>
inline void set_no_margin(T *layout){
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
}
template<typename T>
inline void delete_list(T* store){
    while (store->size()){
        delete store->at(0);
        store->removeAt(0);
    }
    delete store;
}

#endif
