#ifndef CONTOURDIALOG_H
#define CONTOURDIALOG_H

#include <QDialog>

namespace Ui {
class ContourDialog;
}

class ContourDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContourDialog(QWidget *parent = 0);
    ~ContourDialog();

signals:
    void sendContourVals(int, bool, int, bool, bool);

private slots:
    void on_closePushButton_clicked();

    void on_drawPushButton_clicked();

private:
    Ui::ContourDialog *ui;
};

#endif // CONTOURDIALOG_H
