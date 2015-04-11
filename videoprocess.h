#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include <QDialog>
#include <QObject>
#include <QApplication>
#include <QThread>
#include <QtCore>

#include <qsignaltransition.h>
#include <qsignalmapper.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <unistd.h>

#include "imgprocess.h"

using namespace cv;
using namespace std;

class VideoProcess : public QObject
{
    Q_OBJECT

public:
    VideoProcess();
    VideoCapture capture;
    ImgProcess * ip;

    cv::Mat frame;
    cv::Mat destFrame;
    bool isVideoStopped;

signals:
    void sendVidoeOpImg(QImage);
    void sendVideoOpImg();

public slots:
    void displayOpVideo();

    /** All operations possible on the video
        are slots that can be invoked as separate
        threads
    **/
    void doMorphOper(QString name, int, int);
    void addNoise(QString type, int whiteVal, int blackVal, double mean, double var);
    void toColourSpace(int csIdx);
    void doBlur(int idx, int kernelL, int kernelH,double sigmaX, double sigmaY, int medianKernel);
    void doSobelAndLapOper(int,bool,int,int,int,double,int,int);
    void doCannyOper(int kernel, int threshold, bool applyBlur, bool isL2Grad);
    void doHoughLineTransform(int lineMethodIdx, int lineThreshold,
                              int lineRhoRes, int lineThetaRes, bool lineEdgeDetect);
    void doHoughCircleTransform(double cannyThresh, double detectThresh,
                              int minRad, int maxRad, bool applyBlur);
    void doHarrisCorner(int blockSize, int aperture,double kValue, int threshold);
    void doFeatureExtract(int fastThresh, int methodIdx,
                              double siftThresh, double siftLineSensthresh, double surfThresh);
    void doHistogram(int numBins, bool showHistEqImg);
    void drawContours(int edgeThresh, bool doBlur, int methodIdx);

};

#endif // VIDEOPROCESS_H
