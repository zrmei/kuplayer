#-------------------------------------------------
#
# Project created by QtCreator 2014-10-01T10:53:06
#
#-------------------------------------------------

QT       += core gui widgets network av

TARGET = kuplayer
TEMPLATE = app

PRECOMPILED_HEADER = common.h

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_DEBUG +=  -g3 -O0
QMAKE_CXXFLAGS_RELEASE += -s

QMAKE_LFLAGS += -Wl,-rpath,./lib/
QMAKE_LFLAGS_DEBUG +=  -Winline
QMAKE_LFLAGS_RELEASE += -s

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
    main_menu.cpp

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
    main_menu.h

unix: {
    INCLUDEPATH += /usr/include/glib-2.0/
    DEPENDPATH += /usr/include/glib-2.0/

    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include/
    DEPENDPATH += /usr/lib/x86_64-linux-gnu/glib-2.0//include/

    INCLUDEPATH += /usr/include/gdk-pixbuf-2.0/
    DEPENDPATH += /usr/include/gdk-pixbuf-2.0/

    INCLUDEPATH += $$PWD/../../../../../usr/include/python2.7
    DEPENDPATH += $$PWD/../../../../../usr/include/python2.7

    LIBS += -L/usr/lib/i386-linux-gnu/ -lnotify
    LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lpython2.7

    LIBS += $$PWD/../../../../../usr/local/lib/x86_64-linux-gnu/libboost_python.a
    LIBS += $$PWD/../../../../../usr/local/lib/x86_64-linux-gnu/libboost_system.a

    LIBS += -L$$PWD -loptions
}
RESOURCES += \
    kuplayer.qrc

RC_FILE += kuplayer.rc

OTHER_FILES += \
    kuplayer.ico \
    liboptions.so

