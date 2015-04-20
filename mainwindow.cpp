#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ip = new ImgProcess();
    this->vp = new VideoProcess;
    vp->ip = this->ip;
    this->isImgLoaded = false;
    this->isVideoLoaded = false;
    this->isVideoStopped = true;
    // establish all the connections with other classes
    connect(this, SIGNAL(sendVideoIpImage(QImage)), this, SLOT(updateVideoIpImage(QImage)));
    connect(vp,   SIGNAL(sendVidoeOpImg(QImage)),   this, SLOT(updateVideoOpImage(QImage)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::displayOp()
{
    // to keep Qt display happy
    // we convert everything to color
    cv::Mat temp;
    if (ip->opImage.channels() == 1)
    {
        cvtColor(ip->opImage,temp, CV_GRAY2BGR);
    }
    else
    {
        //temp = ip->opImage.clone();
        cvtColor(ip->opImage,temp, CV_RGB2BGR);
    }

    QImage img = QImage((const unsigned char*)(temp.data),
                            temp.cols,temp.rows,QImage::Format_RGB888);

    ui->OutputLabel->setPixmap(QPixmap::fromImage(img));
    ui->OutputLabel->setScaledContents(true);
}


void MainWindow::refreshIpImg()
{
    cv::Mat temp;
    if (ip->opImage.channels() == 1)
    {
        cvtColor(ip->image,temp, CV_GRAY2RGB);
    }
    else
    {
        //temp = ip->image.clone();
        cvtColor(ip->image, temp, CV_RGB2BGR);
    }

    QImage img = QImage((const unsigned char*)(temp.data),
                            temp.cols,temp.rows,QImage::Format_RGB888);

    ui->InputLabel->setPixmap(QPixmap::fromImage(img));
    ui->InputLabel->setScaledContents(true);
}

void MainWindow::displayImage(Mat inputImg, Mat outputImg, string whichImg)
{

    if (whichImg.compare("input") == 0)
    {
        cv::Mat temp;
        if (inputImg.channels() == 1)
        {
            cvtColor(inputImg,temp, CV_GRAY2BGR);
        }
        else
        {
            //temp = inputImg.clone();
            cvtColor(inputImg,temp, CV_RGB2BGR);
        }

        QImage img = QImage((const unsigned char*)(temp.data),
                                temp.cols,temp.rows,QImage::Format_RGB888);

        ui->InputLabel->setPixmap(QPixmap::fromImage(img));
        ui->InputLabel->setScaledContents(true);
        // finally set to img procssing class to be used there
        ip->firstImg = inputImg;
    }

    if (whichImg.compare("output") == 0)
    {
        cv::Mat temp;
        if (outputImg.channels() == 1)
        {
            cvtColor(outputImg,temp, CV_GRAY2BGR);
        }
        else
        {
            //temp = outputImg.clone();
            cvtColor(outputImg,temp, CV_RGB2BGR);
        }

        QImage img = QImage((const unsigned char*)(temp.data),
                                temp.cols,temp.rows,QImage::Format_RGB888);

        ui->OutputLabel->setPixmap(QPixmap::fromImage(img));
        ui->OutputLabel->setScaledContents(true);
        ip->secondImg = outputImg;
    }
    return;
}




void MainWindow::on_logoCheckBox_clicked()
{

    if (ui->logoCheckBox->isChecked() == true)
    {
        // ask user to provide the logo
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
        // user clicked cancel without adding a logo
        if (fileName.toStdString().compare("") == 0) return;

        ip->logo = imread(fileName.toStdString(),CV_RGB2GRAY);
        // saved it successfully. So show message
        QMessageBox messageBox;
        messageBox.information(0, "Success", "Logo added successfully. Adjust Logo to visualise it !");
        messageBox.setFixedSize(500,200);
    }

}


void MainWindow::handleMorphSignal(QString choice, int h, int w)
{
    if (isImgLoaded)
    {
        ip->doMorphOper(choice,w,h);
        displayOp();
    }
    if (isVideoLoaded)
    {
        // by now thread will be created and
        // connection done and ready.
        // so just emit the signal with values
        emit sendMorpOper(choice, w, h);
    }
}


void MainWindow::handleAddNoise(QString method, int white, int black, double mean, double var)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->addNoise(method, white, black, mean, var);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendAddNoiseParams(method, white, black, mean, var);
    }
}


