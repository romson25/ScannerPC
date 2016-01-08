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
#bububaba


SOURCES += main.cpp\
        mainwindow.cpp \
    controlpanel.cpp \
    scene.cpp \
    Eye/eye.cpp

HEADERS  += mainwindow.h \
    controlpanel.h \
    scene.h \
    rendermode.h \
    guimode.h \
    Eye/distance.h \
    Eye/eye.h \
    Eye/direction.h

FORMS    += mainwindow.ui \
    controlpanel.ui \
    scene.ui

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_imgcodecs

LIBS += -lassimp

INCLUDEPATH +=   /usr/include/
LIBS        += -L/usr/include/
LIBS        += -lCGAL
LIBS        += -lgmp
LIBS        += -lassimp
LIBS        += -lmpfr

INCLUDEPATH +=/usr/include/boost/
LIBS += -L/usr/include/boost/lib/ -lboost_thread -lboost_system

INCLUDEPATH += /usr/include/eigen3/
LIBS += -L/usr/include/eigen3/eigen

QMAKE_CXXFLAGS += -frounding-math -O3

#INCLUDEPATH += /home/adam/Downloads/cgal-master/Installation/cmake/modules
INCLUDEPATH += /home/adam/Pobrane/cgal-master/Installation/cmake/modules

DISTFILES += \
    shaders/fragmentaxis.frag \
    shaders/vertexaxis.vert \
    shaders/fragmentmodel.frag \
    shaders/vertexmodel.vert \
    shaders/vertexskeleton.vert \
    shaders/fragmentskeleton.frag


#--LOAD bruceCL

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../library/bruceCL/build/release/release/ -lbruceCL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../library/bruceCL/build/release/debug/ -lbruceCL
else:unix:!macx: LIBS += -L$$PWD/../../library/bruceCL/build/release/ -lbruceCL

INCLUDEPATH += $$PWD/../../library/bruceCL
DEPENDPATH += $$PWD/../../library/bruceCL


#--LOAD bruceCGAL

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../library/bruceRL/build/release/ -lbruceCGAL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../library/bruceRL/build/debug/ -lbruceCGAL
else:unix:!macx: LIBS += -L$$PWD/../../library/bruceRL/build/ -lbruceCGAL

INCLUDEPATH += $$PWD/../../library/bruceRL/include
DEPENDPATH += $$PWD/../../library/bruceRL/include
