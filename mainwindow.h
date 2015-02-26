#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QSlider>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextEdit>
#include <QString>
#include <QLabel>
#include <QMutex>
#include <QImage>
#include <QWaitCondition>
#include <iostream>
#include <QtWidgets/QtWidgets>
#include <QtCore>
#include <QThread>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "imgprocess.h"
#include "videoprocess.h"
#include "Dialogs/morphologydialog.h"
#include "Dialogs/noisedialog.h"
#include "Dialogs/colourdialog.h"
#include "Dialogs/blurdialog.h"
#include "Dialogs/sobeldialog.h"
#include "Dialogs/cannydialog.h"
#include "Dialogs/houghdialog.h"
#include "Dialogs/harrisdialog.h"
#include "Dialogs/featuresdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool isImgLoaded;
    bool isVideoLoaded;
    ImgProcess * ip;
    VideoProcess * vp;
    VideoCapture cap;

signals:
    void sendVideoIpImage(QImage);
    void sendCapture();
    void sendMorpOper(QString, int, int);

private slots:
    // all actions triggered from tools menu
    void on_action_Open_triggered();
    void on_action_Close_triggered();
    void on_action_Save_triggered();
    void on_actionMorphology_triggered();
    void on_actionAdd_Noise_triggered();

    // All handlers for signals emitted from dialog boxes
    void handleMorphSignal(QString choice, int h, int w);
    void handleSnPNoiseSignal(QString name, int n);
    void handleColorDialogSignal(int idx);
    void handleBlurDialogSignal(int idx, int, int, double, double, int);
    void handleSobelDialogSignal(int currentIdx, bool applyBlur,
                                 int kernel, int dx, int dy, double dxWeight,
                                 int delta,int scale);
    void handleCannySignal(int kernel, int threshold, bool applyBlur, bool isL2Grad);
    void handleHoughLineSignal(int,int,int,int,bool);
    void handleHoughCircleSignal(double, double,int, int, bool);
    void handleHarrisSignal(int blockSize, int aperture, double kValue, int threshold);
    void handleFeatureVals(int, int, double, double, double);

    void handleImageOpen();
    void handleVideoOpen(VideoCapture capture);


    void on_pushButton_clicked();

    void on_StartLiveCheckBox_clicked(bool checked);

    void updateVideoOpImage(QImage);
    void updateVideoIpImage(QImage);
    void startVideoOp();

    void on_actionColour_Space_triggered();

    void on_actionBlur_triggered();

    void on_actionSobel_triggered();

    void on_actionCanny_triggered();

    void on_actionHough_triggered();

    void on_actionCorners_triggered();

    void on_actionFAST_triggered();

private:
    Ui::MainWindow *ui;
    void displayOp();
    bool isVideoStopped;

    QThread * currentThread;

};

#endif // MAINWINDOW_H
