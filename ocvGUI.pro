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
    Dialogs/blurdialog.cpp \
    Dialogs/colourdialog.cpp \
    Dialogs/sobeldialog.cpp \
    Dialogs/cannydialog.cpp \
    Dialogs/houghdialog.cpp \
    Dialogs/harrisdialog.cpp \
    Dialogs/featuresdialog.cpp \
    camera.cpp \
    Dialogs/histogramdialog.cpp \
    Dialogs/matchesdialog.cpp \
    Dialogs/contourdialog.cpp \
    Dialogs/epipolardialog.cpp

HEADERS  += mainwindow.h \
    imgprocess.h \
    videoprocess.h \
    Dialogs/morphologydialog.h \
    Dialogs/noisedialog.h \
    Dialogs/blurdialog.h \
    Dialogs/colourdialog.h \
    Dialogs/sobeldialog.h \
    Dialogs/cannydialog.h \
    Dialogs/houghdialog.h \
    Dialogs/harrisdialog.h \
    Dialogs/featuresdialog.h \
    camera.h \
    Dialogs/histogramdialog.h \
    Dialogs/matchesdialog.h \
    Dialogs/contourdialog.h \
    Dialogs/epipolardialog.h \
    robustmatcher.h

INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib \
   -lopencv_core \
   -lopencv_highgui \
   -lopencv_imgproc \
   -lopencv_features2d \
   -lopencv_calib3d \
   -lopencv_nonfree \
   -lopencv_flann \
   -lopencv_legacy \
   -lopencv_stitching


FORMS    += mainwindow.ui \
    Dialogs/morphologydialog.ui \
    Dialogs/noisedialog.ui \
    Dialogs/colourdialog.ui \
    Dialogs/blurdialog.ui \
    Dialogs/sobeldialog.ui \
    Dialogs/cannydialog.ui \
    Dialogs/houghdialog.ui \
    Dialogs/harrisdialog.ui \
    Dialogs/featuresdialog.ui \
    Dialogs/histogramdialog.ui \
    Dialogs/matchesdialog.ui \
    Dialogs/contourdialog.ui \
    Dialogs/epipolardialog.ui

RESOURCES += \
    images.qrc
