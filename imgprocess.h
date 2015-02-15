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

class ImgProcess
{
public:
    ImgProcess();

    cv::Mat image;
    cv::Mat grayImage;
    cv::Mat opImage;

    /** All functions to enable different action **/
    void doMorphOper(QString name);
};

#endif // IMGPROCESS_H
