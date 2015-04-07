#include "matchesdialog.h"
#include "ui_matchesdialog.h"
#include <iostream>

using namespace std;

MatchesDialog::MatchesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MatchesDialog)
{
    ui->setupUi(this);
}

MatchesDialog::~MatchesDialog()
{
    delete ui;
}

void MatchesDialog::on_loadSecondImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    this->secondImg = imread(fileName.toStdString(),CV_LOAD_IMAGE_COLOR);
    string s = "output";
    emit sendMatchImages(this->firstImg, this->secondImg, false, s);
}

void MatchesDialog::on_showMatches_clicked()
{
    if (!(this->firstImg.data) || !(this->secondImg.data))
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Warning","Please load both images first !");
        messageBox.setFixedSize(500,200);
        return;
    }

    emit sendMatchImages(this->firstImg, this->secondImg, true, "");
}

void MatchesDialog::on_loadFirstImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    this->firstImg = imread(fileName.toStdString(),CV_LOAD_IMAGE_COLOR);
    string s = "input";
    std::cout << "going to emit" << std::endl;
    emit sendMatchImages(this->firstImg, this->secondImg, false, s);
}
