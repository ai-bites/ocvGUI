#include "mainwindow.h"
#include <QApplication>
#include "camera.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    //w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
