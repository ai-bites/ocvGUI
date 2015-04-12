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
#include <opencv2/stitching/stitcher.hpp>

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
#include "Dialogs/histogramdialog.h"
#include "Dialogs/matchesdialog.h"
#include "Dialogs/contourdialog.h"
#include "Dialogs/epipolardialog.h"
#include "Dialogs/logodialog.h"


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
    QThread * currentThread;

signals:
    void sendVideoIpImage(QImage);
    void sendCapture();
    void sendMorpOper(QString, int, int);
    void sendAddNoiseParams(QString,int,int,double,double);
    void sendColorSpaceParams(int);
    void sendBlur(int idx,int kernelL,
          int kernelH, double sigmaX, double sigmaY,int medianKernel);
    void sendSobelParams(int,bool,int,int,int,double,int,int);
    void sendCannyParams(int,int,bool,bool);
    void sendHoughLinesParams(int lineMethodIdx, int lineThreshold,
                              int lineRhoRes, int lineThetaRes, bool lineEdgeDetect);
    void sendHoughCirclesParams(double, double,int, int, bool);
    void sendHarrisParams(int blockSize, int aperture, double kValue, int threshold);
    void sendFeatureParams(int threshold, int methodIdx,
                           double siftThresh, double siftLineSensThresh,
                           double surfThresh);
    void sendHistogramParams(int, bool);
    void sendContourParams(int, bool, int, bool, bool);


private slots:
    // all actions triggered from tools menu
    void on_action_Open_triggered();
    void on_action_Close_triggered();
    void on_action_Save_triggered();
    void on_actionMorphology_triggered();
    void on_actionAdd_Noise_triggered();

    // All handlers for signals emitted from dialog boxes
    void handleMorphSignal(QString choice, int h, int w);
    void handleAddNoise(QString method, int, int, double, double);
    void handleLogoSignal(double, int, int);
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
    void handleHistogram(int, bool showHistEqImg);
    void handleMatchImages(cv::Mat firstImg, cv::Mat secondImg, bool isShow, string toDisplay, bool isStitch);
    void handleEpipolarImages(Mat firstImg, Mat secondImg, bool isShow, string toDisplay, int fMatMethodIdx,
                                          bool showInFirst, bool showInSec, double confLevel,
                                          double epiRatio, double minDist, int minHess);
    void handleContour(int edgeThresh, bool doBlur, int methodIdx, bool showRect, bool ShowCircle);


    void handleImageOpen();
    void handleVideoOpen(VideoCapture capture);

    void displayImage(Mat, Mat, string);

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

    void on_actionHistogram_triggered();

    void on_saveOutput_clicked();

    void on_actionMatches_triggered();

    void on_closeButton_clicked();

    void on_actionContour_triggered();

    void on_actionEpipolar_triggered();

    void on_logoCheckBox_clicked();

    void on_actionLogo_triggered();

private:
    Ui::MainWindow *ui;
    void displayOp();
    void refreshIpImg();
    bool isVideoStopped;

};

#endif // MAINWINDOW_H
