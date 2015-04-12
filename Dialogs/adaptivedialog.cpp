#include "adaptivedialog.h"
#include "ui_adaptivedialog.h"

AdaptiveDialog::AdaptiveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdaptiveDialog)
{
    ui->setupUi(this);
}

AdaptiveDialog::~AdaptiveDialog()
{
    delete ui;
}

void AdaptiveDialog::on_blockSizeSpinBox_valueChanged(const QString &arg1)
{
    int blockSize = ui->blockSizeSpinBox->value();
    double constant = ui->constantDoubleSpinBox->value();
    double maxVal = ui->maxValueDoubleSpinBox->value();
    int methodIdx = ui->methodComboBox->currentIndex();
    int threshTypeIdx = ui->ThreshTypecomboBox->currentIndex();

    emit sendAdaptiveThreshVals(blockSize, maxVal, methodIdx, threshTypeIdx, constant);
}

void AdaptiveDialog::on_ThreshTypecomboBox_currentIndexChanged(int index)
{
    int blockSize = ui->blockSizeSpinBox->value();
    double constant = ui->constantDoubleSpinBox->value();
    double maxVal = ui->maxValueDoubleSpinBox->value();
    int methodIdx = ui->methodComboBox->currentIndex();
    int threshTypeIdx = ui->ThreshTypecomboBox->currentIndex();

    emit sendAdaptiveThreshVals(blockSize, maxVal, methodIdx, threshTypeIdx, constant);
}

void AdaptiveDialog::on_methodComboBox_currentIndexChanged(int index)
{
    int blockSize = ui->blockSizeSpinBox->value();
    double constant = ui->constantDoubleSpinBox->value();
    double maxVal = ui->maxValueDoubleSpinBox->value();
    int methodIdx = ui->methodComboBox->currentIndex();
    int threshTypeIdx = ui->ThreshTypecomboBox->currentIndex();

    emit sendAdaptiveThreshVals(blockSize, maxVal, methodIdx, threshTypeIdx, constant);
}

void AdaptiveDialog::on_maxValueDoubleSpinBox_valueChanged(double arg1)
{
    int blockSize = ui->blockSizeSpinBox->value();
    double constant = ui->constantDoubleSpinBox->value();
    double maxVal = ui->maxValueDoubleSpinBox->value();
    int methodIdx = ui->methodComboBox->currentIndex();
    int threshTypeIdx = ui->ThreshTypecomboBox->currentIndex();

    emit sendAdaptiveThreshVals(blockSize, maxVal, methodIdx, threshTypeIdx, constant);
}

void AdaptiveDialog::on_constantDoubleSpinBox_valueChanged(double arg1)
{
    int blockSize = ui->blockSizeSpinBox->value();
    double constant = ui->constantDoubleSpinBox->value();
    double maxVal = ui->maxValueDoubleSpinBox->value();
    int methodIdx = ui->methodComboBox->currentIndex();
    int threshTypeIdx = ui->ThreshTypecomboBox->currentIndex();

    emit sendAdaptiveThreshVals(blockSize, maxVal, methodIdx, threshTypeIdx, constant);
}

void AdaptiveDialog::on_closeButton_clicked()
{
    this->close();
}
