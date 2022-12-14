#include "blindwatermark.h"

QImage cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

cv::Mat QImage2cvMat(QImage image)
{
    cv::Mat mat;
    qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

///////////////////////////////////////

BlindWatermark::BlindWatermark(QObject *parent)
    : _mPos(250,250)
    , _mColor(0,0,0)
    , _mThickness(2)
    , _mScale(1.0)
    , _mWMCtx("blog.jackeylea.com")
{
    allPlanes.clear();
    planes.clear();
}

void BlindWatermark::setImage(QImage image)
{
    _mImg.release();
    _mImg = QImage2cvMat(image);
}

void BlindWatermark::setWatermarkCtx(QString s)
{
    _mWMCtx = s.toStdString();
}

void BlindWatermark::setColor(QColor c)
{
    _mColor = cv::Scalar(c.red(),c.green(),c.blue());
}

void BlindWatermark::setPosition(QPoint pos)
{
    _mPos = cv::Point(pos.x(),pos.y());
}

void BlindWatermark::setThickness(int t)
{
    _mThickness = t;
}

void BlindWatermark::setScale(float s)
{
    _mScale = s;
}

//为加快傅里叶变换的速度，对要处理的图片尺寸进行优化
cv::Mat BlindWatermark::optimizeImageDim(cv::Mat image)
{
    cv::Mat padded;
    //get the optimal rows size for dft
    int addPixelRows = cv::getOptimalDFTSize(image.rows);
    //get the optimal cols size for dft
    int addPixelCols = cv::getOptimalDFTSize(image.cols);
    //apply the optimal cols and rows size to the image
    cv::copyMakeBorder(image,padded,0,addPixelRows - image.rows,0,addPixelCols - image.cols,cv::BORDER_CONSTANT,cv::Scalar::all(0));

    return padded;
}

//图片通道分离
cv::Mat BlindWatermark::splitSrc(cv::Mat mat)
{
    allPlanes.clear();
    mat = optimizeImageDim(mat);
    cv::split(mat,allPlanes);
    cv::Mat padded;
    if(allPlanes.size()>1){
        padded = allPlanes.at(0);
    }else{
        padded = mat;
    }
    return padded;
}

//把DFT灰度图转换为普通图
cv::Mat BlindWatermark::antiTransformImage(cv::Mat complexImage, std::vector<cv::Mat> allPlanes)
{
    cv::Mat invDFT;
    cv::idft(complexImage,invDFT,cv::DFT_SCALE | cv::DFT_REAL_OUTPUT,0);
    cv::Mat restoredImage;
    invDFT.convertTo(restoredImage,CV_8U);
    if(allPlanes.size()==0){
        allPlanes.push_back(restoredImage);
    }else{
        auto iter = allPlanes.begin();
        *iter = restoredImage;
    }
    cv::Mat lastImage;
    cv::merge(allPlanes,lastImage);
    allPlanes.clear();
    planes.clear();//连续多次操作时防止奔溃

    return lastImage;
}

//添加文字水印
cv::Mat BlindWatermark::addImageWatermarkWithText(cv::Mat image)
{
    cv::Mat complexImage;
    //优化图像尺寸
    cv::Mat padded = splitSrc(image);
    padded.convertTo(padded,CV_32F);
    planes.push_back(padded);
    planes.push_back(cv::Mat::zeros(padded.size(),CV_32F));

    cv::merge(planes,complexImage);
    //dft
    cv::dft(complexImage,complexImage);
    //添加文本水印//FONT_HERSHEY_COMPLEX FONT_HERSHEY_TRIPLEX
    cv::putText(complexImage,_mWMCtx,_mPos,cv::FONT_HERSHEY_COMPLEX,_mScale,_mColor,_mThickness);
    cv::flip(complexImage,complexImage,-1);
    cv::putText(complexImage,_mWMCtx,_mPos,cv::FONT_HERSHEY_COMPLEX,_mScale,_mColor,_mThickness);
    cv::flip(complexImage,complexImage,-1);

    return antiTransformImage(complexImage,allPlanes);
}

//交换四个向限内容
void BlindWatermark::shiftDFT(cv::Mat &image)
{
    image = cv::Mat(image,cv::Rect(0,0,image.cols & -2,image.rows & -2));
    int cx = image.cols /2;
    int cy = image.rows /2;

    cv::Mat q0 = cv::Mat(image,cv::Rect(0,0,cx,cy));
    cv::Mat q1 = cv::Mat(image,cv::Rect(cx,0,cx,cy));
    cv::Mat q2 = cv::Mat(image,cv::Rect(0,cy,cx,cy));
    cv::Mat q3 = cv::Mat(image,cv::Rect(cx,cy,cx,cy));

    cv::Mat tmp;

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

cv::Mat BlindWatermark::createOptimizedMagnitude(cv::Mat complexImage)
{
    std::vector<cv::Mat> newPlanes;
    cv::Mat mag;

    //split the comples image in two planes
    cv::split(complexImage,newPlanes);
    //compute the magnitude
    cv::magnitude(newPlanes.at(0),newPlanes.at(1),mag);

    //move to a logarithmic scale
    cv::add(cv::Mat::ones(mag.size(),CV_32F),mag,mag);
    cv::log(mag,mag);
    //optionally reorder the 4 quadrants of the magnitude image
    shiftDFT(mag);
    // normalize the magnitude image for the visualization since both JavaFX
    // and OpenCV need images with value between 0 and 255
    // convert back to CV_8UC1
    mag.convertTo(mag,CV_8UC1);
    cv::normalize(mag,mag,0,255,cv::NORM_MINMAX,CV_8UC1);

    return mag;
}

cv::Mat BlindWatermark::getImageWatermarkWithText(cv::Mat image)
{
    std::vector<cv::Mat> planes;
    cv::Mat complexImage;
    cv::Mat padded = splitSrc(image);
    padded.convertTo(padded, CV_32F);
    planes.push_back(padded);
    planes.push_back(cv::Mat::zeros(padded.size(), CV_32F));
    cv::merge(planes, complexImage);
    // dft
    cv::dft(complexImage, complexImage);
    cv::Mat magnitude = createOptimizedMagnitude(complexImage);
    planes.clear();
    return magnitude;
}

cv::Mat BlindWatermark::getMatResult()
{
    cv::Mat r;
    if(!_mImg.empty()){
        r = addImageWatermarkWithText(_mImg);//此时获取到的是带盲水印的正常图
    }
    return r;
}

QImage BlindWatermark::getQImageResult()
{
    cv::Mat t = getMatResult();
    QImage r;
    if(!t.empty()){
        r = cvMat2QImage(t);
    }
    return r;
}

cv::Mat BlindWatermark::getMatWatermark()
{
    cv::Mat tmpMat = getMatResult();//带盲水印图
    return getImageWatermarkWithText(tmpMat);//此时获取到的是带水印的四向限图
}

QImage BlindWatermark::getQImageWatermark()
{
    cv::Mat tmpMat = getMatWatermark();
    QImage r;
    if(!tmpMat.empty()){
        r = cvMat2QImage(tmpMat);
    }
    return r;
}

QImage BlindWatermark::getQImageWatermark(QString filePath)
{
    cv::Mat tmpMat = cv::imread(filePath.toStdString());
    cv::Mat tmpMat2 = getImageWatermarkWithText(tmpMat);
    QImage r;
    if(!tmpMat2.empty()){
        r = cvMat2QImage(tmpMat2);
    }
    return r;
}

