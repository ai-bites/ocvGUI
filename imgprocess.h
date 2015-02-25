#ifndef IMGPROCESS_H
#define IMGPROCESS_H

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

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <qglobal.h>

class ImgProcess
{
public:
    ImgProcess();

    cv::Mat image;
    cv::Mat grayImage;
    cv::Mat opImage;
    cv::Mat logo;

    /** All functions to enable different actions on images **/
    // Do morphological operations
    void doMorphOper(QString name,int, int);
    // Add a given type of noise
    void addNoise(QString name, int val);
    // Add a logo to image
    void addLogo(float alpha,int h, int l);
    // Convert to a new colourspace
    void toColourSpace(int);
    // Do blurring of input image
    void doBlur(int idx, int, int, double, double, int);
    // Do sobel and laplacian operations
    void doSobelAndLapOper(int, bool, int, int, int, double, int, int);
    // Do canny edge detection
    void doCannyOper(int kernel, int threshold, bool applyBlur, bool isL2Grad);
    // Do hough transform for extracting lines
    void doHoughLineTransform(
            int lineMethodIdx, int lineThreshold,
            int lineRhoRes, int lineThetaRes, bool lineEdgeDetect);
    // Do hough transform for circles
    void doHoughCircleTransform(
            double cannyThresh, double detectThresh, int minRad, int maxRad, bool applyBlur);
};

#endif // IMGPROCESS_H
