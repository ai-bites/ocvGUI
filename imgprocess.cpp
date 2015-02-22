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




