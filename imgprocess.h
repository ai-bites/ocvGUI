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

    /** All functions to enable different action **/
    // Do morphological operations
    void doMorphOper(QString name,int, int);
    // Add a given type of noise
    void addNoise(QString name, int val);
    // add a logo to image
    void addLogo(float alpha,int h, int l);

};

#endif // IMGPROCESS_H
