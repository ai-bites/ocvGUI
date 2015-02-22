#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QSlider>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextEdit>
#include <QString>
#include <QLabel>
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <qmediaplayer.h>
#include <qmediaplaylist.h>
#include <iostream>
#include <QtMultimedia/qmediaplayer.h>
#include <QtWidgets/QtWidgets>
#include <QVideoWidget>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "imgprocess.h"
#include "videoprocess.h"
#include "morphologydialog.h"
#include "noisedialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool isImgLoaded;
    ImgProcess * ip;
    VideoProcess * vp;

private slots:
    // all actions triggered from tools menu
    void on_action_Open_triggered();
    void on_action_Close_triggered();
    void on_action_Save_triggered();
    void on_actionMorphology_triggered();
    void on_actionAdd_Noise_triggered();

    // All handlers for signals emitted from dialog boxes
    void handleMorphSignal(QString choice, int h, int w);
    void handleSnPNoiseSignal(QString name, int n);
    void handleImageOpen();
    void handleVideoOpen();

    void on_pushButton_clicked();

    void updateVideoImage(QImage);

private:
    Ui::MainWindow *ui;
    void displayOp();

signals:
    void sendVideoImage(QImage);

};

#endif // MAINWINDOW_H
