#include "featuresdialog.h"
#include "ui_featuresdialog.h"

FeaturesDialog::FeaturesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeaturesDialog)
{
    ui->setupUi(this);
}

FeaturesDialog::~FeaturesDialog()
{
    delete ui;
}
