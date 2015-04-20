#ifndef BRIGHTNESSDIALOG_H
#define BRIGHTNESSDIALOG_H

#include <QDialog>

namespace Ui {
class BrightnessDialog;
}

class BrightnessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrightnessDialog(QWidget *parent = 0);
    ~BrightnessDialog();

signals:
    void sendBrightnessSignal(double alpha, int beta);

private slots:
    void on_alphaDoubleSpinBox_valueChanged(double arg1);

    void on_betaSpinBox_valueChanged(int arg1);

private:
    Ui::BrightnessDialog *ui;
};

#endif // BRIGHTNESSDIALOG_H
