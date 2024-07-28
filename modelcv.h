#ifndef MODELCV_H
#define MODELCV_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <QDir>
#include <QDebug>
#include <opencv2/face.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

#include "user.h"

class ModelCV : public QObject
{
    Q_OBJECT
public:
    explicit ModelCV(QObject *parent = nullptr);

    void save2file(cv::Mat face, int id);
    cv::Mat process(cv::Mat face);
    void detectAndDisplay(cv::Mat &frame);

    double calculateSimilarity(const cv::Mat &image1, const cv::Mat &image2);
    int extractUserIdFromFileName(const std::string &filePath);
    void compareWithReferenceFolder(const cv::Mat &inputImage);
    std::vector<cv::Point2f> detectLandmarks(const cv::Mat& image);
signals:

private:

};

#endif // MODELCV_H
