#include "imgprocess.h"

using namespace std;
using namespace cv;

ImgProcess::ImgProcess()
{
}


void ImgProcess::doMorphOper(QString name, int l=5, int b=5)
{
    if (name.toStdString() == "Open")
    {
        cout << "in do morph open" << "l is: " << l << endl;
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


void ImgProcess::addNoise(QString type, int val)
{
    this->opImage = this->grayImage;

    if (type.toStdString() == "SALT_AND_PEPPER")
    {
        for (int k = 0; k <= val; k++)
        {
            int a = (QString::number(qrand() % ((grayImage.rows + 1)) + 10)).toInt();
            int b = (QString::number(qrand() % ((grayImage.cols + 1)) + 10)).toInt();
            int c = (QString::number(qrand() % ((grayImage.rows + 1)) + 10)).toInt();
            int d = (QString::number(qrand() % ((grayImage.cols + 1)) + 10)).toInt();

            if(this->opImage.channels() == 1)
            {
                // salt
                this->opImage.at<uchar>(a,b) = 255;
                // pepper
                this->opImage.at<uchar>(c,d) = 0;
            }
        }
    }
    else if (type == "GAUSSIAN")
    {
        //TODO: add gaussian filtering
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
        cvtColor(temp, grayTemp, CV_RGB2GRAY);
    }
    else
    {
        temp = this->image.clone();
        grayTemp = this->image.clone();
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

    cvtColor(temp, this->opImage, CV_GRAY2BGR);

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
    HoughCircles(temp, circles, CV_HOUGH_GRADIENT, 1,
                 temp.rows/8, cannyThresh, detectThresh, minRad, maxRad);

    this->opImage = this->image.clone();
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
    std::vector<cv::KeyPoint> keypoints;
    this->opImage = image.clone();
    cout << "vals are: " << siftThresh << " " << siftLineSensthresh << endl;

    if (methodIdx == 1) // FAST
    {
        cv::FastFeatureDetector fast(fastThresh);
        fast.detect(this->image, keypoints);
        // draw white coloured keypoints
        cv::drawKeypoints(image,keypoints,this->opImage,
                          cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
    }
    if (methodIdx == 2) // SURF
    {
        SurfFeatureDetector surf(surfThresh);
        surf.detect(this->image, keypoints);
        cv::drawKeypoints(image,keypoints,this->opImage,
                          cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    }
    if (methodIdx == 3) // SIFT
    {
        cv::SiftFeatureDetector sift(siftThresh, siftLineSensthresh);
        sift.detect(this->image, keypoints);
        cv::drawKeypoints(image,keypoints,this->opImage,
                          cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    }

}

