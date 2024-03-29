#include "imgprocess.h"

using namespace std;

ImgProcess::ImgProcess()
{
    this->isCascadeEnabled = false;
}


Mat ImgProcess::getImageToProcess()
{
    Mat toProcess;

    if (this->isCascadeEnabled)
    {
        if (this->opImage.data)
            toProcess = this->opImage.clone();
        else
            toProcess = this->grayImage.clone();
    }
    else
        toProcess = this->grayImage.clone();
    return toProcess;
}

void ImgProcess::doMorphOper(QString name, int l=5, int b=5)
{

    if (name.toStdString() == "Open")
    {
        cv::Mat elem(l,b,CV_8U,cv::Scalar(1));
        cv::morphologyEx(this->getImageToProcess(),this->opImage,cv::MORPH_OPEN,elem);
    }
    else if (name.toStdString() == "Close")
    {
        cv::Mat elem(l,b,CV_8U,cv::Scalar(1));
        cv::morphologyEx(this->getImageToProcess(),this->opImage,cv::MORPH_CLOSE,elem);
    }
    else if (name.toStdString() == "Dilate")
    {
        cv::dilate(this->getImageToProcess(),this->opImage,cv::Mat());
    }
    else if (name.toStdString() == "Erode")
    {
        cv::erode(this->getImageToProcess(),this->opImage,cv::Mat());
    }

}


void ImgProcess::addNoise(QString type, int whiteVal, int blackVal, double mean, double var)
{

    if (type.toStdString() == "Salt and Pepper")
    {
        Mat image = this->getImageToProcess();

        for (int k = 0; k < whiteVal; k++)
        {
            int i = rand() % image.cols;
            int j = rand() % image.rows;

            if (image.channels() == 1)
            {
                image.at<uchar>(j, i)= 255;
            }
            else if (image.channels() == 3)
            {
                image.at<cv::Vec3b>(j, i)[0]= 255;
                image.at<cv::Vec3b>(j, i)[1]= 255;
                image.at<cv::Vec3b>(j, i)[2]= 255;
            }
        }
        for (int k = 0; k < blackVal; k++)
        {
            int i = rand() % image.cols;
            int j = rand() % image.rows;

            if (image.channels() == 1)
            {
                image.at<uchar>(j, i)= 0;
            }
            else if (image.channels() == 3)
            {
                image.at<cv::Vec3b>(j, i)[0]= 0;
                image.at<cv::Vec3b>(j, i)[1]= 0;
                image.at<cv::Vec3b>(j, i)[2]= 0;
            }
        }

        this->opImage = image.clone();
    }
    else if (type.toStdString() == "Gaussian")
    {
        Mat gaussImg = Mat::zeros(this->grayImage.rows, this->grayImage.cols,CV_8U);
        randn(gaussImg,mean,var);
        this->opImage = gaussImg.clone();
        this->opImage = this->getImageToProcess().clone() + gaussImg;
    }
    return;
}


void ImgProcess::addLogo(float alpha, int h, int l)
{
    cv::Rect roi = cv::Rect(h,l, logo.cols, logo.rows);
    // if op image does not exists, create it
    //this->opImage = this->grayImage.clone();
    this->opImage = getImageToProcess();

    cv::Mat imageROI = opImage(roi);

    Mat temp;
    cvtColor(logo, temp, CV_RGB2GRAY);
    //cv::addWeighted(imageROI,1.0,logo,0.3,0.,imageROI);
    addWeighted(imageROI,alpha,temp,1-alpha,0.,imageROI);
    logo.copyTo(imageROI,temp);
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

}


void ImgProcess::doBlur(int idx, int kernelL, int kernelH,
                        double sigmaX, double sigmaY, int medianKernel)
{
    // normalized blur
    if (idx == 1)
    {
        blur(getImageToProcess(), this->opImage, Size(kernelL,kernelH));
    }
    // gaussian blur
    if (idx == 2)
    {
        GaussianBlur(getImageToProcess(), this->opImage, Size(kernelL, kernelH),sigmaX, sigmaY);
    }
    // median blur
    if (idx == 4)
    {
        medianBlur(getImageToProcess(), this->opImage, medianKernel);
    }
}


