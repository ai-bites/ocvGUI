#include "colourdialog.h"
#include "ui_colourdialog.h"

ColourDialog::ColourDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColourDialog)
{
    ui->setupUi(this);
}

ColourDialog::~ColourDialog()
{
    delete ui;
}

void ColourDialog::on_colorSpaceComboBox_currentIndexChanged(int index)
{
    this->currentIdx = index;
    emit sendColorVals(currentIdx);
}

void ColourDialog::on_ColorDialogCloseButton_clicked()
{
    this->close();
}
