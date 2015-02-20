#include "noisedialog.h"
#include "ui_noisedialog.h"

NoiseDialog::NoiseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoiseDialog)
{
    ui->setupUi(this);
}

NoiseDialog::~NoiseDialog()
{
    delete ui;
}

void NoiseDialog::on_SnPNoiseSpinBox_valueChanged(int val)
{
    emit sendSnPNoise(QString("SALT_AND_PEPPER"), val);
}
