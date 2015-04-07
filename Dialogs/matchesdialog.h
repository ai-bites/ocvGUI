#ifndef MATCHESDIALOG_H
#define MATCHESDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/stitching/stitcher.hpp>

using namespace cv;

namespace Ui {
class MatchesDialog;
}

class MatchesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MatchesDialog(QWidget *parent = 0);
    ~MatchesDialog();
    cv::Mat firstImg;
    cv::Mat secondImg;

signals:
    void sendMatchImages(cv::Mat, cv::Mat, bool, string);

private slots:
    void on_loadSecondImage_clicked();

    void on_showMatches_clicked();

    void on_loadFirstImage_clicked();

private:
    Ui::MatchesDialog *ui;

};

#endif // MATCHESDIALOG_H
