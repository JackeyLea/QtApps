///////////////////////////////////
/// \author jackey lea
/// \date 20220831
/// \note 向图片添加盲水印
///////////////////////////////////

#ifndef NORMALWATERMARK_H
#define NORMALWATERMARK_H

#include <QObject>
#include <QColor>
#include <QFont>
#include <QPen>
#include <QImage>
#include <QPainter>

class NormalWatermark
{
public:
    NormalWatermark();

    void setColor(QColor color);//设置水印颜色

    void setFontPointSize(int s);//设置字体大小

    void setFrameVisible(bool v);//设置边框是否显示

    void setImage(QImage img);//设置待处理图片

    void setWatermarkCtx(QString wm);//水印内容

    void setPosX(int x);//水印坐标
    void setPosY(int y);//水印坐标
    void setPosW(int w);//水印宽度
    void setPosH(int h);//水印高度

    QImage result();//获取添加水印后的结果

private:
    QColor _mColor;

    bool _mFrameVisible;

    QImage _mImg;//待处理图片

    int _mPointSize;

    QString _mWatermarkCtx;

    int _mPosX;
    int _mPosY;
    int _mPosW;
    int _mPosH;
};

#endif // NORMALWATERMARK_H
