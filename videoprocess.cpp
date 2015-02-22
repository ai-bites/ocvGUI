#include "videoprocess.h"

#include <iostream>

VideoProcess::VideoProcess()
{
    liveStreamStop = false;
}

void VideoProcess::doMorphOperation(QString name, int l=5, int h=5)
{
    QImage img;

    // Get the frame rate

    cv::Mat frame;
    cv::Mat destFrame;
    // Delay between each frame in ms
    int delay = 100;

    // for all frames in video
    while (!liveStreamStop)
    {
        // read next frame if any
        if (!capture.read(frame)) break;
        //cout << "going to convert to gray" << endl;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        //cout << "after.. " << endl;
        ip->grayImage = destFrame;
        ip->doMorphOper(name, l, h);
        //cout << "after morph oerp" << endl;
        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        cout << "processing Output video" << endl;
        qApp->processEvents();
    }
    // Close the video file.
    capture.release();
}

