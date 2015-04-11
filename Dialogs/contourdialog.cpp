#include "contourdialog.h"
#include "ui_contourdialog.h"

ContourDialog::ContourDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContourDialog)
{
    ui->setupUi(this);
}

ContourDialog::~ContourDialog()
{
    delete ui;
}

void ContourDialog::on_closePushButton_clicked()
{
    this->close();
}

void ContourDialog::on_drawPushButton_clicked()
{
    int edgeThresh = ui->edgeThreshspinBox->value();
    bool doBlur = ui->isEdgeDetectcheckBox->isChecked();
    int methodIdx = ui->contourTypeCombBox->currentIndex();

    emit sendContourVals(edgeThresh, doBlur, methodIdx);
}
