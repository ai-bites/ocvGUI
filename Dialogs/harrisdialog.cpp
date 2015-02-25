#include "harrisdialog.h"
#include "ui_harrisdialog.h"

HarrisDialog::HarrisDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HarrisDialog)
{
    ui->setupUi(this);
}

HarrisDialog::~HarrisDialog()
{
    delete ui;
}