void MainWindow::handleAdaptiveThreshSignal(int blockSize, double maxVal,
                                            int methodIdx, int threshTypeIdx, double constant)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->doAdaptiveThreshold(blockSize, maxVal, methodIdx, threshTypeIdx, constant);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendAdaptiveThreshParams(blockSize, maxVal, methodIdx, threshTypeIdx, constant);
    }
}

void MainWindow::handleLogoSignal(float alpha, int height, int width)
{
    if (isImgLoaded)
    {
        if (!(ip->logo.data))
        {
            QMessageBox messageBox;
            messageBox.warning(0,"Warning","Logo showing not enabled !");
            messageBox.setFixedSize(500,200);
            return;
        }
        ip->addLogo(alpha, height, width);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendAddLogoParams(alpha, height, width);
    }
}

void MainWindow::handleColorDialogSignal(int idx)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->toColourSpace(idx);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendColorSpaceParams(idx);
    }

}


void MainWindow::handleBlurDialogSignal(int idx,int kernelL,
                                        int kernelH, double sigmaX, double sigmaY,int medianKernel)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->doBlur(idx, kernelL, kernelH, sigmaX, sigmaY, medianKernel);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendBlur(idx, kernelL,kernelH,  sigmaX,  sigmaY, medianKernel);
    }

}


void MainWindow::handleBrightnessSignal(double alpha, int beta)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->doBrightContrast(alpha, beta);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendBrightnessParams(alpha, beta);
    }
}


void MainWindow::handleSobelDialogSignal(int currentIdx, bool applyBlur,
                                         int kernel, int dx, int dy, double dxWeight,
                                         int delta,int scale)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->doSobelAndLapOper(currentIdx, applyBlur, kernel, dx, dy, dxWeight, delta, scale);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendSobelParams(currentIdx, applyBlur, kernel, dx, dy, dxWeight, delta, scale);
    }
}

void MainWindow::handleCannySignal(int kernel, int threshold,
                                   bool applyBlur, bool isL2Grad, int maxThreshold)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->doCannyOper(kernel, threshold, applyBlur, isL2Grad, maxThreshold);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendCannyParams( kernel, threshold, applyBlur, isL2Grad, maxThreshold);
    }

}


void MainWindow::handleHoughLineSignal(
        int lineMethodIdx, int lineThreshold, int lineRhoRes, int lineThetaRes, bool lineEdgeDetect)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->doHoughLineTransform(lineMethodIdx, lineThreshold, lineRhoRes, lineThetaRes, lineEdgeDetect);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendHoughLinesParams(lineMethodIdx, lineThreshold, lineRhoRes, lineThetaRes, lineEdgeDetect);
    }

}

void MainWindow::handleHoughCircleSignal(double circleCannyThresh, double circleDetectThresh,
        int circleMinRad, int circleMaxRad, bool circleApplyBlur)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->doHoughCircleTransform(circleCannyThresh, circleDetectThresh, circleMinRad,
                                   circleMaxRad, circleApplyBlur);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendHoughCirclesParams(circleCannyThresh, circleDetectThresh, circleMinRad,
                                    circleMaxRad, circleApplyBlur);
    }

}

void MainWindow::handleHarrisSignal(int blockSize, int aperture, double kValue, int threshold)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->doHarrisCorner(blockSize, aperture, kValue, threshold);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendHarrisParams( blockSize, aperture, kValue, threshold);
    }

}

void MainWindow::handleFeatureVals(int threshold, int methodIdx,
                                   double siftThresh, double siftLineSensThresh,
                                   double surfThresh)
{

    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->doFeatureExtract(threshold, methodIdx, siftThresh, siftLineSensThresh, surfThresh);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendFeatureParams(threshold, methodIdx, siftThresh, siftLineSensThresh,surfThresh);
    }

}


void MainWindow::handleMatchImages(cv::Mat firstImg, cv::Mat secondImg,
                                   bool isShow, string toDisplay, bool isStitch)
{
    if (isShow == true)
    {
        if (isStitch == false)
            ip->doMatchImages(firstImg, secondImg, isShow);
        else
            ip->doStitchImages(firstImg, secondImg, isShow);
        return;
    }
    displayImage(firstImg, secondImg, toDisplay);
}


