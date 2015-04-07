#ifndef NOISEDIALOG_H
#define NOISEDIALOG_H

#include <QDialog>

namespace Ui {
class NoiseDialog;
}

class NoiseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NoiseDialog(QWidget *parent = 0);
    ~NoiseDialog();

private slots:

    void on_noiseAddButton_clicked();

private:
    Ui::NoiseDialog *ui;
    QString currentOption;

signals:
    void sendAddNoise(QString currentOption, int, int, double, double);
};

#endif // NOISEDIALOG_H
