QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

OBJECTS_DIR = obj
RCC_DIR = rcc
MOC_DIR = moc
UI_DIR = ui

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    conversion.cpp \
    main.cpp \
    mainwindow.cpp \
    mapobject.cpp \
    mapobjectcircle.cpp \
    mapobjectline.cpp \
    mapobjectpolygon.cpp \
    mapobjectprivate.cpp \
    mapscene.cpp \
    mapsceneprivate.cpp \
    maptilelayerlistmodel.cpp \
    maptilenetwork.cpp \
    maptileobject.cpp \
    maptilesource.cpp \
    maptilesourceamap.cpp \
    maptilesourcecomposite.cpp \
    maptilesourceconfigurationwidget.cpp \
    maptilesourcedelegate.cpp \
    maptilesourcegrid.cpp \
    maptilesourceosm.cpp \
    maptilesourceprivate.cpp \
    mapview.cpp \
    mapviewprivate.cpp \
    position.cpp

HEADERS += \
    conversion.h \
    mainwindow.h \
    mapobject.h \
    mapobjectcircle.h \
    mapobjectline.h \
    mapobjectpolygon.h \
    mapobjectprivate.h \
    mapscene.h \
    mapsceneprivate.h \
    maptilelayerlistmodel.h \
    maptilenetwork.h \
    maptileobject.h \
    maptilesource.h \
    maptilesourceamap.h \
    maptilesourcecomposite.h \
    maptilesourceconfigurationwidget.h \
    maptilesourcedelegate.h \
    maptilesourcegrid.h \
    maptilesourceosm.h \
    maptilesourceprivate.h \
    mapview.h \
    mapviewprivate.h \
    position.h

FORMS += \
    mainwindow.ui \
    maptilesourceconfigurationwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
