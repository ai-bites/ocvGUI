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


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow * getUI();

private slots:
    void on_action_Open_triggered();

    void on_action_Close_triggered();

    void on_action_Save_triggered();

    void on_MorphComboBox_activated(const QString &value);

private:
    Ui::MainWindow *ui;
    ImgProcess * ip;
    void displayOp();

};

#endif // MAINWINDOW_H
