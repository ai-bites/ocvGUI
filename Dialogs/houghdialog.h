#ifndef HOUGHDIALOG_H
#define HOUGHDIALOG_H

#include <QDialog>

namespace Ui {
class HoughDialog;
}

class HoughDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HoughDialog(QWidget *parent = 0);
    ~HoughDialog();

signals:
    void sendHoughLineVals(int, int, int, int, bool);
    void sendHoughCircleVals(double, double, int, int, bool);
private slots:
    void on_HoughLineMethodComboBox_currentIndexChanged(int index);

    void on_HoughLineEdgeCheckBox_clicked();

    void on_HoughLineThreshSpinBox_valueChanged(int arg1);

    void on_HoughLineRhoSpinBox_valueChanged(int arg1);

    void on_HoughLineThetSpinBox_valueChanged(int arg1);

    void on_HoughCloseButton_clicked();

    void on_HoughCircleBlurCheckBox_clicked();

    void on_HoughCircleCannyTSpinBox_valueChanged(double arg1);

    void on_HoughCircleDetectTSpinBox_valueChanged(double arg1);

    void on_HoughCircleMaxRadSpinBox_valueChanged(int arg1);

    void on_HoughCircleMinRadSpinBox_valueChanged(int arg1);

private:
    Ui::HoughDialog *ui;
    // for line
    bool lineEdgeDetect;
    int lineThreshold;
    int lineRhoRes;
    int lineThetaRes;
    int lineMethodIdx;
    // for circle
    bool circleApplyBlur;
    double circleCannyThresh;
    double circleDetectThresh;
    int circleMinRad;
    int circleMaxRad;

};

#endif // HOUGHDIALOG_H
