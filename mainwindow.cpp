#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user.h"
#include "modelcv.h"
#include <QDebug>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 切换 四个 大界面
void MainWindow::on_captureBtn_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(1);
}

void MainWindow::on_libraryBtn_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(2);
}

void MainWindow::on_detectionBtn_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(3);
}

void MainWindow::on_pushButton_4_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(4);
}

//返回 按钮1
void MainWindow::on_pushButton_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(0);
}

//录入后  拍完照的下一页
void MainWindow::on_pushButton_3_clicked()
{
    if(!facetr.empty())
        ui -> stackedWidget_2 -> setCurrentIndex(1);
    else
        qDebug() << "未录入人脸";
}

void MainWindow::on_caremaBtn_clicked()
{
    cv::VideoCapture cap(0);
    cv::Mat img;
    cap.read(img);
    facetr = img;
    cap.release();

    cv::CascadeClassifier faceCascade;
    faceCascade.load("D://cpp//OpenCV//opencv-4.5.4//opencv-4.5.4//data//haarcascades//haarcascade_frontalface_alt.xml");

    if (faceCascade.empty()) {
        qDebug()<< "未识别到人脸";
    }

    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(img, faces, 1.1, 3);

    for (size_t i = 0; i < faces.size(); i++)
    {
        cv::rectangle(img, faces[i].tl(), faces[i].br(), cv::Scalar(0,255,0), 3);
    }

    cv::imshow("your photo", img);
    cv::waitKey(0);
}

void MainWindow::on_phoneBtn_clicked()
{
    string path = "D://cppQtProgramme//photo2//pexels-justin-shaifer-501272-1222271.jpg";
    Mat img = imread(path);

    if (img.empty()) {
        cout << "Failed to load image: " << path << endl;
        return;
    }

    CascadeClassifier faceCascade;
    faceCascade.load("D://cpp//OpenCV//opencv-4.5.4//opencv-4.5.4//data//haarcascades//haarcascade_frontalface_alt.xml");

    if (faceCascade.empty()) {
        qDebug()<< "未识别到人脸";
        return;
    }

    vector<Rect> faces;
    faceCascade.detectMultiScale(img, faces, 1.1, 3);
    facetr = img;

    for (size_t i = 0; i < faces.size(); i++) {
        rectangle(img, faces[i].tl(), faces[i].br(), Scalar(0, 255, 0), 3);
    }
    imshow("fileFaces", img);
    waitKey(0);
}

void MainWindow::on_idEdit_cursorPositionChanged(int arg1, int arg2)
{

}

void MainWindow::on_nameEdit_cursorPositionChanged(int arg1, int arg2)
{

}

void MainWindow::on_genderBox_activated(const QString &arg1)
{

}

// 录入 完成按钮
void MainWindow::on_sureBtn_clicked()
{
    bool ok;
    int id = ui->idEdit->text().toInt(&ok);
    QString name = ui->nameEdit->text();
    bool gender = (ui->genderBox->currentIndex() == 0);
    int age = ui->ageEdit->text().toInt();

    if (!ok) {
        qDebug() << "未输入学号";
        return;
    }

    ui->idEdit->clear();
    ui->nameEdit->clear();
    ui->ageEdit->clear();

    User userSave(id, name.toStdString(), gender, age);

    userSave.saveUser(id);
    ModelCV cv;
    Mat inputFace = cv.process(facetr);
    cv.save2file(inputFace,id);

    qDebug() << "录入成功";
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_findBtn_clicked()
{
    bool ok;
    int id = ui->idInput->text().toInt(&ok);

    if (!ok) {
        qDebug() << "未输入学号";
        return;
    }

    User userFind(0,"wu",0,0);
    userFind.findUser(id);

    if (userFind.id == 0) {
        qDebug() << "查无此人";
        // 清空输入框
        ui->idInput->clear();
        return;
    }

    // 更新界面显示
    ui->stackedWidget_3->setCurrentIndex(1); // 切换到显示用户信息的页面

    // 设置用户信息显示
    ui->idShow->setText(QString::number(userFind.id));
    ui->nameShow->setText(QString::fromStdString(userFind.name));
    ui->ageShow->setText(QString::number(userFind.age));
    ui->genderShow->setText(userFind.gender ? "男" : "女"); // 假设使用中文显示，可以根据实际需要调整

    // 清空输入框
    ui->idInput->clear();
}

void MainWindow::on_deleteBtn_clicked()
{
    bool ok;
    int id = ui->idInput->text().toInt(&ok);

    if (!ok) {
        qDebug() << "未输入学号";
        return;
    }

    User userDelete(0,"wu",0,0);
    userDelete.deleteUser(id,1);

    qDebug() << "已删除";
    ui->idInput->clear();
}

// changeBtn
void MainWindow::on_pushButton_10_clicked()
{
    bool ok;
    int id = ui->idInput->text().toInt(&ok);

    if (!ok) {
        qDebug() << "未输入学号";
        return;
    }
    ui -> stackedWidget_3 -> setCurrentIndex(2);
}

void MainWindow::on_changeBtn2_clicked()
{
    int id = ui->idInput->text().toInt();
    QString name = ui->newName->text();
    bool gender = (ui->newGender->currentIndex() == 0);
    int age = ui->newAge->text().toInt();

    User userChange(id, name.toStdString(), gender, age);
    userChange.changeUser();

    qDebug() << "  修改成功";

    ui->idInput->clear();
    ui->newName->clear();
    ui->newGender->setCurrentIndex(0); // Assuming index 0 is Male, 1 is Female
    ui->newAge->clear();

    ui->stackedWidget_3->setCurrentIndex(0); // Go back to main view
}

void MainWindow::on_returnBtn3_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(0);
}

// 人脸识别 拍照按钮
void MainWindow::on_pushButton_13_clicked()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        qDebug() << "Error: Failed to open camera.";
        return;
    }

    cv::Mat img6;
    cap.read(img6);
    if (img6.empty()) {
        cout << "拍照失败" << endl;
        return;
    }

    ModelCV cv2;
    faceDetect = cv2.process(img6);

    cap.release();

    cv::imshow("ph2", faceDetect);

    cv::waitKey(0);
}

//人脸识别提交按钮
void MainWindow::on_submitBtn_clicked()
{
    if(!faceDetect.empty()){
        ModelCV cv;
        cv.compareWithReferenceFolder(faceDetect);
    }
    else
        qDebug() << "未录入人脸";
}

void MainWindow::on_returnBtn4_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(0);
}

void MainWindow::on_smileBtn_clicked()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        qDebug() << "Error: Failed to open camera.";
        return;
    }

    cv::Mat img4;
    cap.read(img4);
    cap.release();  // 释放摄像头对象
    ModelCV cv;
    cv.detectAndDisplay(img4);
    cv::imshow("your photo", img4);
    cv::waitKey(0);  // Wait for any key press
}

void MainWindow::on_sureBtn2_clicked()
{
    ui -> stackedWidget_3 -> setCurrentIndex(0);
    ui -> stackedWidget -> setCurrentIndex(0);
    ui-> idShow -> clear();
    ui -> nameShow -> clear();
    ui-> ageShow -> clear();
}

void MainWindow::on_changeBtn_clicked()
{
    bool ok;
    int id = ui->idInput->text().toInt(&ok);

    if (!ok) {
        qDebug() << "未输入学号";
        return;
    }
    ui -> stackedWidget_3 -> setCurrentIndex(2);
}


//人脸库管理返回按钮
void MainWindow::on_returnBtn2_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(0);
}
