/*
 * 功能：实现界面的鼠标绘图
 * 日期：2020-09-15
 * 作者：幽弥狂
 * 说明：黑色为底，画笔为白色
 */

#ifndef DRAWING_H
#define DRAWING_H

#include "global.hpp"

namespace Ui {
class Drawing;
}

class Drawing : public QWidget
{
    Q_OBJECT

public:
    explicit Drawing(QWidget *parent = nullptr);
    ~Drawing();

    QImage getImage();//获取当前画板的图片
    bool saveImage(QString filePath);//将画板中的图片保存下来
    void clearImage();//清空画板

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    Ui::Drawing *ui;

    QPixmap pix;//画板中的图片变量

    QPoint lastPoint;//鼠标上一次的位置
    QPoint endPoint;//鼠标当前的位置

    QPen pen;
    QFont font;
};

#endif // DRAWING_H
