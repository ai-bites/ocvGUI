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
//    Mat first = cv::imread("/Users/shreya/Desktop/test_images/tools_1.bmp");
//    Mat second = cv::imread("/Users/shreya/Desktop/test_images/tools_2");

//    ip->doEpipolarLines(first, second, true, 3);

    return a.exec();
}
