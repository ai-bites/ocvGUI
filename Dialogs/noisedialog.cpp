#include "noisedialog.h"
#include "ui_noisedialog.h"
#include "iostream"

using namespace std;

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

void NoiseDialog::on_noiseAddButton_clicked()
{
    this->currentOption = ui->noiseTypeComboBox->currentText();
    int w = ui->snpWhiteSpinBox->value();
    int b = ui->snpBlackSpinBox->value();
    double m = ui->gaussMeanSpinBox->value();
    double v = ui->gaussVarSpinBox->value();

    cout << "in noise dialog params are: " << currentOption.toStdString() << w << b << m << v << endl;

    emit sendAddNoise(currentOption, w, b, m, v);
}
