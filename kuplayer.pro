#-------------------------------------------------
#
# Project created by QtCreator 2014-10-01T10:53:06
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = kuplayer
TEMPLATE = app

PRECOMPILED_HEADER = src/common.h

QMAKE_CXXFLAGS += -std=c++11 -s
QMAKE_CXXFLAGS_RELEASE += -DQT_NO_DEBUG_OUTPUT
QMAKE_LFLAGS_RELEASE += -Wl,-s
QMAKE_LFLAGS += -Wl,-rpath,./lib

SOURCES += src/control_widget.cpp \
    src/detail_label.cpp \
    src/kuplayer.cpp \
    src/list_widget.cpp \
    src/main.cpp \
    src/main_menu.cpp \
    src/mplayer_widget.cpp \
    src/play_list_widget.cpp \
    src/push_button.cpp \
    src/pyscript.cpp \
    src/select_label.cpp \
    src/shadow_widget.cpp \
    src/skin_widget.cpp \
    src/title_widget.cpp \
    src/ui_control_classes.cpp

HEADERS  += src/common.h \
    src/control_widget.h \
    src/detail_label.h \
    src/kuplayer.h \
    src/list_widget.h \
    src/loadimage.h \
    src/main_menu.h \
    src/mplayer_widget.h \
    src/mthread.h \
    src/play_list_widget.h \
    src/program_options.h \
    src/push_button.h \
    src/pyscript.h \
    src/select_label.h \
    src/shadow_widget.h \
    src/skin_widget.h \
    src/system_notify.h \
    src/title_widget.h \
    src/ui_control_classes.h

unix: {
    INCLUDEPATH += /usr/include/glib-2.0/
    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include/
    INCLUDEPATH += /usr/include/gdk-pixbuf-2.0/
    INCLUDEPATH += /usr/include/python2.7/

    INCLUDEPATH += $$PWD/resources/includes
    INCLUDEPATH += $$PWD/resources/includes/QtAV

    LIBS += $$PWD/resources/libs/libboost_python.a
    LIBS += $$PWD/resources/libs/libboost_system.a

    LIBS += -L$$PWD/resources/libs  -loptions -lQtAV -lpython2.7 -lnotify
}
RESOURCES += \
    src/kuplayer.qrc

RC_FILE += \
    kuplayer.rc

OTHER_FILES += \
    kuplayer.ico \
    kuplayer.rc

FORMS += \
    src/about_widget.ui \
    src/base_set_weidget.ui \
    src/play_set_widget.ui

