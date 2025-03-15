TEMPLATE = app
TARGET = server_chat

QT += core gui 
QT += network
QT += widgets

greaterThan(AT_MAJOR_VERSION, 4): QT += widgets

SOURCES += *.cpp
	

HEADERS += *.h

CONFIG += qt warn_on release
CONFIG += c++11