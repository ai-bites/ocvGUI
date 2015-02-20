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

#include <iostream>
#include "imgprocess.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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

private slots:
    // all actions triggered from tools menu
    void on_action_Open_triggered();
    void on_action_Close_triggered();
    void on_action_Save_triggered();
    void on_actionMorphology_triggered();

    void on_logoCheckBox_clicked();
    void on_logoSpinBox_editingFinished();

    // All handlers for signals emitted from dialog boxes
    void handleMorphSignal(QString choice, int h, int w);
    void handleSnPNoiseSignal(QString name, int n);
    void handleImageOpen();

    void on_actionAdd_Noise_triggered();

private:
    Ui::MainWindow *ui;
    void displayOp();

};

#endif // MAINWINDOW_H
