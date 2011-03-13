#-------------------------------------------------
#
# Project created by DTRAIN 3/12/2011, 8:17 PM
#
#-------------------------------------------------

QT   += core gui

TARGET = bin/CommAudio2010
TEMPLATE = app
OBJECTS_DIR = obj/
MOC_DIR = moc/

LIBS 	+= C:\Qt\2010.05\mingw\lib\libws2_32.a

SOURCES += src/main.cpp\
        src/application/mainwindow.cpp \
	src/network/network.cpp \
	src/network/errors.cpp \
	src/network/udpserver.cpp \
	src/network/socket.cpp

HEADERS  += src/application/mainwindow.h \
	src/network/network.h \
	src/network/errors.h \
	src/network/server.h \
	src/network/udpserver.h \
        src/network/socket.h \
        C:\Qt\2010.05\mingw\include\ws2tcpip.h \
        C:\Qt\2010.05\mingw\include\winsock2.h \


FORMS    += forms/mainwindow.ui \
