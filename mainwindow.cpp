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
}

MainWindow::~MainWindow()
{
    delete ui;
}

Ui::MainWindow * MainWindow::getUI() { return this->ui;}

/*
void MainWindow::on_LoadImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));

    this->image = imread(fileName.toStdString());
    if(! image.data )
    {
        cout <<  "Could not open or find the image" << endl;
        return;
    }
    // change color channel ordering
    cv::cvtColor(image,image,CV_BGR2RGB);
    QImage img = QImage((const unsigned char*)(this->image.data),
                    image.cols,image.rows,QImage::Format_RGB888);
    // display on label
    ui->InputLabel->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->InputLabel->resize(ui->InputLabel->pixmap()->size());

    ui->OutputLabel->setPixmap(QPixmap::fromImage(img));
    ui->OutputLabel->resize(ui->OutputLabel->pixmap()->size());
}

void MainWindow::on_FlipButton_clicked()
{
    flip(this->image,this->image,1);

    cv::cvtColor(this->image,this->image,CV_BGR2RGB);
    QImage img = QImage((const unsigned char*)(this->image.data),
                        this->image.cols,this->image.rows,QImage::Format_RGB888);
    ui->InputLabel->setPixmap(QPixmap::fromImage(img));
    //ui->InputLabel->resize(ui->InputLabel->pixmap()->size());

    //ui->OutputLabel->setPixmap(QPixmap::fromImage(img));
    //ui->OutputLabel->resize(ui->OutputLabel->pixmap()->size());
}
*/

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));

    ip->image = imread(fileName.toStdString());
    if(! ip->image.data )
    {
        cout <<  "Could not open or find the image" << endl;
        return;
    }

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
}

void MainWindow::on_action_Close_triggered()
{
    // action to close the window
}


/**
 * @brief MainWindow::on_action_Save_triggered: Saves the output file.
 */
void MainWindow::on_action_Save_triggered()
{

}



void MainWindow::on_MorphComboBox_activated(const QString &value)
{
    cout << "morph box activated" << value.toStdString() << endl;
    if (value.toStdString() == "select") { return; }
    ip->doMorphOper(value);
    displayOp();
}


void MainWindow::displayOp()
{
    QImage img = QImage((const unsigned char*)(ip->opImage.data),
                       ip->opImage.cols,ip->opImage.rows,QImage::Format_RGB888);
    ui->OutputLabel->setPixmap(QPixmap::fromImage(img));
    ui->OutputLabel->setPixmap(QPixmap::fromImage(img));
    ui->OutputLabel->setScaledContents(true);

}
