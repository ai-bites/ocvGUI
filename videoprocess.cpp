#include "videoprocess.h"


VideoProcess::VideoProcess()
{
    isVideoStopped = true;
    this->ip = new ImgProcess;
}


void VideoProcess::doMorphOper(QString name, int l=5, int h=5)
{
    QImage img;
    //l = 100; h = 100;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame;
        ip->doMorphOper(name, l, h);
        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}


void VideoProcess::displayOpVideo()
{
    QImage img;

    // Delay between each frame in ms
    int delay = 100;
    // for all frames in video
    while (!isVideoStopped)
    {
        cout << "DISPLAY Op VIDEO" << endl;
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        img = QImage((const unsigned char*)(destFrame.data),destFrame.cols,
                     destFrame.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(1);
    }
    // Close the video file.
    capture.release();
}

