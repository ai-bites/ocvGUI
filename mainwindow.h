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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_Open_triggered();

    void on_action_Close_triggered();

    void on_action_Save_triggered();

    void on_MorphComboBox_activated(const QString &value);

    void on_morphOkButton_clicked();

    void on_logoCheckBox_clicked();

    void on_logoSpinBox_editingFinished();

private:
    Ui::MainWindow *ui;
    ImgProcess * ip;
    void displayOp();
    bool isImgLoaded;

};

#endif // MAINWINDOW_H
