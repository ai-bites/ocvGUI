#include "imgprocess.h"

using namespace std;

ImgProcess::ImgProcess()
{
}


void ImgProcess::doMorphOper(QString name, int l=5, int b=5)
{
    if (name.toStdString() == "Open")
    {
        //cout << "in do morph open" << "l is: " << l << endl;
        cv::Mat elem(l,b,CV_8U,cv::Scalar(1));
        cv::morphologyEx(this->grayImage,this->opImage,cv::MORPH_OPEN,elem);
    }
    else if (name.toStdString() == "Close")
    {
        cv::Mat elem(l,b,CV_8U,cv::Scalar(1));
        cv::morphologyEx(this->grayImage,this->opImage,cv::MORPH_CLOSE,elem);
    }
    else if (name.toStdString() == "Dilate")
    {
        cv::dilate(this->grayImage,this->opImage,cv::Mat());
    }
    else if (name.toStdString() == "Erode")
    {
        cv::erode(this->grayImage,this->opImage,cv::Mat());
    }
    else
    {
        cout << "not a standard morphological operation!" << endl;
        return;
    }

}


void ImgProcess::addNoise(QString type, int whiteVal, int blackVal, double mean, double var)
{

    if (type.toStdString() == "Salt and Pepper")
    {
        //cout << "adding snp noise " << endl;
        Mat snpNoise = Mat::zeros(this->grayImage.rows, this->grayImage.cols,CV_8U);
        randu(snpNoise,0,255);

        Mat black = snpNoise < (255 - whiteVal);
        Mat white = snpNoise > (255 - blackVal);

        snpNoise.setTo(0,black);
        snpNoise.setTo(255,white);

        this->opImage = this->grayImage + snpNoise.clone();
    }
    else if (type.toStdString() == "Gaussian")
    {
        Mat gaussImg = Mat::zeros(this->grayImage.rows, this->grayImage.cols,CV_8U);
        randn(gaussImg,mean,var);
        this->opImage = gaussImg.clone();
        this->opImage = this->grayImage.clone() + gaussImg;
    }
    return;
}


void ImgProcess::addLogo(float alpha, int h, int l)
{
    cv::Rect roi = cv::Rect(h,l, logo.cols, logo.rows);
    this->opImage = this->grayImage.clone();
    cv::Mat imageROI = opImage(roi);
    cout << "image roi size: " << imageROI.cols << " " << imageROI.channels() << endl;
    Mat temp;
    cout << "logo size is: " << logo.cols << logo.channels() << endl;
    cvtColor(logo, temp, CV_RGB2GRAY);
    //cv::addWeighted(imageROI,1.0,logo,0.3,0.,imageROI);
    addWeighted(imageROI,alpha,temp,1-alpha,0.,imageROI);
    logo.copyTo(imageROI,temp);
    cout << "image roi size: " << imageROI.cols << " " << imageROI.rows << endl;
}


void ImgProcess::toColourSpace(int idx)
{
    if (idx == 1)
    {
        cvtColor(image, opImage, CV_RGB2GRAY);
    }
    if (idx == 2)
    {
        cvtColor(image, opImage, CV_RGB2Lab);
    }
    if (idx == 3)
    {
        cvtColor(image, opImage, CV_RGB2HSV);
    }
    if (idx == 4)
    {
        cvtColor(image, opImage, CV_RGB2XYZ);
    }
    if (idx == 5)
    {
        cvtColor(image, opImage, CV_RGB2Luv);
    }
    if (idx == 6)
    {
        cvtColor(image, opImage, CV_RGB2HLS);
    }
    cout << "done with conversion" << endl;
}


void ImgProcess::doBlur(int idx, int kernelL, int kernelH,
                        double sigmaX, double sigmaY, int medianKernel)
{
    // normalized blur
    if (idx == 1)
    {
        blur(this->grayImage, this->opImage, Size(kernelL,kernelH));
    }
    // gaussian blur
    if (idx == 2)
    {
        GaussianBlur(this->grayImage, this->opImage, Size(kernelL, kernelH),sigmaX, sigmaY);
    }
    // median blur
    if (idx == 4)
    {
        medianBlur(this->grayImage, this->opImage, medianKernel);
    }
}