void ImgProcess::doBrightContrast(double alpha, int beta)
{
    Mat image = getImageToProcess();

    if (image.channels() == 3)
    {
        for( int y = 0; y < image.rows; y++ )
        {
            for( int x = 0; x < image.cols; x++ )
            {
                for( int c = 0; c < 3; c++ )
                {
                    this->opImage.at<Vec3b>(y,x)[c] =
                    saturate_cast<uchar>( alpha*(image.at<Vec3b>(y,x)[c] ) + beta );
                }
            }
        }
    }
    else
        this->opImage = alpha*(image + beta );
}

void ImgProcess::doAdaptiveThreshold(int blockSize, double maxVal,
                                     int methodIdx, int threshTypeIdx, double constant)
{
    cv::Mat temp = getImageToProcess();
    if (temp.channels() == 3)
        cvtColor(temp, temp, CV_RGB2GRAY);

    if (methodIdx == 1)
    {
        if ( threshTypeIdx == 1)
            cv::adaptiveThreshold( temp, this->opImage,
                             255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,blockSize, constant );
        if (threshTypeIdx == 2)
            cv::adaptiveThreshold( temp, this->opImage,
                             255,CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,blockSize, constant );
    }
    if (methodIdx == 2)
    {
        if (threshTypeIdx == 1)
            cv::adaptiveThreshold( temp, this->opImage,
                             255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV,blockSize, constant );
        if (threshTypeIdx == 2)
            cv::adaptiveThreshold( temp, this->opImage,
                             255,CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV,blockSize, constant );
    }

}

