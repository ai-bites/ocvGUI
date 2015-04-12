#include "logodialog.h"
#include "ui_logodialog.h"

LogoDialog::LogoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogoDialog)
{
    ui->setupUi(this);
}

LogoDialog::~LogoDialog()
{
    delete ui;
}

void LogoDialog::on_logoShowpushButton_clicked()
{
    double alpha = ui->alphaDoubleSpinBox->value();
    int height = ui->logoHeightSpinBox->value();
    int width = ui->logoWidthSpinBox->value();

    emit sendLogoVals(alpha, height, width);
}

void LogoDialog::on_logoClosepushButton_clicked()
{
    this->close();
}
