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
    VideoCapture * capture;
    bool liveStreamStop;
    void doMorphOperation(QString name, int, int);
    ImgProcess * ip;

signals:
    void sendVidoeOpImg(QImage);

public slots:
    void displayOpVideo();
};

#endif // VIDEOPROCESS_H
