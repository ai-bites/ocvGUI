#include "featuresdialog.h"
#include "ui_featuresdialog.h"

FeaturesDialog::FeaturesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeaturesDialog)
{
    ui->setupUi(this);
    this->fastThreshold = ui->FastSpinBox->value();
    this->currentMethod = ui->FeaturesMethodCombBox->currentIndex();
    this->siftLineSensThresh = ui->SiftLineSensThreshSpinBox->value();
    this->siftThreshold = ui->SiftThreshSpinBox->value();
    this->surfThreshold = ui->SurfThreshSpinBox->value();
}

FeaturesDialog::~FeaturesDialog()
{
    delete ui;
}

void FeaturesDialog::on_FastSpinBox_valueChanged(int val)
{
    if (this->currentMethod == 0) return;
    this->fastThreshold = val;
    emit sendFeatureVals(fastThreshold, currentMethod,
                         siftThreshold, siftLineSensThresh, surfThreshold);
}

void FeaturesDialog::on_FeaturesMethodCombBox_currentIndexChanged(int index)
{
    if (index == 0) return;
    this->currentMethod = index;
    emit sendFeatureVals(fastThreshold, currentMethod,
                         siftThreshold, siftLineSensThresh, surfThreshold);
}

void FeaturesDialog::on_SurfThreshSpinBox_valueChanged(double val)
{
    if (this->currentMethod == 0) return;
    this->surfThreshold = val;
    emit sendFeatureVals(fastThreshold, currentMethod,
                         siftThreshold, siftLineSensThresh, surfThreshold);
}

void FeaturesDialog::on_SiftThreshSpinBox_valueChanged(double val)
{
    if (this->currentMethod == 0) return;
    this->siftThreshold = val;
    emit sendFeatureVals(fastThreshold, currentMethod,
                         siftThreshold, siftLineSensThresh, surfThreshold);
}

void FeaturesDialog::on_SiftLineSensThreshSpinBox_valueChanged(double val)
{
    if (this->currentMethod == 0) return;
    this->siftLineSensThresh = val;
    emit sendFeatureVals(fastThreshold, currentMethod,
                         siftThreshold, siftLineSensThresh, surfThreshold);
}

void FeaturesDialog::on_FeaturesCloseButton_clicked()
{
    this->close();
}
