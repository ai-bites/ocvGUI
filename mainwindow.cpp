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
    this->vp = new VideoProcess();
    vp->ip = this->ip;
    this->isImgLoaded = false;
    this->isVideoLoaded = false;
    this->videoStop = false;
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
    cv::Mat temp;

    cvtColor(ip->opImage,temp, CV_GRAY2RGB);
    QImage img = QImage((const unsigned char*)(temp.data),temp.cols,temp.rows,QImage::Format_RGB888);

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
        cout << "going to do video morph" << endl;
        vp->doMorphOperation(choice,w,h);
    }

}


void MainWindow::handleSnPNoiseSignal(QString name, int n)
{
    ui->OutputLabel->clear();
    ip->addNoise(name, n);
    displayOp();
}


void MainWindow::handleImageOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    ip->image = imread(fileName.toStdString(),CV_LOAD_IMAGE_COLOR);

    if(! ip->image.data )
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

    cout << "image size is: " << ip->image.cols << " " << ip->image.rows << endl;

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

    // lets start the output window as a new thread before starting
    // the input window
    QThread * opVideoThread = new QThread;
    this->vp->capture = &capture;
    connect(this, SIGNAL(sendCapture()), this->vp, SLOT(displayOpVideo()));
    this->vp->moveToThread(opVideoThread);

    // op started as new thread, now send input capture
    emit sendCapture();
    opVideoThread->start();

    // now display the input capture in input window
    while (!videoStop)
    {
        // read next frame if any
        capture >> frame;
        cvtColor(frame,destFrame,CV_BGR2RGB);
        img = QImage((const unsigned char*)(destFrame.data),
              destFrame.cols,destFrame.rows,QImage::Format_RGB888);

        emit sendVideoIpImage(img);
        cout << "processing Input video" << endl;
        //sleep(1);
        qApp->processEvents();
    }

    // Close the video file.
    capture.release();

}


void MainWindow::updateVideoIpImage(QImage img)
{
    qApp->processEvents();
    ui->vIpLabel->setPixmap(QPixmap::fromImage(img));
    ui->vIpLabel->setScaledContents(true);
    ui->vIpLabel->resize(ui->vIpFrame->width(), ui->vIpFrame->height());
}


void MainWindow::updateVideoOpImage(QImage img)
{
    cout << "in update video op" << endl;
    ui->vOpLabel->setPixmap(QPixmap::fromImage(img));
    ui->vOpLabel->setScaledContents(true);
    ui->vOpLabel->resize(ui->vIpFrame->width(), ui->vIpFrame->height());
}


//!
//! All the triggers for actions from Operations menu
//!
void MainWindow::on_actionMorphology_triggered()
{
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;
    MorphologyDialog * d = new MorphologyDialog(this);
    connect(d,  SIGNAL(sendMorphSignal(QString, int, int)),
            this, SLOT(handleMorphSignal(QString, int, int)));

    d->show();
}


void MainWindow::on_actionAdd_Noise_triggered()
{
    if (this->isImgLoaded == false && this->isVideoLoaded == false) return;

    NoiseDialog * nd = new NoiseDialog(this);
    connect(nd, SIGNAL(sendSnPNoise(QString,int)), this, SLOT(handleSnPNoiseSignal(QString, int)));
    nd->show();
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
            cv::VideoCapture capture(fileName.toStdString());
            this->handleVideoOpen(capture);
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
        if (capture.isOpened())
        {
            this->isVideoLoaded = true;
            this->handleVideoOpen(capture);
        }
    }
    else
    {
        videoStop = true;
        vp->liveStreamStop = true;
        ui->vIpLabel->clear();
        ui->vOpLabel->clear();
    }
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
}

//! End ************************************************!//
