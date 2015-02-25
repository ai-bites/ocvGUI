#include "blurdialog.h"
#include "ui_blurdialog.h"
#include <iostream>
using namespace std;

BlurDialog::BlurDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlurDialog)
{
    ui->setupUi(this);
    this->currentIndex = ui->BlurComboBox->currentIndex();
    this->kernelH = ui->kernelHeightSpinBox->value();
    this->kernelL = ui->KernelLenspinBox->value();
    this->sigmaX = ui->sigmaXSpinBox->value();
    this->sigmaY = ui->sigmaYSpinBox->value();
    this->medianKernel = ui->medianKernelSpinBox->value();
}

BlurDialog::~BlurDialog()
{
    delete ui;
}

void BlurDialog::on_BlurComboBox_currentIndexChanged(int index)
{
    this->currentIndex = index;
    emit sendBlurVals(currentIndex,kernelL, kernelH, sigmaX, sigmaY, medianKernel);
}

void BlurDialog::on_KernelLenspinBox_valueChanged(int val)
{
    this->kernelL = val;
    cout << "going to emit L" << endl;
    emit sendBlurVals(currentIndex,kernelL, kernelH, sigmaX, sigmaY, medianKernel);
}

void BlurDialog::on_kernelHeightSpinBox_valueChanged(int val)
{
    this->kernelH = val;
    emit sendBlurVals(currentIndex,kernelL, kernelH, sigmaX, sigmaY, medianKernel);
}

void BlurDialog::on_sigmaXSpinBox_valueChanged(double val)
{
    this->sigmaX = val;
    emit sendBlurVals(currentIndex,kernelL, kernelH, sigmaX, sigmaY, medianKernel);
}

void BlurDialog::on_sigmaYSpinBox_valueChanged(double val)
{
    this->sigmaY = val;
    emit sendBlurVals(currentIndex,kernelL, kernelH, sigmaX, sigmaY, medianKernel);
}

void BlurDialog::on_medianKernelSpinBox_valueChanged(int val)
{
    this->medianKernel = val;
    emit sendBlurVals(currentIndex,kernelL, kernelH, sigmaX, sigmaY, medianKernel);
}


void BlurDialog::on_BlurCloseButton_clicked()
{
    this->close();
}
