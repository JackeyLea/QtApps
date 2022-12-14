///////////////////////////////////
/// \author jackey lea
/// \date 20220831
/// \note 向图片添加盲水印
///////////////////////////////////

#ifndef BLINDWATERMARK_H
#define BLINDWATERMARK_H

#include <QObject>
#include <QImage>
#include <QDebug>
#include <QColor>
#include <QPoint>

#include <vector>

#include "opencv4/opencv2/opencv.hpp"

class BlindWatermark : public QObject
{
    Q_OBJECT
public:
    explicit BlindWatermark(QObject *parent = nullptr);

    void setImage(QImage image);//需要添加盲水印的图片

    void setWatermarkCtx(QString s);//水印内容

    void setColor(QColor);
    void setPosition(QPoint);
    void setThickness(int);
    void setScale(float s);

    //////////////////////加水印部分////////////////////////////////////
    cv::Mat optimizeImageDim(cv::Mat image);//为加快傅里叶变换的速度，对要处理的图片尺寸进行优化

    cv::Mat splitSrc(cv::Mat mat);//图片通道分离

    cv::Mat antiTransformImage(cv::Mat complexImage, std::vector<cv::Mat> allPlanes);

    cv::Mat addImageWatermarkWithText(cv::Mat);//添加图片文字水印

    //////////////////////解水印部分////////////////////////////////////
    //Reorder the 4 quadrants of the image representing the magnitude, after the DFT
    void shiftDFT(cv::Mat &image);

    //Optimize the magnitude of the complex image obtained from the DFT, to
    //improve its visualization
    cv::Mat createOptimizedMagnitude(cv::Mat complexImage);

    cv::Mat getImageWatermarkWithText(cv::Mat image);
    ////////////////////////////////////////////////////

    //获取加水印的结果，图片是正常内容，但是里面包含水印
    cv::Mat getMatResult();
    QImage getQImageResult();

    //获取带水印的DFT图
    cv::Mat getMatWatermark();
    QImage getQImageWatermark();
    //测试结果
    QImage getQImageWatermark(QString filePath);

private:
    cv::Mat _mImg;
    cv::Scalar _mColor;
    cv::Point _mPos;
    int _mThickness;
    float _mScale;
    std::string _mWMCtx;
    std::vector<cv::Mat> planes;//通道
    std::vector<cv::Mat> allPlanes;
};

#endif // BLINDWATERMARK_H
