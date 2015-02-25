#ifndef COLOURDIALOG_H
#define COLOURDIALOG_H

#include <QDialog>

namespace Ui {
class ColourDialog;
}

class ColourDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColourDialog(QWidget *parent = 0);
    ~ColourDialog();

signals:
    void sendColorVals(int);

private slots:
    void on_colorSpaceComboBox_currentIndexChanged(int index);

    void on_ColorDialogCloseButton_clicked();

private:
    Ui::ColourDialog *ui;
    int currentIdx;
};

#endif // COLOURDIALOG_H
