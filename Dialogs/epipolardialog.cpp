#include "epipolardialog.h"
#include "ui_epipolardialog.h"

EpipolarDialog::EpipolarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EpipolarDialog)
{
    ui->setupUi(this);
}

EpipolarDialog::~EpipolarDialog()
{
    delete ui;
}

void EpipolarDialog::on_loadFirstImgPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    this->firstImg = imread(fileName.toStdString(),CV_LOAD_IMAGE_COLOR);
    string s = "input";

    int methodIdx = ui->FmatMethodComboBox->currentIndex();
    bool showInFirst = ui->showInFirstCheckBox->isChecked();
    bool showInSec = ui->showInSecondCheckBox->isChecked();
    double confLevel = ui->confidenceDoubleSpinBox->value();
    double epiRatio  = ui->epipolarRationSpinBox->value();
    double minDist = ui->minDistDoubleSpinBox->value();
    int minHess = ui->minHessianSpinBox->value();

    emit sendEpipolarImages(this->firstImg, this->secondImg, false, s, methodIdx, showInFirst, showInSec,
                            confLevel, epiRatio, minDist, minHess);
}

void EpipolarDialog::on_loadSecondImgPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    this->secondImg = imread(fileName.toStdString(),CV_LOAD_IMAGE_COLOR);
    string s = "output";

    int methodIdx = ui->FmatMethodComboBox->currentIndex();
    bool showInFirst = ui->showInFirstCheckBox->isChecked();
    bool showInSec = ui->showInSecondCheckBox->isChecked();
    double confLevel = ui->confidenceDoubleSpinBox->value();
    double epiRatio  = ui->epipolarRationSpinBox->value();
    double minDist = ui->minDistDoubleSpinBox->value();
    int minHess = ui->minHessianSpinBox->value();

    emit sendEpipolarImages(this->firstImg, this->secondImg, false, s, methodIdx, showInFirst, showInSec,
                            confLevel, epiRatio, minDist, minHess);
}

void EpipolarDialog::on_closePushButton_clicked()
{
    this->close();
}


void EpipolarDialog::on_showEpiLinesPushButton_clicked()
{
    if (!(this->firstImg.data) || !(this->secondImg.data))
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Warning","Please load both images first !");
        messageBox.setFixedSize(500,200);
        return;
    }

    int methodIdx = ui->FmatMethodComboBox->currentIndex();

    if (methodIdx == 0)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Warning","Please select F matrix computation method !");
        messageBox.setFixedSize(500,200);
        return;
    }

    bool showInFirst = ui->showInFirstCheckBox->isChecked();
    bool showInSec = ui->showInSecondCheckBox->isChecked();

    if (showInFirst == false && showInSec == false)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Warning","Please choose which image to visualize");
        messageBox.setFixedSize(500,200);
        return;
    }

    double confLevel = ui->confidenceDoubleSpinBox->value();
    double epiRatio  = ui->epipolarRationSpinBox->value();
    double minDist = ui->minDistDoubleSpinBox->value();
    int minHess = ui->minHessianSpinBox->value();

    emit sendEpipolarImages(this->firstImg, this->secondImg, true, "", methodIdx, showInFirst, showInSec,
                            confLevel, epiRatio, minDist, minHess);
}
