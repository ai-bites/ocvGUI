#include "videoprocess.h"


VideoProcess::VideoProcess()
{
    liveStreamStop = false;
}

void VideoProcess::doMorphOperation(QString name, int l=5, int h=5)
{
    QImage img;
}


void VideoProcess::displayOpVideo()
{
    QImage img;
    cv::Mat frame;
    cv::Mat destFrame;

    // Delay between each frame in ms
    int delay = 100;

    // for all frames in video
    while (!liveStreamStop)
    {
        cout << "in op window display" << endl;
        *capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        //ip->grayImage = destFrame;
        //ip->doMorphOper(name, l, h);
        //cout << "after morph oerp" << endl;
        img = QImage((const unsigned char*)(destFrame.data),destFrame.cols,
                     destFrame.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        cout << "processing Output video" << endl;

        qApp->processEvents();
        //sleep(1);
    }
    // Close the video file.
    capture->release();
}

