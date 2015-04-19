#-------------------------------------------------
#
# Project created by QtCreator 2015-04-19T21:32:20
#
#-------------------------------------------------

QT       += widgets network testlib av avwidgets

TARGET = tst_kuplayertest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_kuplayertest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
DEFINES += 
SOURCES += ../src/control_widget.cpp \
    ../src/detail_label.cpp \
    ../src/kuplayer.cpp \
    ../src/list_widget.cpp \
    ../src/main_menu.cpp \
    ../src/mplayer_widget.cpp \
    ../src/play_list_widget.cpp \
    ../src/push_button.cpp \
    ../src/pyscript.cpp \
    ../src/select_label.cpp \
    ../src/shadow_widget.cpp \
    ../src/skin_widget.cpp \
    ../src/title_widget.cpp \
    ../src/ui_control_classes.cpp \
    ../src/control_label.cpp \
    ../src/common.cpp \
    ../src/conf_info.cpp

HEADERS  += ../src/common.h \
    ../src/control_widget.h \
    ../src/detail_label.h \
    ../src/kuplayer.h \
    ../src/list_widget.h \
    ../src/loadimage.h \
    ../src/main_menu.h \
    ../src/mplayer_widget.h \
    ../src/mthread.h \
    ../src/play_list_widget.h \
    ../src/program_options.h \
    ../src/push_button.h \
    ../src/pyscript.h \
    ../src/select_label.h \
    ../src/shadow_widget.h \
    ../src/skin_widget.h \
    ../src/system_notify.h \
    ../src/title_widget.h \
    ../src/ui_control_classes.h \
    ../src/gloal_func.h \
    ../src/control_label.h \
    ../src/conf_info.h

RESOURCES += \
    ../src/kuplayer.qrc
#-------------------------------------------------
#
# Project created by QtCreator 2014-10-01T10:53:06
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11 -DQT_NO_DEBUG_OUTPUT -DAV_NO_DEBUG_OUTPUT -DPYTHON_DONOT_CATCH_EXCEPTION
#DEFINES += NO_WIFI_TEST

QMAKE_LFLAGS += -Wl,-rpath,"../lib"
QMAKE_LFLAGS_RELEASE += -Wl,-s -Wl,-O2



unix: {
    INCLUDEPATH += /usr/include/python2.7/ \
                    $$PWD/../resources/libs
                    
    LIBS += -L$$PWD/../resources/libs \
            $$PWD/../resources/libs/libboost_python.a \
            $$PWD/../resources/libs/libboost_system.a \
            -lQtSingleApplication   \
            -loptions_64 \
            -lpython2.7 

    exists( /usr/include/libnotify/notify.h ) {
        INCLUDEPATH += /usr/include/glib-2.0/
        INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include/
        INCLUDEPATH += /usr/include/gdk-pixbuf-2.0/

        LIBS += -lnotify 
        DEFINES += USE_NOTIFY
    } else {
        message("Can not find libnotify.so. You can: sudo apt-get install libnotify4-dev")
    }
}


