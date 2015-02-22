#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ip = new ImgProcess();
    this->isImgLoaded = false;

    // establish all the connections with other classes
    connect(this, SIGNAL(sendVideoImage(QImage)), this, SLOT(updateVideoImage(QImage)));
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
    if (isImgLoaded == 0) return;

    ip->doMorphOper(choice,w,h);
    displayOp();
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


void MainWindow::handleVideoOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    //vp->loadVideo(fileName);
    //connect main function to receive processed video
}


void MainWindow::updateVideoImage(QImage img)
{
    ui->vIpLabel->setPixmap(QPixmap::fromImage(img));
    ui->vIpLabel->setScaledContents(true);
    ui->vIpLabel->resize(ui->vIpFrame->width(), ui->vIpFrame->height());
}


//!
//! All the triggers for actions from Operations menu
//!
void MainWindow::on_actionMorphology_triggered()
{
    if (this->isImgLoaded == false) return;

    MorphologyDialog * d = new MorphologyDialog(this);
    connect(d,  SIGNAL(sendMorphSignal(QString, int, int)),
            this, SLOT(handleMorphSignal(QString, int, int)));
    d->show();
}


void MainWindow::on_actionAdd_Noise_triggered()
{
    if (this->isImgLoaded == false) return;

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
    if (ui->tabWidget->currentIndex() == 0)
    {
        this->handleImageOpen();
    }
    /*
    if (ui->tabWidget->currentIndex() == 1)
    {
       QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
        QMediaPlayer * player = new QMediaPlayer;

        QMediaPlaylist * playlist = new QMediaPlaylist(player);


        playlist->addMedia(QUrl::fromLocalFile(fileName));

        QVideoWidget * videoWidget = new QVideoWidget;
        player->setVideoOutput(videoWidget);

        videoWidget->show();
        playlist->setCurrentIndex(1);
        player->play();
    }
    */
    else
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
        cv::VideoCapture capture(fileName.toStdString());
        QImage img;

        if (!capture.isOpened()) return;
              // Get the frame rate
              double rate = capture.get(CV_CAP_PROP_FPS);
              cout << "rate is" << rate << endl;

              bool stop(false);
              cv::Mat frame; // current video frame
              cv::Mat destFrame;
              // Delay between each frame in ms
              // corresponds to video frame rate
              int delay= 1000 /rate;
              cout << "delay is: " << delay << endl;
              // for all frames in video
              while (!stop)
              {
                 // read next frame if any
                 if (!capture.read(frame)) break;
                 cvtColor(frame,destFrame,CV_BGR2RGB);
                 img = QImage((const unsigned char*)(destFrame.data),
                              destFrame.cols,destFrame.rows,QImage::Format_RGB888);
                 // introduce a delay
                 if (cv::waitKey(delay) >= 0) stop= true;

                 //emit sendVideoImage(img);
                 updateVideoImage(img);
                 qApp->processEvents();
               }
              // Close the video file.
              // Not required since called by destructor
              capture.release();
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
void MainWindow::on_pushButton_clicked()
{
    this->close();
}


//! End ************************************************!//
