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


void ImgProcess::addNoise(QString type)
{
    this->opImage = this->image;

    if (type.toStdString() == "SALT_AND_PEPPER")
    {
        //TODO: set n dynamically
        for (int k = 0; k < 100; k++)
        {
            int a = (QString::number(qrand() % ((200 + 1) - 10) + 10)).toInt();
            int b = (QString::number(qrand() % ((200 + 1) - 10) + 10)).toInt();
            int c = 10;
            int d = 100;

            if(this->opImage.channels() == 1)
            {
                // salt
                this->opImage.at<uchar>(a,b) = 255;
                // pepper
                this->opImage.at<uchar>(c,d) = 0;
            }
            else
            {
                // salt
                opImage.at<cv::Vec3b>(a,b)[0]= 255;
                opImage.at<cv::Vec3b>(a,b)[1]= 255;
                opImage.at<cv::Vec3b>(a,b)[2]= 255;
                // pepper
                opImage.at<cv::Vec3b>(c,d)[0]= 0;
                opImage.at<cv::Vec3b>(c,d)[1]= 0;
                opImage.at<cv::Vec3b>(c,d)[2]= 0;
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




