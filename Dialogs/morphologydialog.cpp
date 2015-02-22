#include "morphologydialog.h"
#include "ui_morphologydialog.h"

#include <iostream>

MorphologyDialog::MorphologyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MorphologyDialog)
{
    ui->setupUi(this);
}

MorphologyDialog::~MorphologyDialog()
{
    delete ui;
}

void MorphologyDialog::on_morphOkButton_accepted()
{
    this->currentOption = this->ui->MorphComboBox->currentText();
    int h = this->ui->MorphHeightspinBox->value();
    int w = this->ui->MorphWidthSpinBox->value();
    emit sendMorphSignal(currentOption, h, w);
}

void MorphologyDialog::on_MorphHeightspinBox_valueChanged(int h)
{
    this->currentOption = this->ui->MorphComboBox->currentText();
    int w = this->ui->MorphWidthSpinBox->value();
    emit sendMorphSignal(currentOption, h, w);
}

void MorphologyDialog::on_MorphWidthSpinBox_valueChanged(int w)
{
    this->currentOption = this->ui->MorphComboBox->currentText();
    int h = this->ui->MorphHeightspinBox->value();
    emit sendMorphSignal(currentOption, h, w);
}
