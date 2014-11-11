#-------------------------------------------------
#
# Project created by QtCreator 2014-10-01T10:53:06
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = kuplayer
TEMPLATE = app


PRECOMPILED_HEADER = common.h

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE += -DQT_NO_DEBUG_OUTPUT
QMAKE_LFLAGS += -Wl,-rpath,./lib/
QMAKE_LFLAGS_RELEASE += -DQT_NO_DEBUG_OUTPUT
SOURCES += main.cpp\
        kuplayer.cpp \
    detaillabel.cpp \
    pyscript.cpp \
    shadow_widget.cpp \
    title_widget.cpp \
    push_button.cpp \
    list_widget.cpp \
    mplayer_widget.cpp \
    control_widget.cpp \
    select_label.cpp \
    play_list_widget.cpp \
    skin_widget.cpp \
    main_menu.cpp \
    ui_control_classes.cpp

HEADERS  += kuplayer.h \
    detaillabel.h \
    loadimage.h \
    pyscript.h \
    common.h \
    shadow_widget.h \
    title_widget.h \
    push_button.h \
    list_widget.h \
    mplayer_widget.h \
    control_widget.h \
    mthread.h \
    select_label.h \
    play_list_widget.h \
    program_options.h \
    skin_widget.h \
    system_notify.h \
    main_menu.h \
    ui_control_classes.h

unix: {
    INCLUDEPATH += /usr/include/glib-2.0/
    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include/
    INCLUDEPATH += /usr/include/gdk-pixbuf-2.0/

    INCLUDEPATH += $$PWD/sources/includes
    INCLUDEPATH += $$PWD/sources/includes/QtAV
    INCLUDEPATH += $$PWD/sources/includes/python2.7

    LIBS += $$PWD/sources/libs/libboost_python.a
    LIBS += $$PWD/sources/libs/libboost_system.a

    LIBS += -L$$PWD/sources/libs  -loptions -lQtAV -lpython2.7 -lnotify
}
RESOURCES += \
    kuplayer.qrc

RC_FILE += \
    kuplayer.rc

OTHER_FILES += \
    kuplayer.ico

FORMS += \
    base_set_weidget.ui \
    about_widget.ui \
    play_set_widget.ui

