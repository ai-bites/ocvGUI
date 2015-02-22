#ifndef MORPHOLOGYDIALOG_H
#define MORPHOLOGYDIALOG_H

#include <QDialog>
#include "mainwindow.h"

#include <qsignaltransition.h>
#include <qsignalmapper.h>

namespace Ui {
class MorphologyDialog;
}

class MorphologyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MorphologyDialog(QWidget *parent = 0);
    ~MorphologyDialog();
    QString currentOption;

signals:
    void sendMorphSignal(QString str, int h, int w);

private slots:
    void on_morphOkButton_accepted();

    void on_MorphHeightspinBox_valueChanged(int h);

    void on_MorphWidthSpinBox_valueChanged(int w);

private:
    Ui::MorphologyDialog *ui;
};

#endif // MORPHOLOGYDIALOG_H
