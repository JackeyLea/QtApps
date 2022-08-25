QT       += core gui widgets
QT       += sql
QT       += network

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

DESTDIR = bin
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/ui
RCC_DIR = tmp/rcc

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    base.hpp \
    mainwindow.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
