#-------------------------------------------------
#
# Project created by QtCreator 2022-07-01T10:37:59
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = structure
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mytableview.cpp \
    itemdelegate.cpp

HEADERS  += mainwindow.h \
    mytableview.h \
    itemdelegate.h

FORMS    += mainwindow.ui
