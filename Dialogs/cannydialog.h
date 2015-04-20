#ifndef CANNYDIALOG_H
#define CANNYDIALOG_H

#include <QDialog>

namespace Ui {
class CannyDialog;
}

class CannyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CannyDialog(QWidget *parent = 0);
    ~CannyDialog();

signals:
    void sendCannyVals(int, int, bool, bool, int);

private slots:
    void on_CannyCheckBox_clicked();

    void on_CannyThreshSpinBox_valueChanged(int arg1);

    void on_CannyKernelSpinBox_valueChanged(int arg1);

    void on_CannyCancelButton_clicked();

    void on_CannyLtwoCheckBox_clicked();

    void on_CannyMaxThreshSpinBox_valueChanged(int arg1);

private:
    Ui::CannyDialog *ui;
    int threshold;
    int maxThresh;
    int kernel;
    bool applyBlur;
    bool isL2Grad;
};

#endif // CANNYDIALOG_H