void ImgProcess::doSobelAndLapOper(int currentIdx, bool applyBlur,
                       int kernel, int dx, int dy, double dxWeight,
                       int delta,int scale)
{
    Mat xGrad, yGrad, absXGrad, absYGrad, temp, grayTemp;

    if (applyBlur == true)
    {
        GaussianBlur( getImageToProcess(), temp, Size(3,3), 0, 0, BORDER_DEFAULT);
        if (temp.channels() == 3)
        {
            cvtColor(temp, grayTemp, CV_RGB2GRAY);
        }
        else
            grayTemp = temp.clone();
    }
    else
    {
        if (getImageToProcess().channels() == 3)
        {
            cvtColor(getImageToProcess(), grayTemp, CV_RGB2GRAY);
        }
        else
            grayTemp = getImageToProcess().clone();
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


void ImgProcess::doCannyOper(int kernel, int threshold,
                             bool applyBlur, bool isL2Grad, int maxThreshold)
{
    // kernel has to be odd number
    if (kernel%2 == 0) return;

    Mat temp, grayTemp, mask;
    //grayTemp = this->grayImage.clone();
    grayTemp = getImageToProcess();
    if (applyBlur == true)
    {
        GaussianBlur(grayTemp, temp, Size(3,3), 0,0, BORDER_DEFAULT);
        if (temp.channels() == 3)
            cvtColor(temp, grayTemp, CV_RGB2GRAY);
        else
            grayTemp = temp.clone();
    }
    // make a mask from canny operation
    Canny(grayTemp, mask, threshold, maxThreshold, kernel, isL2Grad);
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
        GaussianBlur(getImageToProcess(), temp, Size(9, 9), 2, 2);
    else
        temp = getImageToProcess();
    temp.copyTo(this->opImage);

    if (temp.channels() == 3)
        cvtColor(temp, temp, CV_RGB2GRAY);

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

    Mat temp = this->image;

//    if (temp.channels() == 3)
//        cvtColor(temp, temp, CV_RGB2GRAY);

    cv::calcHist(&temp,
                1,           // histogram from 1 image only
                channels,  // the channel used
                cv::Mat(), // no mask is used
                hist,        // the resulting histogram
                1,           // it is a 1D histogram
                histSize,  // number of bins
                ranges     // pixel value range
    );

    /// HACK BEGINS
    vector<Mat> bgr_planes;
    split( temp, bgr_planes );
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    Mat b_hist, g_hist, r_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &numBins, &histRange, true, true );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &numBins, &histRange, true, true );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &numBins, &histRange, true, true );

    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/numBins );

    Mat hist_Image( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, hist_Image.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, hist_Image.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, hist_Image.rows, NORM_MINMAX, -1, Mat() );

    /// Draw for each channel
    for( int i = 1; i < numBins; i++ )
    {
      line( hist_Image, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      line( hist_Image, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( hist_Image, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
    }

    /// HACK ENDS

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

    cv::resize(hist_Image, this->opImage, this->grayImage.size());

}


void ImgProcess::drawContours(int edgeThresh, bool doBlur,
                              int methodIdx, bool showRect, bool showCircle)
{
    Mat temp;
    RNG rng(12345);

    if (doBlur)
        blur(getImageToProcess(), temp, Size(3,3));
    else
        temp = getImageToProcess().clone();

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    if (temp.channels() == 3)
        cvtColor(temp, temp, CV_RGB2GRAY);

    // Detect edges using canny and find the contours
    Canny( temp, temp, edgeThresh, edgeThresh*2, 3 );

    if (methodIdx == 0)
    {
        return;
    }
    if (methodIdx == 1)
    {
        cv::findContours( temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
    }
    if (methodIdx == 2)
    {
        cv::findContours( temp, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0));
    }
    if (methodIdx == 3)
    {
        cv::findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));
    }
    if (methodIdx == 4)
    {
        cv::findContours( temp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
    }

    // Bounding boxes: Approximate contours to polygons + get bounding rects and circles
    vector<vector <Point> > contoursPoly( contours.size() );
    vector<Rect> boundRects( contours.size() );
    vector<Point2f> center( contours.size() );
    vector<float> radius( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contoursPoly[i], 3, true );
        boundRects[i] = boundingRect( Mat(contoursPoly[i]) );
        minEnclosingCircle( (Mat)contoursPoly[i], center[i], radius[i] );
    }

    // Draw contours
    Mat drawing = Mat::zeros( this->grayImage.size(), CV_8UC1 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

        if (showRect == 0 && showCircle == 0)
            cv::drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );

        // now draw bounding boxes or shapes if needed
        if (showRect)
        {
            cv::drawContours( drawing, contoursPoly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
            boundRects[i] = boundingRect( Mat(contoursPoly[i]) );
            rectangle( drawing, boundRects[i].tl(), boundRects[i].br(), color, 2, 8, 0 );

        }
        if (showCircle)
        {
            cv::drawContours( drawing, contoursPoly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
            minEnclosingCircle( (Mat)contoursPoly[i], center[i], radius[i] );
            circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
        }
    }
    this->opImage = drawing.clone();

}

void ImgProcess::doHarrisCorner(int blockSize, int aperture,double kValue, int threshold)
{
    Mat temp, tempNorm, tempNormScaled;

    temp = getImageToProcess();
    if (temp.channels() == 3)
        cvtColor(temp, temp, CV_RGB2GRAY);

    cornerHarris(temp,temp, blockSize, aperture, kValue);

    normalize(temp, tempNorm, 0, 255, NORM_MINMAX, CV_8U);
    convertScaleAbs(tempNorm, tempNormScaled);

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
    this->opImage = getImageToProcess();
    Mat temp;

    if (getImageToProcess().channels() == 3)
        cvtColor(getImageToProcess(),temp, CV_RGB2GRAY);
    else
        temp = getImageToProcess().clone();

    if (methodIdx == 1) // FAST
    {
        cv::FastFeatureDetector fast(fastThresh);
        fast.detect(temp, keypoints);
        // draw white coloured keypoints
        cv::drawKeypoints(temp,keypoints,this->opImage,
                          cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
    }
    if (methodIdx == 2) // SURF
    {
        cv::SurfFeatureDetector surf(surfThresh);
        surf.detect(temp, keypoints);
        cv::drawKeypoints(temp,keypoints,this->opImage,
                          cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
    }
    if (methodIdx == 3) // SIFT
    {
        cv::SiftFeatureDetector sift(siftThresh, siftLineSensthresh);
        sift.detect(temp, keypoints);
        cv::drawKeypoints(temp, keypoints,this->opImage,
                          cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
    }

}


Mat ImgProcess::computeFundMatrix(int methodIdx)
{
    cv::Mat fundMatrix;
    cv::Mat image;

    cvtColor(this->firstImg,image, CV_RGB2GRAY);
    cv::FastFeatureDetector fastOne(0);
    fastOne.detect(image, keypoints);

    imgOneKeyPts = this->getKeyPoints();
    cvtColor(this->firstImg,image, CV_RGB2GRAY);
    cv::FastFeatureDetector fastTwo(0);
    fastTwo.detect(image, keypoints);
    imgTwoKeyPts = this->getKeyPoints();

    // convert keypoints for computation
    cv::KeyPoint::convert( imgOneKeyPts, imgOneKpFlt);
    cv::KeyPoint::convert(imgTwoKeyPts, imgTwoKpFlt);

    if (methodIdx == 1) // 7 point
    {
        fundMatrix = cv::findFundamentalMat(cv::Mat(imgOneKpFlt), cv::Mat(imgTwoKpFlt),CV_FM_7POINT);
    }
    if (methodIdx == 2) // 8 point
    {
        fundMatrix = cv::findFundamentalMat(Mat(imgOneKpFlt), Mat(imgTwoKpFlt), CV_FM_8POINT);
    }
    if (methodIdx == 3) // RANSAC
    {
        fundMatrix = cv::findFundamentalMat(Mat(imgOneKpFlt), Mat(imgTwoKpFlt),  FM_RANSAC);
    }
    return fundMatrix;

}


void ImgProcess::doMatchImages(Mat firstImg, Mat secondImg, bool isShow)
{
    // we are not going to show matches.
    // we are just loading images
    if (isShow == false) return;

    // detect the keypoints using SURF Detector
    int minHessian = 400;
    SurfFeatureDetector detector( minHessian );
    imgOneKeyPts, imgTwoKeyPts;

    detector.detect( firstImg, imgOneKeyPts );
    detector.detect( secondImg, imgTwoKeyPts );

    // calculate descriptors
    SurfDescriptorExtractor extractor;

    Mat descOne, descTwo;

    extractor.compute( firstImg, imgOneKeyPts, descOne );
    extractor.compute( secondImg, imgTwoKeyPts, descTwo );

    std::vector< DMatch > goodMatches = doFlannMatching(descOne, descTwo);

    Mat finalMatches, resMatches;
    drawMatches(firstImg, imgOneKeyPts, secondImg, imgTwoKeyPts,
                 goodMatches, finalMatches, Scalar::all(-1), Scalar::all(-1),
                 vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    Size sz = firstImg.size();
    resize(finalMatches, resMatches, Size(600,400));
    imshow( "Good Matches", resMatches );

    cv::waitKey(0);
    cvDestroyWindow("Good Matches");
}


void ImgProcess::doStitchImages(Mat firstImg, Mat secondImg, bool isShow)
{
    if (isShow == false) return;

    vector< Mat > vImg;
    Mat rImg;

    vImg.push_back(this->firstImg);
    vImg.push_back(this->secondImg);

    Stitcher stitcher = Stitcher::createDefault();
    Stitcher::Status status = stitcher.stitch(vImg, rImg);

    imshow("Stitched Image", rImg);
    waitKey(0);
    destroyWindow("Stitched Image");

    /*
     * This is the long version with computing homography
     * by ourselves instead of calling the above function
     *
    double confLevel = 1.5;
    double minDist = 5.5;
    double epiRatio = 1.5;
    int minHess = 100;
    // Prepare the matcher
    RobustMatcher rmatcher;
    rmatcher.setConfidenceLevel(confLevel);
    rmatcher.setMinDistanceToEpipolar(minDist);
    rmatcher.setRatio(epiRatio);
    cv::Ptr<cv::FeatureDetector> pfd = new cv::SurfFeatureDetector(minHess);
    rmatcher.setFeatureDetector(pfd);

    // Match the two images
    cv::Mat fundMatrix = rmatcher.match(this->firstImg, this->secondImg,
                                         this->goodMatches, this->imgOneKeyPts, this->imgTwoKeyPts, 2);

    // convert keypoints into points 2f
   for (std::vector<cv::DMatch>::const_iterator
      it = goodMatches.begin(); it!= goodMatches.end(); ++it)
   {
          // Get the position of left keypoints
          float x = imgOneKeyPts[it->queryIdx].pt.x;
          float y = imgOneKeyPts[it->queryIdx].pt.y;
          imgOneKpFlt.push_back(cv::Point2f(x,y));
          cv::circle(this->secondImg,cv::Point(x,y),3,cv::Scalar(255,255,255),3);

          // Get the position of right keypoints
          x = imgTwoKeyPts[it->trainIdx].pt.x;
          y = imgTwoKeyPts[it->trainIdx].pt.y;
          cv::circle(this->firstImg,cv::Point(x,y),3,cv::Scalar(255,255,255),3);
          imgTwoKpFlt.push_back(cv::Point2f(x,y));
   }


    std::vector<uchar> inliers(imgOneKpFlt.size(),0);
    cv::Mat homography = cv::findHomography(
            imgOneKpFlt,
            imgTwoKpFlt, // corresponding points
            inliers,	// outputed inliers matches
            CV_RANSAC,	// RANSAC method
            1.);	    // max distance to reprojection point

    // Draw the inlier points
    std::vector< cv::Point2f >::const_iterator itPts = imgOneKpFlt.begin();
    std::vector< uchar >::const_iterator itIn = inliers.begin();
    while (itPts != imgOneKpFlt.end())
    {
        // draw a circle at each inlier location
        if (*itIn)
            cv::circle(this->secondImg,*itPts,3,cv::Scalar(255,255,255),2);

        ++itPts;
        ++itIn;
    }

    itPts = imgTwoKpFlt.begin();
    itIn= inliers.begin();
    while (itPts!=imgTwoKpFlt.end())
    {
        // draw a circle at each inlier location
        if (*itIn) cv::circle(this->firstImg,*itPts,3,cv::Scalar(255,255,255),2);

        ++itPts;
        ++itIn;
    }

    cv::Mat result;
    warpPerspective(this->firstImg,result,homography,
                    cv::Size(this->firstImg.cols+this->secondImg.cols,this->firstImg.rows));
    cv::Mat half(result,cv::Rect(0,0,this->secondImg.cols,this->secondImg.rows));

    waitKey(0);
    destroyWindow("Result");
    */

}

void ImgProcess::doEpipolarLines( bool isShow, string toDisplay, int fMatMethodIdx,
                                  bool showInFirst,bool showInSec,double confLevel,
                                  double epiRatio,double minDist,int minHess)
{
    // no point showing lines when we don't have to
    if (isShow == false) return;
    if (showInFirst == false && showInSec == false) return;

    // clear the lines and points
    this->imgOneKeyPts.clear();
    this->imgTwoKeyPts.clear();
    this->goodMatches.clear();
    this->imgOneKpFlt.clear();
    this->imgTwoKpFlt.clear();
    cv::Mat secImgToDraw = this->secondImg.clone();
    cv::Mat firstImgToDraw = this->firstImg.clone();

     // Prepare the matcher
     RobustMatcher rmatcher;
     rmatcher.setConfidenceLevel(confLevel);
     rmatcher.setMinDistanceToEpipolar(minDist);
     rmatcher.setRatio(epiRatio);
     cv::Ptr<cv::FeatureDetector> pfd = new cv::SurfFeatureDetector(minHess);
     rmatcher.setFeatureDetector(pfd);

     // Match the two images
     // std::vector<cv::DMatch> matches;

     cv::Mat fundMatrix = rmatcher.match(firstImgToDraw, secImgToDraw,
                                         this->goodMatches, this->imgOneKeyPts,
                                         this->imgTwoKeyPts, fMatMethodIdx);

     // convert keypoints into points 2f
    for (std::vector<cv::DMatch>::const_iterator
       it = goodMatches.begin(); it!= goodMatches.end(); ++it)
    {
           // Get the position of left keypoints
           float x = imgOneKeyPts[it->queryIdx].pt.x;
           float y = imgOneKeyPts[it->queryIdx].pt.y;
           imgOneKpFlt.push_back(cv::Point2f(x,y));
           cv::circle(secImgToDraw,cv::Point(x,y),3,cv::Scalar(255,255,255),3);

           // Get the position of right keypoints
           x = imgTwoKeyPts[it->trainIdx].pt.x;
           y = imgTwoKeyPts[it->trainIdx].pt.y;
           cv::circle(firstImgToDraw,cv::Point(x,y),3,cv::Scalar(255,255,255),3);
           imgTwoKpFlt.push_back(cv::Point2f(x,y));
    }

    // draw lines
    std::vector<cv::Vec3f> linesOnSecondImg, linesOnFirstImg;
    cv::computeCorrespondEpilines(Mat(imgOneKpFlt), 1,fundMatrix,linesOnSecondImg);
    cv::computeCorrespondEpilines(Mat(imgTwoKpFlt), 2,fundMatrix,linesOnFirstImg );

    if (showInSec)
    {
        for (vector<cv::Vec3f>::const_iterator it = linesOnSecondImg.begin();
             it != linesOnSecondImg.end(); ++it)
        {
            cv::line(secImgToDraw, cv::Point(0,-(*it)[2]/(*it)[1]),
                    cv::Point(secImgToDraw.cols,-((*it)[2]+(*it)[0]*secImgToDraw.cols)/(*it)[1]),
                    cv::Scalar(255,255,255));
        }
        this->opImage = secImgToDraw.clone();
    }
    else
    {
        this->opImage = this->secondImg.clone();
    }
    if (showInFirst)
    {
        for (vector<cv::Vec3f>::const_iterator it = linesOnFirstImg.begin();
             it != linesOnFirstImg.end(); ++it)
        {
            cv::line(firstImgToDraw, cv::Point(0,-(*it)[2]/(*it)[1]),
                    cv::Point(firstImgToDraw.cols,-((*it)[2]+(*it)[0]*firstImgToDraw.cols)/(*it)[1]),
                    cv::Scalar(255,255,255));
        }
        this->image = firstImgToDraw.clone();
    }
    else
    {
        this->opImage = this->firstImg.clone();
    }
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


//!
//! This function is currently not used anywhere.
//! It was written with the motivation to use in mosaicing.
//! But ignored when an OpenCV function was found for stitching.
//!
void ImgProcess::computeHomography()
{

    Mat imgOne, imgTwo;
    //std::vector<cv::Point2f> imgOneKpFlt, imgTwoKpFlt;
    cv::Mat fundMatrix;

    //std::vector<KeyPoint> keyPtsOne, keyPtsTwo;
    std::vector<cv::DMatch> matches;

    this->image = cv::imread("/Users/shreya/Desktop/test_images/tools_1.bmp");
    this->doFeatureExtract(0, 2, 2.0, 1.0, 200.);
    imgOneKeyPts = getKeyPoints();
    imgOne = this->image.clone();

    this->opImage = cv::imread("/Users/shreya/Desktop/test_images/tools_2.bmp");
    this->doFeatureExtract(0, 2, 2.0, 1.0, 200.);
    imgTwoKeyPts = this->getKeyPoints();
    imgTwo = this->opImage.clone();

    int minHessian = 400;
    SurfFeatureDetector detector( minHessian );

    detector.detect( this->image, imgOneKeyPts );
    detector.detect( this->opImage, imgTwoKeyPts );

    // Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;

    Mat descOne, descTwo;

    extractor.compute( this->image, imgOneKeyPts, descOne );
    extractor.compute( this->opImage, imgTwoKeyPts, descTwo );

    goodMatches = doFlannMatching(descOne, descTwo);

    // convert to points 2f vector to use for homography computation
    //std::vector< cv::Point2f > points1, points2;
    for( std::vector<cv::KeyPoint>::const_iterator it = imgOneKeyPts.begin(); it!= imgOneKeyPts.end();it++)
    {
        imgOneKpFlt.push_back(it->pt);
    }
    for( std::vector<cv::KeyPoint>::const_iterator it = imgTwoKeyPts.begin(); it!= imgTwoKeyPts.end();it++)
    {
        imgTwoKpFlt.push_back(it->pt);
    }

    std::vector<uchar> inliers(imgOneKpFlt.size(),0);
    cv::Mat homography = cv::findHomography(
            cv::Mat(imgOneKpFlt),
            cv::Mat(imgTwoKpFlt), // corresponding points
            inliers,	// outputed inliers matches
            CV_RANSAC,	// RANSAC method
            1.);	    // max distance to reprojection point

    // Draw the inlier points
    std::vector<cv::Point2f>::const_iterator itPts = imgOneKpFlt.begin();
    std::vector<uchar>::const_iterator itIn = inliers.begin();
    while (itPts != imgOneKpFlt.end())
    {
        // draw a circle at each inlier location
        if (*itIn)
            cv::circle(this->opImage,*itPts,3,cv::Scalar(255,255,255),2);

        ++itPts;
        ++itIn;
    }

    itPts = imgTwoKpFlt.begin();
    itIn= inliers.begin();
    while (itPts!=imgTwoKpFlt.end())
    {
        // draw a circle at each inlier location
        if (*itIn) cv::circle(this->image,*itPts,3,cv::Scalar(255,255,255),2);

        ++itPts;
        ++itIn;
    }

    cv::Mat result;
    warpPerspective(this->image,result,homography,
                    cv::Size(this->image.cols+this->opImage.cols,this->image.rows));
    cv::Mat half(result,cv::Rect(0,0,this->opImage.cols,this->opImage.rows));

    this->opImage.copyTo(half);
    cv::resize(result, result, Size(600,400));
    imshow( "Result", result );

    return;
}


//!
//! \brief ImgProcess::drawEpipolarLines Given the fundamental matrix
//!        and the keypoints in the one image, draws the image in the other image
//! \param fundMatrix - computed fundamental matrix
//! \param keyPoints  - Keypints in image 1
//! \param image      - Image 2 on which to plot
//!
void ImgProcess::drawEpipolarLines(Mat fundMatrix, Mat keyPoints, Mat image)
{
    // vector of lines
    std::vector<cv::Vec3f> lines;
    cv::computeCorrespondEpilines(keyPoints, 1,fundMatrix,lines);

    for (vector<cv::Vec3f>::const_iterator it= lines.begin();it!=lines.end(); ++it)
    {
        //
        cv::line(image, cv::Point(0,-(*it)[2]/(*it)[1]),
                cv::Point(image.cols,-((*it)[2]+(*it)[0]*image.cols)/(*it)[1]),
                cv::Scalar(255,255,255));
    }

}

