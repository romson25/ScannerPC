#-------------------------------------------------
#
# Project created by QtCreator 2015-10-22T12:44:04
#
#-------------------------------------------------

QT += core gui
QT += network
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = ScannerPC
TEMPLATE = app
CONFIG  += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    controlPanel/controlpanel.cpp \
    reconstructor/laserreconstructor.cpp \
    reconstructor/photogrammetryreconstructor.cpp \
    reconstructor/reconstructorbase.cpp \
    renderWindow/eye/eye.cpp \
    renderWindow/renderObject/axis/axis.cpp \
    renderWindow/renderObject/model/model.cpp \
    renderWindow/renderObject/skeleton/skeleton.cpp \
    renderWindow/scene.cpp

HEADERS  += mainwindow.h \
    controlPanel/controlpanel.h \
    controlPanel/guimode.h \
    reconstructor/laserreconstructor.h \
    reconstructor/photogrammetryreconstructor.h \
    reconstructor/reconstructorbase.h \
    renderWindow/eye/direction.h \
    renderWindow/eye/distance.h \
    renderWindow/eye/eye.h \
    renderWindow/renderObject/axis/axis.h \
    renderWindow/renderObject/model/model.h \
    renderWindow/renderObject/skeleton/skeleton.h \
    renderWindow/renderObject/irenderobject.h \
    renderWindow/rendermode.h \
    renderWindow/scene.h

FORMS    += mainwindow.ui \
    controlPanel/controlpanel.ui \
    renderWindow/scene.ui

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib
LIBS += -lassimp
LIBS += -lCGAL
#LIBS += -ltheia
LIBS += -lopencv_core
LIBS += -lopencv_highgui
LIBS += -lopencv_imgproc
LIBS += -lopencv_imgcodecs

#--bruceCL
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bruceCL/build/Debug/release/ -lbruceCL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bruceCL/build/Debug/debug/ -lbruceCL
else:unix:!macx: LIBS += -L$$PWD/../bruceCL/build/Debug/ -lbruceCL

INCLUDEPATH += $$PWD/../bruceCL
DEPENDPATH += $$PWD/../bruceCL

#--bruceRL
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bruceRL/build/Domyślna/release/ -lbruceRL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bruceRL/build/Domyślna/debug/ -lbruceRL
else:unix:!macx: LIBS += -L$$PWD/../bruceRL/build/Domyślna/ -lbruceRL

INCLUDEPATH += $$PWD/../bruceRL/include
DEPENDPATH += $$PWD/../bruceRL/include

#--brucePL
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../prototype/brucePL/build/Domyślna/release/ -lbrucePL
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../prototype/brucePL/build/Domyślna/debug/ -lbrucePL
#else:unix:!macx: LIBS += -L$$PWD/../../prototype/brucePL/build/Domyślna/ -lbrucePL

#INCLUDEPATH += $$PWD/../../prototype/brucePL/include
#DEPENDPATH += $$PWD/../../prototype/brucePL/include