void MainWindow::handleEpipolarImages(Mat firstImg, Mat secondImg, bool isShow, string toDisplay, int fMatMethodIdx,
                                      bool showInFirst,bool showInSec,double confLevel,
                                      double epiRatio,double minDist,int minHess)
{
    if (isShow == true)
    {
        ip->doEpipolarLines( isShow,  toDisplay,  fMatMethodIdx,
                             showInFirst, showInSec, confLevel,
                             epiRatio, minDist, minHess);
        displayOp();
        refreshIpImg();
        return;
    }
    displayImage(firstImg, secondImg, toDisplay);
}

void MainWindow::handleHistogram(int numBins, bool showHistEqImg)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->doHistogram(numBins, showHistEqImg);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendHistogramParams(numBins, showHistEqImg);
    }

}


void MainWindow::handleContour(int edgeThresh, bool doBlur,
                               int methodIdx, bool showRect, bool ShowCircle)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->drawContours(edgeThresh, doBlur, methodIdx, showRect, ShowCircle);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendContourParams(edgeThresh, doBlur, methodIdx, showRect, ShowCircle);
    }

}

void MainWindow::handleImageOpen()
{
    if (ui->useDefaultImgCheckBox->isChecked())
    {
        ip->image = imread(":/images/marais.jpg");

        if(! ip->image.data)
        {
            QMessageBox messageBox;
            messageBox.critical(0,"Warning",
                        "Not able to open the default image. Please try uploading one!");
            messageBox.setFixedSize(500,200);
        }
        return;
    }
    else
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
        // user cancelled without adding a file
        if (fileName.toStdString().compare("") == 0) return;
        ip->image = imread(fileName.toStdString(),CV_LOAD_IMAGE_COLOR);
    }
    if(! ip->image.data)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Warning",
                    "Not able to open the file. Please try a different file!");
        messageBox.setFixedSize(500,200);
        return;
    }

    // colour to grayscale
    cv::cvtColor(ip->image, ip->grayImage, CV_BGR2GRAY);

    QImage img = QImage((const unsigned char*)(ip->image.data),
                       ip->image.cols,ip->image.rows,QImage::Format_RGB888);

    //display on label
    ui->InputLabel->setPixmap(QPixmap::fromImage(img));
    ui->InputLabel->setScaledContents(true);

    //resize everything to suit the image
    ui->InputLabel->resize(ui->inFrame->width(), ui->inFrame->height());
    ui->OutputLabel->resize(ui->inFrame->width(), ui->inFrame->height());

    this->isImgLoaded = true;
}


void MainWindow::handleVideoOpen(VideoCapture capture)
{
    QImage img;
    // Get the frame rate
    double rate = capture.get(CV_CAP_PROP_FPS);
    cv::Mat frame;
    cv::Mat destFrame;

    // now display the input capture in input window
    while (this->isVideoStopped == false)
    {
        // read next frame if any
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2RGB);
        img = QImage((const unsigned char*)(destFrame.data),
              destFrame.cols,destFrame.rows,QImage::Format_RGB888);

        emit sendVideoIpImage(img);
        qApp->processEvents();
        sleep(0.5);
    }

    // Close the video file.
    capture.release();

}

void MainWindow::startVideoOp()
{
    // lets start the output window as a new thread before starting
    // the input window
    QThread * opVideoThread = new QThread;
    this->currentThread = opVideoThread;
    this->vp->capture = this->cap;
    connect(this, SIGNAL(sendCapture()), this->vp, SLOT(displayOpVideo()));
    this->vp->moveToThread(opVideoThread);

    // op started as new thread, now send input capture
    emit sendCapture();
    opVideoThread->start();
}


void MainWindow::updateVideoIpImage(QImage img)
{
    ui->vIpLabel->setPixmap(QPixmap::fromImage(img));
    ui->vIpLabel->setScaledContents(true);
    ui->vIpLabel->resize(ui->vIpFrame->width(), ui->vIpFrame->height());
    qApp->processEvents();
}


void MainWindow::updateVideoOpImage(QImage img)
{
    ui->vOpLabel->setPixmap(QPixmap::fromImage(img));
    ui->vOpLabel->setScaledContents(true);
    ui->vOpLabel->resize(ui->vIpFrame->width(), ui->vIpFrame->height());
    qApp->processEvents();
}


