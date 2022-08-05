#-------------------------------------------------
#
# Project created by QtCreator 2022-07-05T09:47:06
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sender
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    myudpsocket.cpp

HEADERS  += widget.h \
    myudpsocket.h

FORMS    += widget.ui
