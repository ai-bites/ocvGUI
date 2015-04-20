#include "cannydialog.h"
#include "ui_cannydialog.h"

CannyDialog::CannyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CannyDialog)
{
    ui->setupUi(this);
    threshold = ui->CannyThreshSpinBox->value();
    kernel = ui->CannyKernelSpinBox->value();
    applyBlur = ui->CannyCheckBox->isChecked();
    isL2Grad = ui->CannyLtwoCheckBox->isChecked();
}

CannyDialog::~CannyDialog()
{
    delete ui;
}

void CannyDialog::on_CannyCheckBox_clicked()
{
    this->applyBlur = ui->CannyCheckBox->isChecked();
    emit sendCannyVals(kernel, threshold, applyBlur, isL2Grad, maxThresh);
}

void CannyDialog::on_CannyThreshSpinBox_valueChanged(int val)
{
    this->threshold = val;
    emit sendCannyVals(kernel, threshold, applyBlur, isL2Grad, maxThresh);
}

void CannyDialog::on_CannyMaxThreshSpinBox_valueChanged(int val)
{
    this->maxThresh = val;
    emit sendCannyVals(kernel, threshold, applyBlur, isL2Grad, maxThresh);
}

void CannyDialog::on_CannyKernelSpinBox_valueChanged(int val)
{
    this->kernel = val;
    emit sendCannyVals(kernel, threshold, applyBlur, isL2Grad, maxThresh);
}

void CannyDialog::on_CannyCancelButton_clicked()
{
    this->close();
}

void CannyDialog::on_CannyLtwoCheckBox_clicked()
{
    this->isL2Grad = ui->CannyLtwoCheckBox->isChecked();
    emit sendCannyVals(kernel, threshold, applyBlur, isL2Grad, maxThresh);
}


