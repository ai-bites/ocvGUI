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

    void on_SnPNoiseSpinBox_valueChanged(int arg1);

private:
    Ui::NoiseDialog *ui;

signals:
    void sendSnPNoise(QString name, int);
};

#endif // NOISEDIALOG_H
