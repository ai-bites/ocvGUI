#include "videoprocess.h"


VideoProcess::VideoProcess()
{
    isVideoStopped = true;
    this->ip = new ImgProcess;
}


void VideoProcess::doMorphOper(QString name, int l=5, int h=5)
{
    QImage img;
    cout << "in do morph video " << endl;

    while (isVideoStopped == false) // it is running
    {
        cout << "processing morph frames " << endl;
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


void VideoProcess::addNoise(QString type, int whiteVal, int blackVal, double mean, double var)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        cout << "processing video frames" << endl;
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame;
        ip->addNoise( type, whiteVal,  blackVal,  mean, var);
        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}


void VideoProcess::doAdaptiveThreshold(int blockSize, double maxVal,
                                       int methodIdx, int threshTypeIdx, double constant)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame;
        ip->doAdaptiveThreshold(blockSize, maxVal, methodIdx, threshTypeIdx, constant);
        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}


void VideoProcess::addLogo(float alpha, int h, int l)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame;
        ip->addLogo(alpha, h, l);

        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}

void VideoProcess::toColourSpace(int csIdx)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame;
        this->ip->image = frame;
        ip->toColourSpace( csIdx );
        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}

void VideoProcess::doBlur(int idx, int kernelL, int kernelH,
                          double sigmaX, double sigmaY, int medianKernel)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame;
        this->ip->image = frame;
        ip->doBlur( idx, kernelL, kernelH, sigmaX, sigmaY, medianKernel);

        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}


void VideoProcess::doBrightContrast(double alpha, int beta)
{
    cout << "adjusting brightness" << endl;
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame;
        this->ip->image = frame;
        ip->doBrightContrast(alpha, beta);

        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}

void VideoProcess::doSobelAndLapOper(int currentIdx, bool applyBlur,
                                     int kernel, int dx, int dy, double dxWeight,
                                     int delta,int scale)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame;
        this->ip->image = frame;
        ip->doSobelAndLapOper(currentIdx, applyBlur,kernel, dx, dy, dxWeight, delta, scale);

        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}


void VideoProcess::doCannyOper(int kernel, int threshold,
                               bool applyBlur, bool isL2Grad, int maxThreshold)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame;
        this->ip->image = frame;
        ip->doCannyOper(kernel, threshold, applyBlur, isL2Grad, maxThreshold);

        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}


void VideoProcess::doHoughLineTransform(
        int lineMethodIdx, int lineThreshold, int lineRhoRes, int lineThetaRes, bool lineEdgeDetect)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame.clone();
        this->ip->image = frame.clone();
        ip->doHoughLineTransform(lineMethodIdx, lineThreshold, lineRhoRes, lineThetaRes, lineEdgeDetect);

        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}


void VideoProcess::doHoughCircleTransform(
        double cannyThresh, double detectThresh, int minRad, int maxRad, bool applyBlur)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame.clone();
        this->ip->image = frame.clone();
        ip->doHoughCircleTransform(cannyThresh, detectThresh, minRad, maxRad, applyBlur);

        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}


void VideoProcess::doHarrisCorner(int blockSize, int aperture,double kValue, int threshold)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame.clone();
        this->ip->image = frame.clone();
        ip->doHarrisCorner( blockSize,  aperture, kValue, threshold);

        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}


void VideoProcess::doFeatureExtract(int fastThresh, int methodIdx,
                                    double siftThresh, double siftLineSensthresh, double surfThresh)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame.clone();
        this->ip->image = frame.clone();
        ip->doFeatureExtract(fastThresh, methodIdx, siftThresh, siftLineSensthresh, surfThresh);

        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}


void VideoProcess::doHistogram(int numBins, bool showHistEqImg)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame.clone();
        this->ip->image = frame.clone();
        ip->doHistogram(numBins, showHistEqImg);

        img = QImage((const unsigned char*)(ip->opImage.data),ip->opImage.cols,
                     ip->opImage.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    capture.release();
}


void VideoProcess::drawContours(int edgeThresh, bool doBlur,
                                int methodIdx, bool showRect, bool showCircle)
{
    QImage img;

    while (isVideoStopped == false) // it is running
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        this->ip->grayImage = destFrame.clone();
        this->ip->image = frame.clone();
        ip->drawContours(edgeThresh, doBlur, methodIdx, showRect, showCircle);

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

    // for all frames in video
    while (!isVideoStopped)
    {
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2GRAY);
        img = QImage((const unsigned char*)(destFrame.data),destFrame.cols,
                     destFrame.rows,QImage::Format_Indexed8);

        emit sendVidoeOpImg(img);
        qApp->processEvents();
        sleep(0.5);
    }
    // Close the video file.
    capture.release();
}

