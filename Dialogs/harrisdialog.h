#ifndef HARRISDIALOG_H
#define HARRISDIALOG_H

#include <QDialog>

namespace Ui {
class HarrisDialog;
}

class HarrisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HarrisDialog(QWidget *parent = 0);
    ~HarrisDialog();

signals:
    void sendHarrisVals(int, int, double,int);

private slots:
    void on_HarrisCloseButton_clicked();

    void on_HarrisBlockSizeSpinBox_valueChanged(int arg1);

    void on_HarrisApertureSpinBox_valueChanged(int arg1);

    void on_HarrisKSpinBox_valueChanged(double arg1);

    void on_HarrisThreshSpinBox_valueChanged(int arg1);

private:
    Ui::HarrisDialog *ui;

    int blockSize;
    int aperture;
    double kValue;
    int threshold;
};

#endif // HARRISDIALOG_H
