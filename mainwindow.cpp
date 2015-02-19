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

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::displayOp()
{
    cv::Mat temp;
    cvtColor(ip->opImage,temp, CV_GRAY2RGB);
    QImage img = QImage((const unsigned char*)(temp.data),
                       temp.cols,temp.rows,QImage::Format_RGB888);
    ui->OutputLabel->setPixmap(QPixmap::fromImage(img));
    ui->OutputLabel->setPixmap(QPixmap::fromImage(img));
    ui->OutputLabel->setScaledContents(true);
}


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
    cout << "in spinbox edit" << endl;
    ip->addLogo(ui->logoSpinBox->value(), 10,10);
    displayOp();
}


void MainWindow::handleMorphSignal(QString choice, int h, int w)
{
    if (isImgLoaded == 0) return;

    ip->doMorphOper(choice,w,h);
    displayOp();
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

//! End ************************************************!//


//!
//! All the triggers for actions from File menu
//!
void MainWindow::on_action_Open_triggered()
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

    return;
}


void MainWindow::on_action_Save_triggered()
{

}

void MainWindow::on_action_Close_triggered()
{
    // action to close the window
}

//! End ************************************************!//
