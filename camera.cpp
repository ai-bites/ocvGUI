#include "camera.h"

Camera::Camera()
{
    boardWidth = 6;
    boardHeight = 7;
    distCoeffs = cv::Mat::zeros(8, 1, CV_64F);
    cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
}


void Camera::calibrate()
{
    // All needed images
    // for holding caliberation images,
    // rotation and translation.
    Mat tempImage;
    std::vector<cv::Mat> rvecs, tvecs;

    Size boardSize(boardWidth, boardHeight);

    // read caliberation images into inputFiles vector
    readCalibImages();

    // the points on the chessboard
    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;

    // set the object corners
    for (int i = 0; i < boardHeight; i++)
    {
        for (int j = 0; j < boardWidth; j++)
        {
            objectCorners.push_back(cv::Point3f(i, j, 0.0f));
        }
    }

    // read every calib image and do caliberation
    for (int i = 0; i < inputFiles.size(); i++)
    {
        tempImage = imread(inputFiles[i],0);
        bool found = cv::findChessboardCorners(tempImage, boardSize, imageCorners);
        if (found == false)
        {
            cout << "No corners found for input file No: " << i+1 << endl;
            continue;
        }

        cornerSubPix(tempImage, imageCorners,Size(11,11), Size(-1,-1),
                     TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));

        // pick only good points
        if (imageCorners.size() == boardSize.area())
        {
            imagePoints.push_back(imageCorners);
            objectPoints.push_back(objectCorners);
        }
        else cout << "image unfit for file No: " << i << endl;
    }
    cout << "done with all the input files" << endl;

    // If we reach here, we have done all the prep work for caliberation.
    // So now caliberate. The result will be stored in cameraMatrix and distCoeffs
    Size imgSize = tempImage.size();
    double rms = calibrateCamera(objectPoints,imagePoints,
                                 imgSize, cameraMatrix, distCoeffs, rvecs, tvecs);

    cout << "rms is: " << rms << endl;

    this->removeDistortion(tempImage);

}


void Camera::removeDistortion(Mat &image)
{
    Mat map1, map2;
    // initialize undistortion using the params computed during caliberation
    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs,
    cv::Mat(), cv::Mat(), image.size(), CV_32FC1, map1, map2);
    // do actual undistortion and save in undistorted image file
    cv::remap(image, undistImg, map1, map2,cv::INTER_LINEAR); // interpolation type

    //cv::namedWindow("result");
    //cv::imshow("output", undistImg);

}


void Camera::readCalibImages()
{
    inputFiles.push_back("/Users/shreya/Desktop/calib/calib_1.jpg");
    inputFiles.push_back("/Users/shreya/Desktop/calib/calib_2.jpg");
    inputFiles.push_back("/Users/shreya/Desktop/calib/calib_3.jpg");
    inputFiles.push_back("/Users/shreya/Desktop/calib/calib_4.jpg");
}
