#include "drawing.h"
#include "ui_drawing.h"

Drawing::Drawing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Drawing)
{
    ui->setupUi(this);
    //如果初始值为正值，会在界面绘制一个白色的点
    lastPoint=endPoint=QPoint(-20,-20);

    pix = QPixmap(this->width(),this->height());
    pix.fill(Qt::black);//填充图片的黑色背景

    pen.setColor(Qt::white);
    pen.setWidth(24);

    font.setWeight(5);
    font.setPixelSize(2);
}

Drawing::~Drawing()
{
    delete ui;
}

QImage Drawing::getImage()
{
    return pix.toImage();
}

bool Drawing::saveImage(QString filePath)
{
    return pix.save(filePath);
}
//清空界面图片，以黑色填充
void Drawing::clearImage()
{
    lastPoint=endPoint=QPoint(-20,-20);
    pix.fill(Qt::black);
    update();
}
//绘图
void Drawing::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    //先绘制在图片上
    QPainter pp(&pix);
    pp.setPen(pen);
    pp.setFont(font);
    pp.drawLine(lastPoint,endPoint);
    pp.setRenderHint(QPainter::HighQualityAntialiasing,true);

    lastPoint = endPoint;
    //再绘制再界面上
    QPainter painter(this);
    painter.drawPixmap(0,0,pix);
}
//鼠标按下
void Drawing::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
    }
    endPoint = lastPoint;
}
//鼠标松开
void Drawing::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        endPoint = event->pos();
        update();
    }
}
//鼠标移动
void Drawing::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        endPoint = event->pos();
        update();
    }
}
//如果界面的尺寸发生了调整
void Drawing::resizeEvent(QResizeEvent *event)
{
    pix = QPixmap(event->size());
    pix.fill(Qt::black);
    update();
}
