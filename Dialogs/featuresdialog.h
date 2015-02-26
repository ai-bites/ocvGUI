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

private:
    Ui::FeaturesDialog *ui;
};

#endif // FEATURESDIALOG_H
