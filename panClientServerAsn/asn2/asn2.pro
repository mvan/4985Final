#-------------------------------------------------
#
# Project created by QtCreator 2011-02-15T09:30:41
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = asn2
TEMPLATE = app

LIBS     += libwsock32 -lws2_32

SOURCES += main.cpp\
        mainwindow.cpp \
    serverwindow.cpp \
    clientwindow.cpp \
    connectdlg.cpp \
    startserverdlg.cpp \
    serverlistenthread.cpp \
    clientprocessthread.cpp

HEADERS  += mainwindow.h \
    serverwindow.h \
    clientwindow.h \
    connectdlg.h \
    startserverdlg.h \
    serverlistenthread.h \
    clientprocessthread.h

FORMS    += mainwindow.ui \
    serverwindow.ui \
    clientwindow.ui \
    connectdlg.ui \
    startserverdlg.ui
