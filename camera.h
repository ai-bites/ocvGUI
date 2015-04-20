#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class Camera
{
public:
    Camera();

    // actual caliberation
    void calibrate();
    // read and store input file names in inputFiles vector
    void readCalibImages();
    // used to remove any distortion after caliberation
    void removeDistortion(Mat &image);
    // a vector to store all input file names
    vector<string> inputFiles;

private:

    // choice for the size of chess board corners
    int boardWidth, boardHeight;

    // output image points. Will hold a vector of
    // image points and object points corresponding to each
    // viewpoint
    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;

    // vector of image corner and object corner vectors
    // for 5 caliberation images, the length of this vector
    // will be 5, if all images are good.
    std::vector<std::vector<cv::Point2f> > imagePoints;
    std::vector<std::vector<cv::Point3f> > objectPoints;

    // caliberation output params
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;

    // tackle distortions in calib output
    cv::Mat undistImg;

};

#endif // CAMERA_H