//!
//! All the triggers for actions from Operations menu
//! These lead to the creation of a dialog box to change parameters
//! which pops up as a separate window.
//!
void MainWindow::on_actionMorphology_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    // create new dialog and connect
    MorphologyDialog * d = new MorphologyDialog(this);
    connect(d,  SIGNAL(sendMorphSignal(QString, int, int)),
            this, SLOT(handleMorphSignal(QString, int, int)));

    // it could be a video operation. So setup threads for it.
    if (this->isVideoLoaded)
    {
        QThread * opMorphThrd = new QThread;
        //vp->moveToThread(opMorphThrd);
        currentThread->exit(0);

        this->currentThread = opMorphThrd;
        this->connect(this,SIGNAL(sendMorpOper(QString, int, int)),
                      this->vp,  SLOT(doMorphOper(QString, int, int)));

        opMorphThrd->start();
    }

    // we are all set for image and video. now show.
    d->show();
}


void MainWindow::on_actionAdd_Noise_triggered()
{
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    NoiseDialog * nd = new NoiseDialog(this);
    connect(nd, SIGNAL(sendAddNoise(QString,int,int,double,double)),
          this, SLOT(handleAddNoise(QString, int, int, double, double)));

    // it could be a video operation. So setup threads for it.
    if (this->isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendAddNoiseParams(QString,int,int,double,double)),
                      this->vp,  SLOT(addNoise(QString,int,int,double,double)));
        t->start();
    }

    nd->show();
    nd->move(100,0);
}


void MainWindow::on_actionThreshold_triggered()
{
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    AdaptiveDialog * ad = new AdaptiveDialog;
    connect(ad, SIGNAL(sendAdaptiveThreshVals(int,double,int,int,double)),
            this, SLOT(handleAdaptiveThreshSignal(int,double,int,int,double)));

    if (this->isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit();
        this->currentThread = t;

        this->connect(this, SIGNAL(sendAdaptiveThreshParams(int,double,int,int,double)),
                      this->vp, SLOT(doAdaptiveThreshold(int,double,int,int,double)));
        t->start();
    }

    ad->show();
}


void MainWindow::on_actionLogo_triggered()
{
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    LogoDialog * ld = new LogoDialog(this);
    connect(ld,  SIGNAL(sendLogoVals(float, int, int)),
            this, SLOT(handleLogoSignal(float, int, int)));

    if (this->isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit();

        this->connect(this, SIGNAL(sendAddLogoParams(float,int,int)),
                      this->vp, SLOT(addLogo(float,int,int)));
        t->start();
    }
    ld->show();
}

void MainWindow::on_actionColour_Space_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    // create new colour space dialog
    ColourDialog * cd = new ColourDialog(this);
    connect(cd, SIGNAL(sendColorVals(int)), this, SLOT(handleColorDialogSignal(int)));

    if (this->isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendColorSpaceParams(int)),
                      this->vp,  SLOT(toColourSpace(int)));
        t->start();
    }

    cd->show();
}

void MainWindow::on_actionBlur_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    // create a new blurring dialog
    BlurDialog * bd = new BlurDialog(this);
    connect(bd, SIGNAL(sendBlurVals(int,int,int,double,double,int)),
            this, SLOT(handleBlurDialogSignal(int,int,int,double,double,int)));

    if (this->isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendBlur(int,int,int,double,double,int)),
                      this->vp,  SLOT(doBlur(int, int, int,double, double, int)));
        t->start();
    }

    bd->show();
}



void MainWindow::on_actionBrighten_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    BrightnessDialog * bd = new BrightnessDialog;

    connect(bd, SIGNAL(sendBrightnessSignal(double,int)),
            this, SLOT(handleBrightnessSignal(double, int)));

    if (this->isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendBrightnessParams(double,int)),
                      this->vp,  SLOT(doBrightContrast(double,int)));

        t->start();
    }

    bd->show();
}


