#include "brightnessdialog.h"
#include "ui_brightnessdialog.h"

BrightnessDialog::BrightnessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrightnessDialog)
{
    ui->setupUi(this);
}

BrightnessDialog::~BrightnessDialog()
{
    delete ui;
}

void BrightnessDialog::on_alphaDoubleSpinBox_valueChanged(double arg1)
{
    double alpha = ui->alphaDoubleSpinBox->value();
    int beta = ui->betaSpinBox->value();

    sendBrightnessSignal(alpha, beta);
}

void BrightnessDialog::on_betaSpinBox_valueChanged(int arg1)
{
    double alpha = ui->alphaDoubleSpinBox->value();
    int beta = ui->betaSpinBox->value();

    sendBrightnessSignal(alpha, beta);
}
