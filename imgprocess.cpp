#include "imgprocess.h"

using namespace std;
using namespace cv;

ImgProcess::ImgProcess()
{
}

void ImgProcess::doMorphOper(QString name)
{
    if (name.toStdString() == "Open")
    {
        cv::Mat elem(5,5,CV_8U,cv::Scalar(1));
        cv::morphologyEx(this->image,this->opImage,cv::MORPH_OPEN,elem);
    }
    else if (name.toStdString() == "Close")
    {
        cv::Mat elem(5,5,CV_8U,cv::Scalar(1));
        cv::morphologyEx(this->image,this->opImage,cv::MORPH_CLOSE,elem);
    }
    else if (name.toStdString() == "Dilate")
    {
        cv::dilate(this->image,this->opImage,cv::Mat());
    }
    else if (name.toStdString() == "Erode")
    {
        cv::erode(this->image,this->opImage,cv::Mat());
    }
    else
    {
        cout << "not a standard morphological operation!" << endl;
        return;
    }
}




