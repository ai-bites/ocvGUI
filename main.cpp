#include "mainwindow.h"
#include <QApplication>
#include "camera.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    //w.setWindowState(Qt::WindowMaximized);
    w.show();
    //ImgProcess * ip = new ImgProcess;
    //ip->computeHomography();
    //ip->computeFundMatrix(1);

    return a.exec();
}
