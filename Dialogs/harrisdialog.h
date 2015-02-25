#ifndef HARRISDIALOG_H
#define HARRISDIALOG_H

#include <QDialog>

namespace Ui {
class HarrisDialog;
}

class HarrisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HarrisDialog(QWidget *parent = 0);
    ~HarrisDialog();

private:
    Ui::HarrisDialog *ui;
};

#endif // HARRISDIALOG_H
