#include "houghdialog.h"
#include "ui_houghdialog.h"

HoughDialog::HoughDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HoughDialog)
{
    ui->setupUi(this);
    this->lineMethodIdx = ui->HoughLineMethodComboBox->currentIndex();
    this->lineRhoRes = ui->HoughLineRhoSpinBox->value();
    this->lineThetaRes = ui->HoughLineThetSpinBox->value();
    this->lineThreshold = ui->HoughLineThreshSpinBox->value();
    this->lineEdgeDetect = ui->HoughLineEdgeCheckBox->isChecked();

    this->circleApplyBlur = ui->HoughCircleBlurCheckBox->isChecked();
    this->circleCannyThresh = ui->HoughCircleCannyTSpinBox->value();
    this->circleDetectThresh = ui->HoughCircleDetectTSpinBox->value();
    this->circleMinRad = ui->HoughCircleMinRadSpinBox->value();
    this->circleMaxRad = ui->HoughCircleMaxRadSpinBox->value();
}

HoughDialog::~HoughDialog()
{
    delete ui;
}



void HoughDialog::on_HoughLineMethodComboBox_currentIndexChanged(int index)
{
    if (index == 0) return;
    this->lineMethodIdx = index;
    emit sendHoughLineVals(lineMethodIdx, lineThreshold, lineRhoRes, lineThetaRes, lineEdgeDetect);
}


void HoughDialog::on_HoughLineEdgeCheckBox_clicked()
{
    if (ui->HoughLineMethodComboBox->currentIndex() == 0) return;
    this->lineEdgeDetect = ui->HoughLineEdgeCheckBox->isChecked();
    emit sendHoughLineVals(lineMethodIdx, lineThreshold, lineRhoRes, lineThetaRes, lineEdgeDetect);
}

void HoughDialog::on_HoughLineThreshSpinBox_valueChanged(int val)
{
    if (ui->HoughLineMethodComboBox->currentIndex() == 0) return;
    this->lineThreshold = val;
    emit sendHoughLineVals(lineMethodIdx, lineThreshold, lineRhoRes, lineThetaRes, lineEdgeDetect);
}

void HoughDialog::on_HoughLineRhoSpinBox_valueChanged(int val)
{
    if (ui->HoughLineMethodComboBox->currentIndex() == 0) return;
    this->lineRhoRes = val;
    emit sendHoughLineVals(lineMethodIdx, lineThreshold, lineRhoRes, lineThetaRes, lineEdgeDetect);
}

void HoughDialog::on_HoughLineThetSpinBox_valueChanged(int val)
{
    if (ui->HoughLineMethodComboBox->currentIndex() == 0) return;
    this->lineThetaRes = val;
    emit sendHoughLineVals(lineMethodIdx, lineThreshold, lineRhoRes, lineThetaRes, lineEdgeDetect);
}

void HoughDialog::on_HoughCloseButton_clicked()
{
    this->close();
}

void HoughDialog::on_HoughCircleBlurCheckBox_clicked()
{
    this->circleApplyBlur = ui->HoughCircleBlurCheckBox->isChecked();
    emit sendHoughCircleVals(
                circleCannyThresh, circleDetectThresh, circleMinRad, circleMaxRad, circleApplyBlur);
}

void HoughDialog::on_HoughCircleCannyTSpinBox_valueChanged(double val)
{
    this->circleCannyThresh = val;
    emit sendHoughCircleVals(
                circleCannyThresh, circleDetectThresh, circleMinRad, circleMaxRad, circleApplyBlur);
}

void HoughDialog::on_HoughCircleDetectTSpinBox_valueChanged(double val)
{
    this->circleDetectThresh = val;
    emit sendHoughCircleVals(
                circleCannyThresh, circleDetectThresh, circleMinRad, circleMaxRad, circleApplyBlur);
}


void HoughDialog::on_HoughCircleMaxRadSpinBox_valueChanged(int val)
{
    this->circleMaxRad - val;
    emit sendHoughCircleVals(
                circleCannyThresh, circleDetectThresh, circleMinRad, circleMaxRad, circleApplyBlur);
}

void HoughDialog::on_HoughCircleMinRadSpinBox_valueChanged(int val)
{
    this->circleMinRad = val;
    emit sendHoughCircleVals(
                circleCannyThresh, circleDetectThresh, circleMinRad, circleMaxRad, circleApplyBlur);
}
