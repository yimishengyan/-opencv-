#include "modelcv.h"

using namespace cv;
using namespace std;
//"D://cpp//OpenCV//opencv-4.5.4//opencv-4.5.4//data//haarcascades//haarcascade_frontalface_alt.xml"

ModelCV::ModelCV(QObject *parent) : QObject(parent)
{

}

 void ModelCV::save2file(cv::Mat face, int id) {

    // 保存切割后的人脸图像到data文件夹
    QString grayImagePath = "D://cppQtProgramme//User//UserImg//num" + QString::number(id) + ".jpg";
    cv::imwrite(grayImagePath.toStdString(), face);
}

 cv::Mat ModelCV::process(cv::Mat face) {
     // 转换为灰度图像
     cv::Mat grayFace;
     cvtColor(face, grayFace, cv::COLOR_BGR2GRAY);

     // 加载人脸检测器
     cv::CascadeClassifier faceDetector;
     faceDetector.load("D://cpp//OpenCV//opencv-4.5.4//opencv-4.5.4//data//haarcascades//haarcascade_frontalface_alt.xml");

     // 检测人脸
     std::vector<cv::Rect> faces;
     faceDetector.detectMultiScale(grayFace, faces);

     // 切割出第一个人脸（假设我们只关心最大的一张脸）
     cv::Mat faceROI = grayFace(faces[0]);

     // 定义目标尺寸（假设统一调整为 300x300）
     cv::Size targetSize(300, 300);

     // 调整输入图像的尺寸
     cv::Mat resizedFace;
     cv::resize(faceROI, resizedFace, targetSize);

     return resizedFace;  // 返回灰度图像
 }

 double ModelCV::calculateSimilarity(const cv::Mat &image1, const cv::Mat &image2) {
     cv::Mat diff;
     cv::absdiff(image1, image2, diff);
     diff.convertTo(diff, CV_32F);  // 转换为浮点数

     double mse = cv::mean(diff.mul(diff))[0];  // MSE
     double similarity = 1.0 / (1.0 + mse);     // 相似度评分

     return similarity;
 }

 int ModelCV::extractUserIdFromFileName(const std::string &filePath) {
     // 文件名格式示例："D:/cppQtProgramme/User/UserImg/num1001.jpg"
     std::string fileName = filePath.substr(filePath.find_last_of("/\\") + 1);  // 提取文件名部分
     fileName = fileName.substr(3, fileName.find_last_of(".") - 3);  // 提取数字部分，去掉前缀"num"和后缀".jpg"

     int userId = std::stoi(fileName);  // 转换为整数作为用户ID

     return userId;
 }

std::vector<cv::Point2f> ModelCV::detectLandmarks(const cv::Mat& image) {
     std::vector<cv::Point2f> keypoints;

    // 创建ORB特征点检测器
    cv::Ptr<cv::Feature2D> orb = cv::ORB::create();

    // 检测特征点
    std::vector<cv::KeyPoint> orbKeypoints;
    orb->detect(image, orbKeypoints);

    // 提取特征点的坐标
    for (const auto& kp : orbKeypoints) {
        keypoints.push_back(kp.pt);
    }

    return keypoints;
}

