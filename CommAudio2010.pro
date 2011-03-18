#-------------------------------------------------
#
# Project created by DTRAIN 3/12/2011, 8:17 PM
#
#-------------------------------------------------

QT   += core gui

TARGET = CommAudio2010
TEMPLATE = app
OBJECTS_DIR = obj/
DESTDIR = bin/
MOC_DIR = moc/

LIBS 	+= C:\Qt\2010.05\mingw\lib\libws2_32.a

SOURCES += src/main.cpp\
        src/application/mainwindow.cpp \
        src/network/network.cpp \
        src/network/errors.cpp \
        src/network/udpserver.cpp \
        src/network/socket.cpp \
        src/application/aboutwindow.cpp \
    src/application/manwindow.cpp \
    src/application/appwindow.cpp \
    src/network/tcpserver.cpp \
    src/network/buffer.cpp \
    src/network/filetransfer.cpp \
    src/network/filetransferin.cpp

HEADERS  += src/application/mainwindow.h \
        src/network/network.h \
        src/network/errors.h \
        src/network/server.h \
        src/network/udpserver.h \
        src/network/socket.h \
        C:\Qt\2010.05\mingw\include\ws2tcpip.h \
        C:\Qt\2010.05\mingw\include\winsock2.h \
        src/application/aboutwindow.h \
    src/application/manwindow.h \
    src/application/appwindow.h \
    src/network/tcpserver.h \
    src/network/buffer.h \
    src/network/filetransfer.h \
    ../Admin/Desktop/filetransferin.h \
    src/network/filetransferin.h


FORMS   += src/application/mainwindow.ui \
        src/application/aboutwindow.ui \
    src/application/manwindow.ui \
    src/application/appwindow.ui