void ImgProcess::doSobelAndLapOper(int currentIdx, bool applyBlur,
                       int kernel, int dx, int dy, double dxWeight,
                       int delta,int scale)
{
    Mat xGrad, yGrad, absXGrad, absYGrad, temp, grayTemp;

    if (applyBlur == true)
    {
        GaussianBlur( this->image, temp, Size(3,3), 0, 0, BORDER_DEFAULT);
        if (temp.channels() == 3)
        {
            cvtColor(temp, grayTemp, CV_RGB2GRAY);
        }
    }
    else
    {
        if (this->image.channels() == 3)
        {
            cvtColor(this->image, grayTemp, CV_RGB2GRAY);
        }
        else
            grayTemp = this->grayImage.clone();
    }

    if (currentIdx == 1) // sobel
    {
        Sobel(grayTemp, xGrad, CV_16S, dx, 0, kernel, scale, delta,BORDER_DEFAULT );
        Sobel(grayTemp, yGrad, CV_16S, 0, dy, kernel, scale, delta, BORDER_DEFAULT );
        convertScaleAbs(xGrad, absXGrad);
        convertScaleAbs(yGrad, absYGrad);
        // finally add gradients to compute the final image
        addWeighted(absXGrad, dxWeight, absYGrad, 1-dxWeight, 0, this->opImage);
    }
    else // laplacian
    {
        Laplacian(grayTemp, this->opImage, CV_16S, kernel, scale, delta, BORDER_DEFAULT);
        convertScaleAbs(this->opImage, this->opImage);
    }

}


void ImgProcess::doCannyOper(int kernel, int threshold, bool applyBlur, bool isL2Grad)
{
    Mat temp, grayTemp, mask;
    grayTemp = this->grayImage.clone();
    if (applyBlur == true)
    {
        GaussianBlur(this->image, temp, Size(3,3), 0,0, BORDER_DEFAULT);
        cvtColor(temp, grayTemp, CV_RGB2GRAY);
    }
    // make a mask from canny operation
    Canny(grayTemp, mask, threshold, 3*threshold, kernel, isL2Grad);
    this->opImage = Scalar::all(0);
    // use mask to copy over input to result
    grayTemp.copyTo(this->opImage, mask);
}


void ImgProcess::doHoughLineTransform(
        int lineMethodIdx, int lineThreshold, int lineRhoRes, int lineThetaRes, bool lineEdgeDetect)
{
    Mat temp;

    if (lineEdgeDetect == true)
        Canny(this->grayImage, temp, 50, 150, 3);
    else
        temp = this->grayImage.clone();

    temp.copyTo(this->opImage);
    //cvtColor(temp, this->opImage, CV_GRAY2RGB);

    if (lineMethodIdx == 1) // standard method
    {
        vector<Vec2f> lines;
        HoughLines(temp, lines, lineRhoRes,double(lineThetaRes*(CV_PI/180)), lineThreshold);
        for( int i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            line( this->opImage, pt1, pt2, Scalar(0,0,255), 2, CV_AA);
        }
    }
    else // prob method
    {
        vector<Vec4i> lines;
        HoughLinesP(temp, lines, lineRhoRes, double(lineThetaRes*(CV_PI/180)), lineThreshold);
        for( int i = 0; i < lines.size(); i++ )
        {
            Vec4i l = lines[i];
            line( this->opImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 2, CV_AA);
        }
    }
}


void ImgProcess::doHoughCircleTransform(
        double cannyThresh, double detectThresh, int minRad, int maxRad, bool applyBlur)
{
    Mat temp;
    vector<Vec3f> circles;

    if (applyBlur == true)
        GaussianBlur(this->grayImage, temp, Size(9, 9), 2, 2);
    else
        temp = this->grayImage.clone();
    temp.copyTo(this->opImage);

    HoughCircles(temp, circles, CV_HOUGH_GRADIENT, 1,
                 temp.rows/8, cannyThresh, detectThresh, minRad, maxRad);

    for( int i = 0; i < circles.size(); i++ )
    {
       Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
       int radius = cvRound(circles[i][2]);
       // circle center
       circle( this->opImage, center, 3, Scalar(0,255,0), -1, 8, 0 );
       // circle outline
       circle( this->opImage, center, radius, Scalar(0,0,255), 3, 8, 0 );
     }

}


