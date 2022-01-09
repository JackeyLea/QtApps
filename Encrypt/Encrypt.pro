QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32{
    INCLUDEPATH += $$PWD/include
LIBS += -L$$PWD/lib
LIBS += -lcrypto-1_1-x64 -lssl-1_1-x64

}

unix {
    INCLUDEPATH += /usr/include/openssl-1.0/
    #LIBS += -L/usr/lib/openssl-1.0
    LIBS += -lcrypto -lssl
}

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
