#include "harrisdialog.h"
#include "ui_harrisdialog.h"

HarrisDialog::HarrisDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HarrisDialog)
{
    ui->setupUi(this);
    this->blockSize = ui->HarrisBlockSizeSpinBox->value();
    this->aperture = ui->HarrisApertureSpinBox->value();
    this->kValue = ui->HarrisKSpinBox->value();
    this->threshold = ui->HarrisThreshSpinBox->value();
}

HarrisDialog::~HarrisDialog()
{
    delete ui;
}

void HarrisDialog::on_HarrisCloseButton_clicked()
{
    this->close();
}

void HarrisDialog::on_HarrisBlockSizeSpinBox_valueChanged(int val)
{
    this->blockSize = val;
    emit sendHarrisVals(blockSize, aperture, kValue, threshold);

}

void HarrisDialog::on_HarrisApertureSpinBox_valueChanged(int val)
{
    this->aperture = val;
    emit sendHarrisVals(blockSize, aperture, kValue, threshold);
}

void HarrisDialog::on_HarrisKSpinBox_valueChanged(double val)
{
    this->kValue = val;
    emit sendHarrisVals(blockSize, aperture, kValue, threshold);
}

void HarrisDialog::on_HarrisThreshSpinBox_valueChanged(int val)
{
    this->threshold = val;
    emit sendHarrisVals(blockSize, aperture, kValue, threshold);
}
