#ifndef BLURDIALOG_H
#define BLURDIALOG_H

#include <QDialog>

namespace Ui {
class BlurDialog;
}

class BlurDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BlurDialog(QWidget *parent = 0);
    ~BlurDialog();

signals:
    void sendBlurVals(int,int,int,double,double,int);

private slots:
    void on_BlurComboBox_currentIndexChanged(int index);

    void on_BlurCloseButton_clicked();

    void on_KernelLenspinBox_valueChanged(int arg1);

    void on_kernelHeightSpinBox_valueChanged(int arg1);

    void on_sigmaXSpinBox_valueChanged(double arg1);

    void on_sigmaYSpinBox_valueChanged(double arg1);

    void on_medianKernelSpinBox_valueChanged(int arg1);

private:
    Ui::BlurDialog *ui;
    int currentIndex;
    int kernelL;
    int kernelH;
    double sigmaX;
    double sigmaY;
    int medianKernel;

};

#endif // BLURDIALOG_H
