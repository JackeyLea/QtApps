/*
 * 2020-09-15
 * gloabl function for all
 */

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <QString>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QResizeEvent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QWidget>
#include <QThread>
#include <QFrame>
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QIcon>
#include <QLabel>
#include <QUrl>
#include <QPlainTextEdit>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>

using namespace cv;
using namespace cv::ml;
using namespace std;

//加载主题文件
inline QString loadTheme(QString themeName){
    QString themeCtx;
    QFile file(QString(":/resources/themes/%1.qss").arg(themeName));
    if(file.open(QIODevice::Text|QIODevice::ReadOnly)){
        QTextStream readIn(&file);
        themeCtx = readIn.readAll();
    }else{
        qDebug()<<"Cannot open file: "<<file.errorString();
    }
    file.close();

    return themeCtx;
}
//cv::Mat转换为QImage
inline QImage toQImage(Mat img){
    QImage qimg=QImage(static_cast<unsigned char*>(img.data),
                       img.cols,img.rows,img.cols*img.channels(),QImage::Format_RGB888);
    return qimg;
}
//QImage转换为cv::Mat
inline Mat toMat(QImage image){
    Mat mat;
    if(image.format() == QImage::Format_ARGB32 || image.format() == QImage::Format_RGB32||image.format() == QImage::Format_ARGB32_Premultiplied){
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
    }else if(image.format() == QImage::Format_RGB888){
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, COLOR_BGR2RGB);
    }else if(image.format()== QImage::Format_Indexed8){
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
    }
    return mat;
}
//将图片处理为灰度图
inline cv::Mat getGrayImg(cv::Mat img) {
    cv::Mat grayImg;
    if(img.channels() ==4){
        cv::cvtColor(img,grayImg,cv::COLOR_BGRA2GRAY);
    }else if (img.channels() == 3) {
        cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
    } else {
        grayImg = img;
    }

    return grayImg;
}
//灰度图二值化
inline cv::Mat getBinImg(cv::Mat grayImg, int threholdValue=-1) {
    cv::Mat result;
    int     middle = 0;

    if (threholdValue == -1) {
        uchar *       p;
        int           nrows = grayImg.rows;
        int           ncols = grayImg.cols;
        long long int count = 0;
        int           mid   = 0;
        //获取并计算灰度值的平均值
        for (int i = 0; i < nrows; i++) {
            p = grayImg.ptr<uchar>(i);
            for (int j = 0; j < ncols; j++) {
                mid += p[ j ];
                count++;
            }
        }
        middle = int((mid / count) * 2 / 3); //
    } else {
        middle = threholdValue;
    }
    //qDebug() << "Average pixel is: " << middle;

    //以平均值二值化图片
    threshold(grayImg, result, middle, 255, cv::THRESH_BINARY);

    //如果黑色像素点较多则翻转图片像素
    int white = 0;
    int black = 0;
    for (int i = 0; i < result.rows; i++) {
        uchar *ptr = result.ptr(i);
        for (int j = 0; j < result.cols; j++) {
            if (ptr[ j ] == 0)
                black++;
            else
                white++;
        }
    }

    return result;
}

#endif // GLOBAL_HPP
