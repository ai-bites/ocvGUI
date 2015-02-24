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

public slots:
    void displayOpVideo();

    /** All operations possible on the video
        are slots that can be invoked as separate
        threads
    **/
    void doMorphOper(QString name, int, int);

};

#endif // VIDEOPROCESS_H
