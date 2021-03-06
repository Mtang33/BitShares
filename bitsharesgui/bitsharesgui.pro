#-------------------------------------------------
#
# Project created by QtCreator 2013-08-14T18:36:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXX=/opt/local/bin/g++
QMAKE_LINK=/opt/local/bin/g++
QMAKE_CC=/opt/local/bin/gcc

TARGET = bitsharesgui
TEMPLATE = app

!win32 {
QMAKE_CXXFLAGS += -std=c++11
}

win32 {
LIBS += ../Debug/bshare.lib ../fc/Debug/fc_debug.lib
} else {
LIBS += ../libbshare.a ../fc/libfc.a
LIBS += -L${BOOST_LIBRARY_DIR} -lboost_context -lboost_thread -lboost_system -lboost_chrono -lboost_filesystem -lboost_system -lboost_date_time -lboost_coroutine
LIBS += ../libbshare.a ../fc/libfc.a 
}


INCLUDEPATH += ../include 
INCLUDEPATH += /usr/local/include
INCLUDEPATH += ../fc/include

#PRECOMPILED_HEADER = bitsharesguiprecomp.h

SOURCES += main.cpp \
    bitsharesmainwindow.cpp \
    profileeditor.cpp \
    bitsharestreemodel.cpp \
    chatwidget.cpp \
    selfsizingwidget.cpp \
    selfsizingmainwindow.cpp

HEADERS  += \
    bitsharesmainwindow.h \
    profileeditor.h \
    bitsharestreemodel.h \
    chatwidget.h \
    selfsizingwidget.h \
    selfsizingmainwindow.h \
    bitsharesguiprecomp.h

FORMS    += \
    bitsharesmainwindow.ui \
    profileeditor.ui \
    chatwidget.ui


#SOURCES += main.cpp \
#    bshare_gui.cpp \
#    bshare_viewport.cpp \
#    bshare_viewports/bshare_viewport_mail.cpp

#HEADERS  += \
#    bshare_gui.h \
#    bshare_viewport.hpp \
#    bshare_viewports/bshare_viewport_mail.hpp

#FORMS    += \
#    bshare_gui.ui

RESOURCES += \
    bitshares.qrc

