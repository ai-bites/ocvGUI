#include "sobeldialog.h"
#include "ui_sobeldialog.h"

SobelDialog::SobelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SobelDialog)
{
    ui->setupUi(this);
    this->currentIdx = ui->SobelComboBox->currentIndex();
    this->kernel = ui->SobelKernelspinBox->value();
    this->dx = ui->SobelDxSpinBox->value();
    this->dy = ui->SobelDySpinBox->value();
    this->dxWeight = ui->SobelDxWeightSpinBox->value();
    this->delta = ui->SobelDeltaSpinBox->value();
    this->applyBlur = ui->sobelApplyBlurCheckBox->isChecked();
    this->scale = ui->SobelScaleSpinBox->value();
}

SobelDialog::~SobelDialog()
{
    delete ui;
}

void SobelDialog::on_SobelComboBox_currentIndexChanged(int index)
{
    if (index == 2) // laplacian
    {
        ui->SobelDxSpinBox->setDisabled(true);
        ui->SobelDxWeightSpinBox->setDisabled(true);
        ui->SobelDySpinBox->setDisabled(true);
    }
    else
    {
        ui->SobelDxSpinBox->setDisabled(false);
        ui->SobelDxWeightSpinBox->setDisabled(false);
        ui->SobelDySpinBox->setDisabled(false);
    }
    this->currentIdx = index;
    emit sendSobelVals(currentIdx, applyBlur, kernel, dx, dy, dxWeight, delta, scale);
}

void SobelDialog::on_sobelApplyBlurCheckBox_clicked()
{
    this->applyBlur = ui->sobelApplyBlurCheckBox->isChecked();
    emit sendSobelVals(currentIdx, applyBlur, kernel, dx, dy, dxWeight, delta, scale);
}

void SobelDialog::on_SobelKernelspinBox_valueChanged(int val)
{
    this->kernel = val;
    emit sendSobelVals(currentIdx, applyBlur, kernel, dx, dy, dxWeight, delta, scale);
}

void SobelDialog::on_SobelDxSpinBox_valueChanged(int val)
{
    this->dx = val;
    emit sendSobelVals(currentIdx, applyBlur, kernel, dx, dy, dxWeight, delta, scale);
}

void SobelDialog::on_SobelDxWeightSpinBox_valueChanged(double val)
{
    this->dxWeight = val;
    emit sendSobelVals(currentIdx, applyBlur, kernel, dx, dy, dxWeight, delta, scale);
}

void SobelDialog::on_SobelDySpinBox_valueChanged(int val)
{
    this->dy = val;
    emit sendSobelVals(currentIdx, applyBlur, kernel, dx, dy, dxWeight, delta, scale);
}


void SobelDialog::on_SobelDeltaSpinBox_valueChanged(int val)
{
    this->delta = val;
    emit sendSobelVals(currentIdx, applyBlur, kernel, dx, dy, dxWeight, delta, scale);
}

void SobelDialog::on_SobelScaleSpinBox_valueChanged(int val)
{
    this->scale = val;
    emit sendSobelVals(currentIdx, applyBlur, kernel, dx, dy, dxWeight, delta, scale);
}
