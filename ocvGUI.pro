#-------------------------------------------------
#
# Project created by QtCreator 2015-02-12T21:25:58
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ocvGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imgprocess.cpp \
    Dialogs/morphologydialog.cpp \
    Dialogs/noisedialog.cpp \
    videoprocess.cpp \
    Dialogs/blurdialog.cpp

HEADERS  += mainwindow.h \
    imgprocess.h \
    videoprocess.h \
    Dialogs/morphologydialog.h \
    Dialogs/noisedialog.h \
    Dialogs/blurdialog.h

INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib \
   -lopencv_core \
   -lopencv_highgui \
   -lopencv_imgproc \
   -lopencv_features2d \
   -lopencv_calib3d

FORMS    += mainwindow.ui \
    morphologydialog.ui \
    noisedialog.ui
