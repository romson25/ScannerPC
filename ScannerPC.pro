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
    controlPanel/scanningmode.h \
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


INCLUDEPATH += /home/adam/Pobrane/cgal-master/Installation/cmake/modules
INCLUDEPATH += /usr/include/boost/
LIBS += -L/usr/include/boost/lib/ -lboost_thread -lboost_system

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_highgui
LIBS += -lopencv_imgproc
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_features2d

LIBS += -lassimp

INCLUDEPATH +=   /usr/include/
LIBS        += -L/usr/include/
LIBS        += -lCGAL

#--bruceCL
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../library/bruceCL/build/release/release/ -lbruceCL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../library/bruceCL/build/release/debug/ -lbruceCL
else:unix:!macx: LIBS += -L$$PWD/../../library/bruceCL/build/release/ -lbruceCL

INCLUDEPATH += $$PWD/../../library/bruceCL
DEPENDPATH += $$PWD/../../library/bruceCL

#--bruceRL
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../library/bruceRL/build/release/ -lbruceRL
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../library/bruceRL/build/debug/ -lbruceRL
#else:unix:!macx: LIBS += -L$$PWD/../../library/bruceRL/build/ -lbruceRL

#INCLUDEPATH += $$PWD/../../library/bruceRL/include
#DEPENDPATH += $$PWD/../../library/bruceRL/include
