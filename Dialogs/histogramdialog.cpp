#include "histogramdialog.h"
#include "ui_histogramdialog.h"

HistogramDialog::HistogramDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistogramDialog)
{
    ui->setupUi(this);
}

HistogramDialog::~HistogramDialog()
{
    delete ui;
}

void HistogramDialog::on_showHistPushButton_clicked()
{
    int numBins = ui->numBinsSpinBox->value();
    bool showHistEqImg = ui->showHistEqImage->isChecked();
    emit sendHistogram(numBins, showHistEqImg);
}

void HistogramDialog::on_showHistEqImage_clicked(bool checked)
{
    int numBins = ui->numBinsSpinBox->value();
    bool showHistEqImg = ui->showHistEqImage->isChecked();
    emit sendHistogram(numBins, showHistEqImg);
}
