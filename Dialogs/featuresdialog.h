#ifndef FEATURESDIALOG_H
#define FEATURESDIALOG_H

#include <QDialog>

namespace Ui {
class FeaturesDialog;
}

class FeaturesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FeaturesDialog(QWidget *parent = 0);
    ~FeaturesDialog();

signals:
    void sendFeatureVals(int threshold,int method, double siftThreshold,
                         double siftLineSensThresh, double surfThreshold);

private slots:
    void on_FastSpinBox_valueChanged(int arg1);

    void on_FeaturesMethodCombBox_currentIndexChanged(int index);

    void on_SurfThreshSpinBox_valueChanged(double arg1);

    void on_SiftThreshSpinBox_valueChanged(double arg1);

    void on_SiftLineSensThreshSpinBox_valueChanged(double arg1);

    void on_FeaturesCloseButton_clicked();

private:
    Ui::FeaturesDialog *ui;

    int fastThreshold;
    int currentMethod;
    double siftThreshold;
    double siftLineSensThresh;
    double surfThreshold;
};

#endif // FEATURESDIALOG_H
