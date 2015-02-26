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
        cvtColor(ip->opImage,temp, CV_GRAY2RGB);
    else
        temp = ip->opImage.clone();

    QImage img = QImage((const unsigned char*)(temp.data),
                            temp.cols,temp.rows,QImage::Format_RGB888);

    ui->OutputLabel->setPixmap(QPixmap::fromImage(img));
    ui->OutputLabel->setScaledContents(true);
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


void MainWindow::handleSnPNoiseSignal(QString name, int n)
{
    ui->OutputLabel->clear();
    ip->addNoise(name, n);
    displayOp();
}


void MainWindow::handleColorDialogSignal(int idx)
{
    ui->OutputLabel->clear();
    ip->toColourSpace(idx);
    displayOp();
}


void MainWindow::handleBlurDialogSignal(int idx,int kernelL,
                                        int kernelH, double sigmaX, double sigmaY,int medianKernel)
{
    ui->OutputLabel->clear();
    ip->doBlur(idx, kernelL, kernelH, sigmaX, sigmaY, medianKernel);
    displayOp();
}


void MainWindow::handleSobelDialogSignal(int currentIdx, bool applyBlur,
                                         int kernel, int dx, int dy, double dxWeight,
                                         int delta,int scale)
{
    ui->OutputLabel->clear();
    ip->doSobelAndLapOper(currentIdx, applyBlur, kernel, dx, dy, dxWeight, delta, scale);
    displayOp();
}

void MainWindow::handleCannySignal(int kernel, int threshold, bool applyBlur, bool isL2Grad)
{
    ui->OutputLabel->clear();
    ip->doCannyOper(kernel, threshold, applyBlur, isL2Grad);
    displayOp();
}


void MainWindow::handleHoughLineSignal(
        int lineMethodIdx, int lineThreshold, int lineRhoRes, int lineThetaRes, bool lineEdgeDetect)
{
    ui->OutputLabel->clear();
    ip->doHoughLineTransform(lineMethodIdx, lineThreshold, lineRhoRes, lineThetaRes, lineEdgeDetect);
    displayOp();
}

void MainWindow::handleHoughCircleSignal(double circleCannyThresh, double circleDetectThresh,
        int circleMinRad, int circleMaxRad, bool circleApplyBlur)
{
    ui->OutputLabel->clear();
    ip->doHoughCircleTransform(circleCannyThresh, circleDetectThresh, circleMinRad,
                               circleMaxRad, circleApplyBlur);
    displayOp();
}

void MainWindow::handleHarrisSignal(int blockSize, int aperture, double kValue, int threshold)
{
    ui->OutputLabel->clear();
    ip->doHarrisCorner(blockSize, aperture, kValue, threshold);
    displayOp();
}

void MainWindow::handleFeatureVals(int threshold, int methodIdx,
                                   double siftThresh, double siftLineSensThresh,
                                   double surfThresh)
{
    cout << "in handle feature vals" << endl;
    ui->OutputLabel->clear();
    ip->doFeatureExtract(threshold, methodIdx, siftThresh, siftLineSensThresh, surfThresh);
    displayOp();
}

void MainWindow::handleImageOpen()
{
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    //ip->image = imread(fileName.toStdString(),CV_LOAD_IMAGE_COLOR);
    ip->image = imread("/Users/shreya/Desktop/1_nature.jpg");

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
    // Delay between each frame in ms
    int delay = 100;

    // now display the input capture in input window
    while (this->isVideoStopped == false)
    {
        cout << "input: in while loop" << endl;
        // read next frame if any
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2RGB);
        img = QImage((const unsigned char*)(destFrame.data),
              destFrame.cols,destFrame.rows,QImage::Format_RGB888);

        emit sendVideoIpImage(img);
        cout << "input: processing video" << endl;
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
    this->vp->moveToThread(opVideoThread);

    // op started as new thread, now send input capture
    emit sendCapture();
    opVideoThread->start();
}


void MainWindow::updateVideoIpImage(QImage img)
{
    cout << "input: update video" << endl;
    ui->vIpLabel->setPixmap(QPixmap::fromImage(img));
    ui->vIpLabel->setScaledContents(true);
    ui->vIpLabel->resize(ui->vIpFrame->width(), ui->vIpFrame->height());
    qApp->processEvents();
}


void MainWindow::updateVideoOpImage(QImage img)
{
    cout << "output: update video" << endl;
    ui->vOpLabel->setPixmap(QPixmap::fromImage(img));
    ui->vOpLabel->setScaledContents(true);
    ui->vOpLabel->resize(ui->vIpFrame->width(), ui->vIpFrame->height());
}


//!
//! All the triggers for actions from Operations menu
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
    QThread * opMorphThrd;
    opMorphThrd = new QThread;
    vp->moveToThread(opMorphThrd);
    currentThread->exit(0);
    this->currentThread = opMorphThrd;
    this->connect(this,   SIGNAL(sendMorpOper(QString, int, int)),
                  vp, SLOT(doMorphOper(QString, int, int)));
    opMorphThrd->start();

    // we are all set for image and video. now show.
    d->show();
}


void MainWindow::on_actionAdd_Noise_triggered()
{
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    NoiseDialog * nd = new NoiseDialog(this);
    connect(nd, SIGNAL(sendSnPNoise(QString,int)), this, SLOT(handleSnPNoiseSignal(QString, int)));
    nd->show();
}


void MainWindow::on_actionColour_Space_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    // create new colour space dialog
    ColourDialog * cd = new ColourDialog(this);
    connect(cd, SIGNAL(sendColorVals(int)), this, SLOT(handleColorDialogSignal(int)));
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

    hd->show();
}


void MainWindow::on_actionCorners_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    // create new dialog for harris parameters
    HarrisDialog * hd = new HarrisDialog(this);
    connect(hd, SIGNAL(sendHarrisVals(int,int,double,int)),
            this, SLOT(handleHarrisSignal(int,int,double,int)));

    hd->show();
}

void MainWindow::on_actionFAST_triggered()
{
    // check
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    FeaturesDialog * fd = new FeaturesDialog(this);
    connect(fd, SIGNAL(sendFeatureVals(int,int,double,double,double)),
            this, SLOT(handleFeatureVals(int,int,double,double,double)));

    fd->show();
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

}

void MainWindow::on_action_Close_triggered()
{
    // action to close the window
    this->close();
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
        this->isVideoLoaded = false;
        this->isVideoStopped = true;
        vp->isVideoStopped = true;
        ui->vIpLabel->clear();
        ui->vOpLabel->clear();
    }
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
}

//! End ************************************************!//

