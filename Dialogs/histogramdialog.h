#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QDialog>

namespace Ui {
class HistogramDialog;
}

class HistogramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistogramDialog(QWidget *parent = 0);
    ~HistogramDialog();

signals:
    void sendHistogram(int numBins, bool showHistEqImg);

private slots:
    void on_showHistPushButton_clicked();

    void on_showHistEqImage_clicked(bool checked);

private:
    Ui::HistogramDialog *ui;
};

#endif // HISTOGRAMDIALOG_H