void ImgProcess::doHistogram(int numBins, bool showHistEqImg)
{
    if (showHistEqImg)
    {
        cv::equalizeHist(this->grayImage, this->opImage);
        return;
    }

    // init paramters needed later
    //float hranges[2];
    int histSize[0];
    cv::MatND hist;

    //hranges[0] = 0.0;
    //hranges[1] = 255.0;
    float hranges[] = {0.0, 255.0};
    const float* ranges[] = {hranges};
    histSize[0] = numBins;
    int channels[] = {0};

    cv::calcHist(&(this->grayImage),
                1,           // histogram from 1 image only
                channels,  // the channel used
                cv::Mat(), // no mask is used
                hist,        // the resulting histogram
                1,           // it is a 1D histogram
                histSize,  // number of bins
                ranges     // pixel value range
    );

    // now create bar graphs for dispaying histogram data, hist
    double maxVal=0;
    double minVal=0;

    cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);
    cv::Mat histImg(histSize[0], histSize[0], CV_8U, cv::Scalar(255));
    int hpt = static_cast<int>(0.9*histSize[0]);

    for (int h = 0; h < histSize[0]; h++)
    {
        float binVal = hist.at<float>(h);
        int intensity = static_cast<int>(binVal*hpt/maxVal);
        cv::line(histImg,cv::Point(h,histSize[0]), cv::Point(h, histSize[0]-intensity), cv::Scalar::all(0));
    }

    cv::resize(histImg, this->opImage, this->grayImage.size());

}


