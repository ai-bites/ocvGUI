#ifndef SOBELDIALOG_H
#define SOBELDIALOG_H

#include <QDialog>

namespace Ui {
class SobelDialog;
}

class SobelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SobelDialog(QWidget *parent = 0);
    ~SobelDialog();

signals:
    void sendSobelVals(int, bool, int, int, int,double, int, int);

private slots:
    void on_SobelComboBox_currentIndexChanged(int index);

    void on_sobelApplyBlurCheckBox_clicked();

    void on_SobelKernelspinBox_valueChanged(int arg1);

    void on_SobelDxSpinBox_valueChanged(int arg1);

    void on_SobelDxWeightSpinBox_valueChanged(double arg1);

    void on_SobelDySpinBox_valueChanged(int arg1);

    void on_SobelDeltaSpinBox_valueChanged(int arg1);

    void on_SobelScaleSpinBox_valueChanged(int arg1);

private:
    Ui::SobelDialog *ui;
    int currentIdx;
    int kernel;
    bool applyBlur;
    int dx;
    int dy;
    double dxWeight;
    int delta;
    int scale;
};

#endif // SOBELDIALOG_H
