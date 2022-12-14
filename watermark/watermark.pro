QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

MOC_DIR = moc
RCC_DIR = rcc
OBJECTS_DIR = obj
UI_DIR = ui

unix{
    INCLUDEPATH += /usr/include/opencv4 /usr/include/opencv4/opencv2
    LIBS += -lopencv_core -lopencv_features2d \
            -lopencv_highgui -lopencv_imgcodecs \
            -lopencv_imgproc -lopencv_video \
            -lopencv_videoio
}

SOURCES += \
    blindwatermark.cpp \
    main.cpp \
    normalwatermark.cpp \
    widget.cpp

HEADERS += \
    blindwatermark.h \
    normalwatermark.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