void ImgProcess::drawContours(int edgeThresh, bool doBlur, int methodIdx)
{
    Mat temp;
    RNG rng(12345);

    if (doBlur)
        blur(this->grayImage, temp, Size(3,3));
    else
        temp = this->grayImage.clone();

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    // Detect edges using canny and find the contours
    Canny( temp, temp, edgeThresh, edgeThresh*2, 3 );

    if (methodIdx == 0)
    {
        return;
    }
    if (methodIdx == 1)
    {
        cv::findContours( temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    }
    if (methodIdx == 2)
    {
        cv::findContours( temp, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    }
    if (methodIdx == 3)
    {
        cv::findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    }
    if (methodIdx == 4)
    {
        cv::findContours( temp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    }

    // Draw contours
    Mat drawing = Mat::zeros( this->grayImage.size(), CV_8UC1 );
    cout << "doing drawing" << endl;
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        cv::drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
    this->opImage = drawing.clone();

}

void ImgProcess::doHarrisCorner(int blockSize, int aperture,double kValue, int threshold)
{
    Mat temp, tempNorm, tempNormScaled;
    cornerHarris(this->grayImage,temp, blockSize, aperture, kValue);
    //cout << "sample corner op: " << temp.at<int>(10,10) << endl;

    normalize(temp, tempNorm, 0, 255, NORM_MINMAX, CV_32FC1);
    //cout << "normalized sample: " << tempNorm.at<int>(10,10) << endl;
    convertScaleAbs(tempNorm, tempNormScaled);
    //cout << "converted scaled: " << tempNormScaled.at<int>(10,10) << endl;

    for (int i = 0; i < tempNorm.rows; i++)
    {
        for (int j = 0; j < tempNorm.cols; j++)
        {
            if (int(tempNorm.at<float>(i,j)) > threshold)
                circle(tempNormScaled, Point(i,j), 5, Scalar(0),2,8,0);
        }
    }
    this->opImage = tempNormScaled.clone();

}


void ImgProcess::doFeatureExtract(int fastThresh, int methodIdx,
                                  double siftThresh, double siftLineSensthresh, double surfThresh)
{
    this->opImage = grayImage.clone();

    if (methodIdx == 1) // FAST
    {
        cv::FastFeatureDetector fast(fastThresh);
        fast.detect(this->grayImage, keypoints);
        // draw white coloured keypoints
        cv::drawKeypoints(grayImage,keypoints,this->opImage,
                          cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
    }
    if (methodIdx == 2) // SURF
    {
        cv::SurfFeatureDetector surf(surfThresh);
        surf.detect(this->grayImage, keypoints);
        cv::drawKeypoints(grayImage,keypoints,this->opImage,
                          cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
    }
    if (methodIdx == 3) // SIFT
    {
        cv::SiftFeatureDetector sift(siftThresh, siftLineSensthresh);
        sift.detect(this->grayImage, keypoints);
        cv::drawKeypoints(grayImage, keypoints,this->opImage,
                          cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
    }

}


void ImgProcess::computeFundMatrix(int methodIdx)
{
    Mat imgOne, imgTwo;
    std::vector<cv::KeyPoint> imgOneKeyPts, imgTwoKeyPts;
    std::vector<cv::Point2f> imgOneKpFlt, imgTwoKpFlt;
    cv::Mat fundMatrix;

    this->image = cv::imread("/Users/shreya/Documents/workspace/cpp/ocvGUI/images/fm_1.png");
    this->doFeatureExtract(0, 2, 0.0, 0.0, 200.);
    imgOneKeyPts = getKeyPoints();
    imgOne = this->image.clone();

    this->image = cv::imread("/Users/shreya/Documents/workspace/cpp/ocvGUI/images/fm_2.png");
    this->doFeatureExtract(0, 2, 0.0, 0.0, 200.);
    imgTwoKeyPts = this->getKeyPoints();
    imgTwo = this->image.clone();

    cout << "before conversion" << endl;
    // convert keypoints for computation
    cv::KeyPoint::convert(imgOneKpFlt, imgOneKeyPts);
    cv::KeyPoint::convert(imgTwoKpFlt, imgOneKeyPts);

    cout << "after conv" << endl;

    if (methodIdx == 1) // 7 point
    {
        fundMatrix = cv::findFundamentalMat(cv::Mat(imgOneKpFlt), cv::Mat(imgTwoKpFlt),CV_FM_7POINT);
    }
    if (methodIdx == 2) // 8 point
    {
        fundMatrix = cv::findFundamentalMat(imgOneKpFlt, imgTwoKpFlt, CV_FM_8POINT);
    }
    if (methodIdx == 3) // RANSAC
    {
        fundMatrix = cv::findFundamentalMat(imgOneKpFlt, imgTwoKpFlt,  FM_RANSAC);
    }
    //this->drawEpipolarLines(fundMatrix, cv::Mat(imgOneKpFlt), imgTwo);
}


void ImgProcess::doMatchImages(Mat firstImg, Mat secondImg, bool isShow)
{
    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 400;
    SurfFeatureDetector detector( minHessian );
    std::vector<KeyPoint> keyPtsOne, keyPtsTwo;

    detector.detect( firstImg, keyPtsOne );
    detector.detect( secondImg, keyPtsTwo );

    //-- Step 2: Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;

    Mat descOne, descTwo;

    extractor.compute( firstImg, keyPtsOne, descOne );
    extractor.compute( secondImg, keyPtsTwo, descTwo );

    std::vector< DMatch > goodMatches = doFlannMatching(descOne, descTwo);

    Mat finalMatches;
    drawMatches(firstImg, keyPtsOne, secondImg, keyPtsTwo,
                 goodMatches, finalMatches, Scalar::all(-1), Scalar::all(-1),
                 vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    Size sz = firstImg.size();
    resize(finalMatches, finalMatches, sz);
    imshow( "Good Matches", finalMatches );
}

std::vector< DMatch > ImgProcess::doFlannMatching(Mat descOne, Mat descTwo)
{
    // Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descOne, descTwo, matches );

    double max_dist = 0;
    double min_dist = 100;

    // Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descOne.rows; i++ )
    {
        double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }
    std::vector< DMatch > goodMatches;

    for( int i = 0; i < descOne.rows; i++ )
    {
        if( matches[i].distance <= max(2*min_dist, 0.02) )
        {
            goodMatches.push_back( matches[i]);
        }
    }
    return goodMatches;
}

void ImgProcess::computeHomography()
{
    cout << "computing homography" << endl;
    Mat imgOne, imgTwo;
    std::vector<cv::KeyPoint> imgOneKeyPts, imgTwoKeyPts;
    std::vector<cv::Point2f> imgOneKpFlt, imgTwoKpFlt;
    cv::Mat fundMatrix;

    std::vector<KeyPoint> keyPtsOne, keyPtsTwo;
    std::vector<cv::DMatch> matches;

    this->image = cv::imread("/Users/shreya/Desktop/test_images/table_1.jpg");
    this->doFeatureExtract(0, 2, 0.0, 0.0, 200.);
    imgOneKeyPts = getKeyPoints();
    imgOne = this->image.clone();

    this->opImage = cv::imread("/Users/shreya/Desktop/test_images/table_2.jpg");
    this->doFeatureExtract(0, 2, 0.0, 0.0, 200.);
    imgTwoKeyPts = this->getKeyPoints();
    imgTwo = this->opImage.clone();



    cout << "got images" << endl;
    int minHessian = 400;
    SurfFeatureDetector detector( minHessian );

    detector.detect( this->image, keyPtsOne );
    detector.detect( this->opImage, keyPtsTwo );

    // Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;

    Mat descOne, descTwo;

    extractor.compute( this->image, keyPtsOne, descOne );
    extractor.compute( this->opImage, keyPtsTwo, descTwo );

    cout << "got desc" << endl;

    std::vector< DMatch > goodMatches = doFlannMatching(descOne, descTwo);
    cout << "got matches" << goodMatches.size() << endl;
    cout << "keypoints 1 count" << imgOneKeyPts.size() << endl;
    cout << "kpts 2 count" << imgTwoKeyPts.size() << endl;


    // convert to points 2f vector to use for homography computation
    std::vector< cv::Point2f > points1, points2;
    cout << "after declaration" << endl;
    for( std::vector<cv::KeyPoint>::const_iterator it = imgOneKeyPts.begin(); it!= imgOneKeyPts.end();it++)
    {
        points1.push_back(it->pt);
    }
    for( std::vector<cv::KeyPoint>::const_iterator it = imgTwoKeyPts.begin(); it!= imgTwoKeyPts.end();it++)
    {
        points2.push_back(it->pt);
    }

    cout << "created points 1 and points 2" << endl;

    std::vector<uchar> inliers(points1.size(),0);
    cv::Mat homography = cv::findHomography(
            cv::Mat(points1),
            cv::Mat(points2), // corresponding points
            inliers,	// outputed inliers matches
            CV_RANSAC,	// RANSAC method
            1.);	    // max distance to reprojection point

    cout << "computed homography !" << endl;

    // Draw the inlier points
    std::vector<cv::Point2f>::const_iterator itPts = points1.begin();
    std::vector<uchar>::const_iterator itIn= inliers.begin();
    while (itPts != points1.end())
    {
        // draw a circle at each inlier location
        if (*itIn)
            cv::circle(this->opImage,*itPts,3,cv::Scalar(255,255,255),2);

        ++itPts;
        ++itIn;
    }

    itPts= points2.begin();
    itIn= inliers.begin();
    while (itPts!=points2.end())
    {
        // draw a circle at each inlier location
        if (*itIn) cv::circle(this->image,*itPts,3,cv::Scalar(255,255,255),2);

        ++itPts;
        ++itIn;
    }

    // Display the images with points
//    cv::namedWindow("Image 1 Homography Points", CV_WINDOW_NORMAL);
//    cv::resize(this->opImage, this->opImage, Size(400,400));
//    cv::imshow("Image 1 Homography Points",this->opImage);
//    cv::namedWindow("Image 2 Homography Points", CV_WINDOW_NORMAL);
//    cv::resize(this->image, this->image, Size(400,400));
//    cv::imshow("Image 2 Homography Points",this->image);

    cv::Mat result;
    warpPerspective(this->image,result,homography,
                    cv::Size(this->image.cols+this->opImage.cols,this->image.rows));
    cv::Mat half(result,cv::Rect(0,0,this->opImage.cols,this->opImage.rows));

    this->opImage.copyTo(half);
    cv::resize(result, result, Size(400,400));
    imshow( "Result", result );

    return;
}


void ImgProcess::drawEpipolarLines(Mat fundMatrix, Mat keyPoints, Mat image)
{
    // vector of lines
    std::vector<cv::Vec3f> lines;
    cv::computeCorrespondEpilines(keyPoints, 1,fundMatrix,lines);

//    for (vector<cv::Vec3f>::const_iterator it= lines.begin();it!=lines.end(); ++it)
//    {
//        //
//        cv::line(image, cv::Point(0,-(*it)[2]/(*it)[1]),
//                cv::Point(image.cols,-((*it)[2]+(*it)[0]*image.cols)/(*it)[1]),
//                cv::Scalar(255,255,255));
//    }

}

