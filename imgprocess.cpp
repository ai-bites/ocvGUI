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
        cv::morphologyEx(this->image,this->opImage,cv::MORPH_OPEN,elem);
    }
    else if (name.toStdString() == "Close")
    {
        cv::Mat elem(l,b,CV_8U,cv::Scalar(1));
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




