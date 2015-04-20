#include "calibratedialog.h"
#include "ui_calibratedialog.h"


using namespace std;



CalibrateDialog::CalibrateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrateDialog)
{
    ui->setupUi(this);
    this->filesLoaded = false;
}

CalibrateDialog::~CalibrateDialog()
{
    delete ui;
}

void CalibrateDialog::on_openDirPushButton_clicked()
{
    QString d = QFileDialog::getExistingDirectory(this, QString(), QString());

    std::vector<std::string> files;
    struct dirent *dirp;
    struct stat;
    string filepath;

    string dir = d.toStdString();
    DIR * dp = opendir( dir.c_str() );

    if (dp == NULL)
    {
        cout << "error opening dir" << endl;

    }
    this->cam = new Camera;
    this->filesLoaded = true;

    while ((dirp = readdir( dp )))
    {
        filepath = dir + "/" + dirp->d_name;
        cam->inputFiles.push_back(filepath);
        cout << "adding file :" << String(filepath) << "to path" << endl;
    }
    cout << "done with while loop" <<endl;

    QMessageBox messageBox;
    messageBox.information(0,"Success","Successfully added files for calibration !");
    messageBox.setFixedSize(500,200);
    return;
}

void CalibrateDialog::on_calibratePushButton_clicked()
{
    cout << "in calib push" << endl;
    if (this->filesLoaded == false)
    {
        QMessageBox messageBox;
        messageBox.warning(0,"Message","Please add files for calibrating first!");
        messageBox.setFixedSize(500,200);
        return;
    }

    this->cam->calibrate();

}
