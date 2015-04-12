#ifndef ADAPTIVEDIALOG_H
#define ADAPTIVEDIALOG_H

#include <QDialog>

namespace Ui {
class AdaptiveDialog;
}

class AdaptiveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdaptiveDialog(QWidget *parent = 0);
    ~AdaptiveDialog();

signals:
    void sendAdaptiveThreshVals(int, double, int, int, double);
private slots:
    void on_blockSizeSpinBox_valueChanged(const QString &arg1);

    void on_ThreshTypecomboBox_currentIndexChanged(int index);

    void on_methodComboBox_currentIndexChanged(int index);

    void on_maxValueDoubleSpinBox_valueChanged(double arg1);

    void on_constantDoubleSpinBox_valueChanged(double arg1);

private:
    Ui::AdaptiveDialog *ui;
};

#endif // ADAPTIVEDIALOG_H
