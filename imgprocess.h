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
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/stitching/stitcher.hpp>
//#include <opencv2/stitching/warpers.hpp>
#include <opencv2/stitching/detail/matchers.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "robustmatcher.h"

using namespace cv;


#include <qglobal.h>

class ImgProcess
{
public:
    ImgProcess();

    cv::Mat image;
    cv::Mat grayImage;
    cv::Mat opImage;
    cv::Mat logo;

    // if we cascade, the output of previous operation
    // becomes the input for the next operation
    bool isCascadeEnabled;
    // dynamically provides image to process based on cascading
    // or other conditions
    Mat getImageToProcess();

    // for matching, correspondece, fund matrix computation, etc
    cv::Mat firstImg;
    cv::Mat secondImg;
    std::vector<cv::KeyPoint> imgOneKeyPts, imgTwoKeyPts;
    std::vector<cv::Point2f> imgOneKpFlt, imgTwoKpFlt;
    std::vector< DMatch > goodMatches;

    // need these when we compute fundamental matrix
    std::vector<cv::KeyPoint> getKeyPoints() {return this->keypoints;}

    /** All functions to enable different actions on images **/
    // Do morphological operations
    void doMorphOper(QString name,int, int);
    // Add a given type of noise
    void addNoise(QString type, int whiteVal, int blackVal, double mean, double var);
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
    // Do harris corner extraction
    void doHarrisCorner(int blockSize, int aperture,double kValue, int threshold);
    // Do feature extraction using FAST extractor
    void doFeatureExtract(int fastThresh, int methodIdx,
                          double surfThresh, double siftThresh, double lineSensThresh);
    // compute Fundamental matrix given two images
    Mat computeFundMatrix(int methodIdx);
    // Given an input image and number of bins, compute histogram and display as output
    void doHistogram(int numBins, bool showHistEqImg);
    // Finds the match between two given images
    void doMatchImages(Mat, Mat, bool);
    // Stitch images and display the stiched images as one
    void doStitchImages(Mat firstImg, Mat secondImg, bool isShow);
    // Draws the epipolar lines for given set of images
    void doEpipolarLines(bool isShow, string toDisplay, int fMatMethodIdx,
                         bool showInFirst, bool showInSec, double confLevel,
                         double epiRatio, double minDist, int minHess);
    // compute homography between two given images
    void computeHomography();
    // does Flann matching and returns the matches
    std::vector< DMatch > doFlannMatching(Mat descOne, Mat descTwo);
    // Draw contour of connected objects
    void drawContours(int, bool, int methodIdx, bool showRect, bool showCircle);
    // Does adaptive thresholding on the given image
    void doAdaptiveThreshold(int blockSize, double maxVal,
                             int methodIdx, int threshTypeIdx, double constant);

private:
    std::vector<cv::KeyPoint> keypoints;
    void drawEpipolarLines(cv::Mat fundMatrix, cv::Mat keyPoints, cv::Mat image);
};

#endif // IMGPROCESS_H
