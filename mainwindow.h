#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/core.hpp>
#include <QTimer>

#include "user.h"
#include "modelcv.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void on_pushButtonSubmit_clicked();

    cv::Mat facetr;
    cv::Mat faceDetect;

private slots:
    void on_captureBtn_clicked();

    void on_libraryBtn_clicked();

    void on_detectionBtn_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_caremaBtn_clicked();

    void on_phoneBtn_clicked();

    void on_idEdit_cursorPositionChanged(int arg1, int arg2);

    void on_nameEdit_cursorPositionChanged(int arg1, int arg2);

    void on_genderBox_activated(const QString &arg1);

    void on_sureBtn_clicked();

    void on_findBtn_clicked();

    void on_deleteBtn_clicked();

    void on_pushButton_10_clicked();

    void on_changeBtn2_clicked();

    void on_returnBtn3_clicked();

    void on_pushButton_13_clicked();

    void on_submitBtn_clicked();

    void on_returnBtn4_clicked();

    void on_smileBtn_clicked();

    void on_sureBtn2_clicked();

    void on_changeBtn_clicked();

    void on_returnBtn2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
