#ifndef CALIBRATEDIALOG_H
#define CALIBRATEDIALOG_H


#include <QDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextEdit>
#include <QString>
#include <QLabel>
#include <QMutex>
#include <QImage>
#include <QWaitCondition>
#include <iostream>
#include <QtWidgets/QtWidgets>
#include <QtCore>
#include <iostream>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <string>
#include "camera.h"

using namespace std;

namespace Ui {
class CalibrateDialog;
}

class CalibrateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrateDialog(QWidget *parent = 0);
    ~CalibrateDialog();

private slots:
    void on_openDirPushButton_clicked();

    void on_calibratePushButton_clicked();

private:
    Ui::CalibrateDialog *ui;
    Camera * cam;
    bool filesLoaded;
};

#endif // CALIBRATEDIALOG_H
