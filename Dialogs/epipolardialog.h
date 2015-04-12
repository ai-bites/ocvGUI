#ifndef EPIPOLARDIALOG_H
#define EPIPOLARDIALOG_H

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
class EpipolarDialog;
}

class EpipolarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EpipolarDialog(QWidget *parent = 0);
    ~EpipolarDialog();
    cv::Mat firstImg;
    cv::Mat secondImg;

signals:
    void sendEpipolarImages(Mat, Mat, bool, string, int, bool, bool, double, double, double, int);

private slots:
    void on_loadFirstImgPushButton_clicked();

    void on_loadSecondImgPushButton_clicked();

    void on_closePushButton_clicked();

    void on_showEpiLinesPushButton_clicked();

private:
    Ui::EpipolarDialog *ui;
};

#endif // EPIPOLARDIALOG_H