void MainWindow::on_actionSobel_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    // create a new window for parameters
    SobelDialog * sd = new SobelDialog(this);
    connect(sd, SIGNAL(sendSobelVals(int,bool,int,int,int,double,int,int)),
            this, SLOT(handleSobelDialogSignal(int,bool,int,int,int,double,int,int)));

    if (this->isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendSobelParams(int,bool,int,int,int,double,int,int)),
                      this->vp,  SLOT(doSobelAndLapOper(int,bool,int,int,int,double,int,int)));

        t->start();
    }

    sd->show();
}


void MainWindow::on_actionCanny_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    // create dialog for canny operation params
    CannyDialog * cd = new CannyDialog(this);
    connect(cd, SIGNAL(sendCannyVals(int,int,bool,bool,int)),
            this, SLOT(handleCannySignal(int,int,bool,bool,int)));
    if (isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendCannyParams(int,int,bool,bool,int)),
                      this->vp,  SLOT(doCannyOper( int,int,bool,bool,int)));

        t->start();
    }
    cd->show();
}


void MainWindow::on_actionHough_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    // create new hough dialog
    HoughDialog * hd = new HoughDialog(this);
    connect(hd,   SIGNAL(sendHoughLineVals(int,int,int,int,bool)),
            this, SLOT(handleHoughLineSignal(int,int,int,int,bool)));
    connect(hd,   SIGNAL(sendHoughCircleVals(double,double,int,int,bool)),
            this, SLOT(handleHoughCircleSignal(double,double,int,int,bool)));

    if (isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendHoughLinesParams(int, int, int, int, bool)),
                      this->vp,SLOT(doHoughLineTransform(int, int, int, int, bool)));
        this->connect(this, SIGNAL(sendHoughCirclesParams(double,double,int,int,bool)),
                      this->vp, SLOT(doHoughCircleTransform(double,double,int,int,bool)));
        t->start();
    }
    hd->show();
}


// To be specific, it is Harris corner detection
void MainWindow::on_actionCorners_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    // create new dialog for harris parameters
    HarrisDialog * hd = new HarrisDialog(this);
    connect(hd, SIGNAL(sendHarrisVals(int,int,double,int)),
            this, SLOT(handleHarrisSignal(int,int,double,int)));

    if (isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendHarrisParams(int,int,double,int)),
                      this->vp,SLOT(doHarrisCorner(int,int,double,int)));

        t->start();
    }
    hd->show();
}

void MainWindow::on_actionFAST_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    FeaturesDialog * fd = new FeaturesDialog(this);
    connect(fd, SIGNAL(sendFeatureVals(int,int,double,double,double)),
            this, SLOT(handleFeatureVals(int,int,double,double,double)));

    if (isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendFeatureParams(int,int,double,double,double)),
                      this->vp,SLOT(doFeatureExtract(int, int,double, double, double)));

        t->start();
    }

    fd->show();
}



void MainWindow::on_actionHistogram_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;
    HistogramDialog * hd = new HistogramDialog(this);
    connect(hd,SIGNAL(sendHistogram(int, bool)),
            this, SLOT(handleHistogram(int, bool)));

    if (isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendHistogramParams(int, bool)),
                      this->vp,SLOT(doHistogram(int,bool)));

        t->start();
    }

    hd->show();
}


void MainWindow::on_actionContour_triggered()
{
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    ContourDialog * cd = new ContourDialog;
    connect(cd, SIGNAL(sendContourVals(int, bool, int, bool, bool)),
            this, SLOT(handleContour(int, bool, int, bool, bool)));

    if (isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendContourParams(int, bool, int, bool, bool)),
                      this->vp,SLOT(drawContours(int, bool, int, bool, bool)));

        t->start();
    }

    cd->show();
}


void MainWindow::on_actionEpipolar_triggered()
{
    if (this->isVideoLoaded == true) return;
    EpipolarDialog * ed = new EpipolarDialog;

    connect(ed, SIGNAL(sendEpipolarImages(Mat, Mat,bool,string,int,bool,bool,double,double,double,int)),
            this, SLOT(handleEpipolarImages(Mat, Mat, bool,string,int,bool,bool,double,double,double,int)));

    ui->InputLabel->clear();
    ui->OutputLabel->clear();
    ed->show();
}


