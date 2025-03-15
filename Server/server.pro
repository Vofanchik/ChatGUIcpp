TEMPLATE = app
TARGET = server_chat

QT += core gui sql
QT += network
QT += widgets

greaterThan(AT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
        main.cpp \
	mainwindow.cpp \
	Server.cpp \
	Database.cpp \
	RequestHandler.cpp \
	SHA_1_Wrapper.cpp
	

HEADERS += \
	mainwindow.h \
	Server.h \
	Database.h \
	RequestHandler.h \
	sha1.hpp

CONFIG += qt warn_on release
CONFIG += c++11