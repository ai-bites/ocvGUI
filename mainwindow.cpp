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
        cvtColor(ip->opImage,temp, CV_GRAY2RGB);
    }
    else
    {
        temp = ip->opImage.clone();
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
        temp = ip->image.clone();
    }

    QImage img = QImage((const unsigned char*)(temp.data),
                            temp.cols,temp.rows,QImage::Format_RGB888);

    ui->InputLabel->setPixmap(QPixmap::fromImage(img));
    ui->InputLabel->setScaledContents(true);
}

void MainWindow::displayImage(Mat inputImg, Mat outputImg, string whichImg)
{
    cout << "in display image, which img is: " << whichImg << endl;

    if (whichImg.compare("input") == 0)
    {
        cv::Mat temp;
        if (inputImg.channels() == 1)
        {
            cvtColor(inputImg,temp, CV_GRAY2RGB);
        }
        else
        {
            temp = inputImg.clone();
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
            cout << "channels are one" << endl;
            cvtColor(outputImg,temp, CV_GRAY2RGB);
        }
        else
        {
            cout << "setting temp" << endl;
            temp = outputImg.clone();
        }

        QImage img = QImage((const unsigned char*)(temp.data),
                                temp.cols,temp.rows,QImage::Format_RGB888);

        ui->OutputLabel->setPixmap(QPixmap::fromImage(img));
        ui->OutputLabel->setScaledContents(true);
        ip->secondImg = outputImg;
    }
    return;
}



/*
void MainWindow::on_logoCheckBox_clicked()
{
    if (this->isImgLoaded == false) return;

    if (ui->logoCheckBox->isChecked() == true)
    {
        cout << "it is checked" << endl;
        double alpha = ui->logoSpinBox->value();
        ip->addLogo(alpha, 10, 10);
    }
    else
        cout << "it is unchecked" << endl;
    displayOp();
}

void MainWindow::on_logoSpinBox_editingFinished()
{
    if (this->isImgLoaded == false) return;
    cout << "in spinbox edit" << endl;
    ip->addLogo(ui->logoSpinBox->value(), 10,10);
    displayOp();
}
*/


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

void MainWindow::handleCannySignal(int kernel, int threshold, bool applyBlur, bool isL2Grad)
{
    if (isImgLoaded)
    {
        ui->OutputLabel->clear();
        ip->doCannyOper(kernel, threshold, applyBlur, isL2Grad);
        displayOp();
    }
    if (isVideoLoaded)
    {
        emit sendCannyParams( kernel, threshold, applyBlur, isL2Grad);
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
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    //ip->image = imread(fileName.toStdString(),CV_LOAD_IMAGE_COLOR);
    ip->image = imread("/Users/shreya/Desktop/test_images/marais.jpg");

    if(! ip->image.data)
    {
        cout <<  "Could not open or find the image" << endl;
        return;
    }
    //TODO: change to right place or make UI for it.
    ip->logo = imread("/Users/shreya/Desktop/smile.png",CV_RGB2GRAY);

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
    cout << "input: going out of handle" << endl;

}

void MainWindow::startVideoOp()
{
    // lets start the output window as a new thread before starting
    // the input window
    QThread * opVideoThread = new QThread;
    this->currentThread = opVideoThread;
    this->vp->capture = this->cap;
    connect(this, SIGNAL(sendCapture()), this->vp, SLOT(displayOpVideo()));
    cout << "befor emove to thread - start video op" << endl;
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
        cout << "in main, it is video loaded. creating thread" << endl;
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendAddNoiseParams(QString,int,int,double,double)),
                      this->vp,  SLOT(addNoise(QString,int,int,double,double)));
        t->start();
    }

    nd->show();
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
    connect(cd, SIGNAL(sendCannyVals(int,int,bool,bool)),
            this, SLOT(handleCannySignal(int,int,bool,bool)));
    if (isVideoLoaded)
    {
        QThread * t = new QThread;
        currentThread->exit(0);
        this->currentThread = t;

        this->connect(this,SIGNAL(sendCannyParams(int,int,bool,bool)),
                      this->vp,  SLOT(doCannyOper( int,int,bool,bool)));

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

    ed->show();
}


void MainWindow::on_actionMatches_triggered()
{
    if (this->isVideoLoaded == true) return;
    MatchesDialog * md = new MatchesDialog(this);
    connect(md, SIGNAL(sendMatchImages(cv::Mat, cv::Mat, bool, string, bool)),
            this, SLOT(handleMatchImages(cv::Mat, cv::Mat, bool, string, bool)));

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
            cv::VideoCapture ipCapture(fileName.toStdString());
            this->handleVideoOpen(ipCapture);
    }
    return;
}


void MainWindow::on_action_Save_triggered()
{
    if (isImgLoaded)
    {
        QString s = QFileDialog::getSaveFileName(0,"Images (*.png *.xpm *.jpg)",QString(""),
                        QString("Save file dialog"),0);
        if (s.toStdString() == "") return;

        cout << "string is: " << s.toStdString() << endl;

       bool res = cv::imwrite(s.toStdString(), ip->opImage);
       cout << "res is: " << res << endl;
    }
    else
    {
        cout << "nto an image" << endl;
        QMessageBox messageBox;
        messageBox.critical(0,"Warning","Can save only images !");
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
        cv::VideoCapture capture(0);
        cv::VideoCapture ipCapture(0);
        if (capture.isOpened())
        {
            this->isVideoLoaded = true;
            this->isVideoStopped = false;
            vp->isVideoStopped = false;
            this->cap = capture;
            this->startVideoOp();
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

        cout << "string is: " << s.toStdString() << endl;

       bool res = cv::imwrite(s.toStdString(), ip->opImage);
       cout << "res is: " << res << endl;
    }
    else
    {
        cout << "not an image" << endl;
        QMessageBox messageBox;
        messageBox.critical(0,"Warning","Can save only images !");
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

