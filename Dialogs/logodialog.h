#ifndef LOGODIALOG_H
#define LOGODIALOG_H

#include <QDialog>

namespace Ui {
class LogoDialog;
}

class LogoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogoDialog(QWidget *parent = 0);
    ~LogoDialog();

signals:
    void sendLogoVals(double, int, int);

private slots:

    void on_logoShowpushButton_clicked();

    void on_logoClosepushButton_clicked();

private:
    Ui::LogoDialog *ui;
};

#endif // LOGODIALOG_H
