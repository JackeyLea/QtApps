#include "normalwatermark.h"

NormalWatermark::NormalWatermark()
    : _mColor(Qt::white)
    , _mPosX(0)
    , _mPosY(0)
    , _mPosW(0)
    , _mPosH(0)
{}

void NormalWatermark::setColor(QColor color)
{
    _mColor = color;
}

void NormalWatermark::setFontPointSize(int s)
{
    _mPointSize = s;
}

void NormalWatermark::setFrameVisible(bool v)
{
    _mFrameVisible = v;
}

void NormalWatermark::setImage(QImage img)
{
    _mImg = img;
}

void NormalWatermark::setWatermarkCtx(QString wm)
{
    _mWatermarkCtx = wm;
}

void NormalWatermark::setPosX(int x)
{
    _mPosX = x;
}

void NormalWatermark::setPosY(int y)
{
    _mPosY = y;
}

void NormalWatermark::setPosW(int w)
{
    _mPosW = w;
}

void NormalWatermark::setPosH(int h)
{
    _mPosH = h;
}

QImage NormalWatermark::result()
{
    //水印位置
    QRect rect(_mPosX,_mPosY,_mPosW,_mPosH);

    //水印内容
    if(_mWatermarkCtx.isEmpty()){
        return QImage();
    }

    QFont font;
    font.setPointSize(_mPointSize);//字体大小
    QPen pen;
    pen.setColor(_mColor);

    //如果待处理图片为空
    if(_mImg.isNull()){
        return QImage();
    }

    QImage tmpImg = _mImg;

    QPainter painter(&tmpImg);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);
    painter.setFont(font);
    if(_mFrameVisible){
        painter.drawRect(rect);
    }
    painter.drawText(_mPosX,_mPosY,_mPosW,_mPosH,Qt::AlignCenter,_mWatermarkCtx);

    return tmpImg;
}