void ModelCV::compareWithReferenceFolder(const cv::Mat &inputImage) {
    // 初始化人脸检测器和特征点检测器
    cv::CascadeClassifier faceDetector;
    if (!faceDetector.load("D:/cpp/OpenCV/opencv-4.5.4/opencv-4.5.4/data/haarcascades/haarcascade_frontalface_alt.xml")) {
        std::cerr << "Error: Unable to load face cascade classifier." << std::endl;
        return;
    }

    // 在输入图像中检测人脸
    std::vector<cv::Rect> faces;
    faceDetector.detectMultiScale(inputImage, faces);

    // 对每张检测到的人脸进行处理
    for (const auto &faceRect : faces) {
        // 加载参考图像文件夹中的所有图片
        std::vector<std::string> imageFiles;
        cv::glob("D:/cppQtProgramme/User/UserImg/num*.jpg", imageFiles);

        bool foundMatch = false;
        for (const auto &filePath : imageFiles) {
            // 加载参考图像
            cv::Mat referenceImage = cv::imread(filePath, cv::IMREAD_GRAYSCALE);
            if (referenceImage.empty()) {
                std::cerr << "Error: Could not read reference image " << filePath << std::endl;
                continue;
            }

            // 调整输入图像中的人脸区域大小以匹配参考图像的大小
            cv::Mat faceROI_resized;
            cv::resize(inputImage(faceRect), faceROI_resized, referenceImage.size());

            // 计算输入图像人脸区域与参考图像的相似度评分
            double similarityScore = calculateSimilarity(faceROI_resized, referenceImage);

            // 从文件名中提取用户ID
            int userId = extractUserIdFromFileName(filePath);

            // 根据相似度评分进行处理
            if (similarityScore > 0.000226) {
                foundMatch = true;

                qDebug() <<userId <<endl;
                qDebug() <<similarityScore<<endl;

                // 在输入图像上写入用户ID和相似度评分信息
                std::string text = "ID: " + std::to_string(userId) + ", Score: " + std::to_string(similarityScore);
                int textWidth = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, nullptr).width;
                cv::putText(inputImage, text, cv::Point(faceRect.x + (faceRect.width - textWidth) / 2, faceRect.y - 10),
                            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);

                // 画出特征点
                std::vector<cv::Point2f> landmarks = detectLandmarks(faceROI_resized);
                for (const auto &landmark : landmarks) {
                    cv::circle(inputImage, cv::Point(faceRect.x + landmark.x, faceRect.y + landmark.y), 1, cv::Scalar(0, 255, 0), 2);
                }

                break;  // 找到匹配，不需要继续查找
            }
        }

        // 如果未找到匹配，写入 "notFound"
        if (!foundMatch) {
            int textWidth = cv::getTextSize("notFound", cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, nullptr).width;
            cv::putText(inputImage, "notFound", cv::Point(faceRect.x + (faceRect.width - textWidth) / 2, faceRect.y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
            qDebug() <<"notFound" <<endl;
        }
    }

    // 在窗口中显示结果图像
    cv::imshow("Result", inputImage);
    cv::waitKey(0);  // 等待用户按键退出
}


 void ModelCV::detectAndDisplay(cv::Mat& frame){
     // 加载人脸检测器
     cv::CascadeClassifier face_cascade;
     if (!face_cascade.load("D://cpp//OpenCV//opencv-4.5.4//opencv-4.5.4//data//haarcascades//haarcascade_frontalface_alt.xml")) {
         qDebug() << "Error: Unable to load face cascade classifier.";
         return;
     }

     // 加载笑脸检测器
     cv::CascadeClassifier smile_cascade;
     if (!smile_cascade.load("D://cpp//OpenCV//opencv-4.5.4//opencv-4.5.4//data//haarcascades//haarcascade_smile.xml")) {
         qDebug() << "Error: Unable to load smile cascade classifier.";
         return;
     }

     // 将图像转换为灰度图像，这是人脸检测器所需的格式
     cv::Mat gray;
     cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
     cv::equalizeHist(gray, gray);

     // 检测人脸
     std::vector<cv::Rect> faces;
     face_cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

     // 处理每张检测到的人脸
     for (size_t i = 0; i < faces.size(); ++i) {
         cv::Rect faceROI = faces[i];

         // 在人脸周围画矩形框
         cv::rectangle(frame, faceROI, cv::Scalar(255, 0, 0), 2);

         // 检测笑脸
         cv::Mat face_gray = gray(faceROI);
         std::vector<cv::Rect> smiles;
         smile_cascade.detectMultiScale(face_gray, smiles, 1.8, 20, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(20, 20));

         // 判断是否检测到笑脸
         if (smiles.size() > 0) {
             // 在人脸上写笑容
             cv::putText(frame, "smile", cv::Point(faceROI.x, faceROI.y - 10), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
         } else {
             // 在人脸上写无笑容
             cv::putText(frame, "noSmile", cv::Point(faceROI.x, faceROI.y - 10), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 2);
         }
     }
 }