void MainWindow::on_actionMatches_triggered()
{
    if (this->isVideoLoaded == true) return;

    MatchesDialog * md = new MatchesDialog(this);
    connect(md, SIGNAL(sendMatchImages(cv::Mat, cv::Mat, bool, string, bool)),
            this, SLOT(handleMatchImages(cv::Mat, cv::Mat, bool, string, bool)));

    ui->InputLabel->clear();
    ui->OutputLabel->clear();
    md->show();
}


//! End ************************************************!//


//!
//! All the triggers for actions from File menu
//!
void MainWindow::on_action_Open_triggered()
{
    // dealing with image
    if (ui->tabWidget->currentIndex() == 0) // image
    {
        this->handleImageOpen();
    }
    if (ui->tabWidget->currentIndex() == 1) // video
    {
            QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));

            // user cancelled without adding a file
            if (fileName.toStdString().compare("") == 0) return;

            cv::VideoCapture ipCapture(fileName.toStdString());
            this->handleVideoOpen(ipCapture);
    }
    return;
}


void MainWindow::on_action_Save_triggered()
{
    if (isImgLoaded && ip->opImage.data)
    {
        QString s = QFileDialog::getSaveFileName(0,"Images (*.png *.xpm *.jpg)",QString(""),
                        QString("Save file dialog"),0);
        if (s.toStdString() == "") return;

       bool res = cv::imwrite(s.toStdString(), ip->opImage);
    }
    else
    {
        QMessageBox messageBox;

        messageBox.critical(
                    0,"Warning",
                    "Not Possible! You are either trying to save a livestream or there is no processed image to save!");
        messageBox.setFixedSize(500,200);
        return;
    }

}

void MainWindow::on_action_Close_triggered()
{
    // action to close the window
    this->close();
    QCoreApplication::quit();
}

//! End ************************************************!//

//!
//! All actions from the Main window (save, close and logo)
//!
void MainWindow::on_StartLiveCheckBox_clicked(bool checked)
{
    if (checked) // start
    {
        // we cannot cascade operations for video
        // so diable it first
        if (ip->isCascadeEnabled)
        {
            ip->isCascadeEnabled = false;
            ui->cascadeCheckBox->setChecked(false);
            QMessageBox messageBox;
            messageBox.information(0,"Message","Disabled cascade operation for video !");
            messageBox.setFixedSize(500,200);
        }

        cv::VideoCapture capture(0);
        cv::VideoCapture ipCapture(0);
        if (capture.isOpened())
        {
            this->isVideoLoaded = true;
            this->isVideoStopped = false;
            vp->isVideoStopped = false;
            this->cap = capture;
            this->startVideoOp();

            // now we are not dealing with imges,
            // so clear and switch to video tab.
            ui->tabWidget->setCurrentIndex(1);
            ui->OutputLabel->clear();
            ui->InputLabel->clear();
            this->isImgLoaded = false;
            this->handleVideoOpen(ipCapture);   
        }
    }
    else
    {
        this->cap.release();
        this->isVideoLoaded = false;
        this->isVideoStopped = true;
        vp->isVideoStopped = true;
        ui->vIpLabel->clear();
        ui->vOpLabel->clear();
    }
}


void MainWindow::on_saveOutput_clicked()
{
    if (isImgLoaded)
    {
        QString s = QFileDialog::getSaveFileName(0,"Images (*.png *.xpm *.jpg)",QString(""),
                        QString("Save file dialog"),0);
        if (s.toStdString() == "") return;

       bool res = cv::imwrite(s.toStdString(), ip->opImage);
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(
                    0,"Warning",
                    "Not Possible! You are either trying to save a livestream or there is no processed image to save!");
        messageBox.setFixedSize(500,200);
        return;
    }
}


void MainWindow::on_closeButton_clicked()
{
    this->close();
    QCoreApplication::quit();
}


//! End ************************************************!//


void MainWindow::on_cascadeCheckBox_clicked()
{
    bool isCascade = ui->cascadeCheckBox->isChecked();
    if (isCascade)
    {
        ip->isCascadeEnabled = true;
        QMessageBox messageBox;
        messageBox.information(0,"Success","Cascade Enabled Successfully !");
        messageBox.setFixedSize(500,200);
    }
    else
    {
        ip->isCascadeEnabled = false;
    }
}

void MainWindow::on_actionCaliberate_triggered()
{
    CalibrateDialog * cd = new CalibrateDialog;
    cd->show();
}
