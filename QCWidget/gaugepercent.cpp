#include "gaugepercent.h"

#include <QPainter>
#include <QtMath>

GaugePercent::GaugePercent(QWidget *parent)
    : QWidget{parent}
{
    bgColor=QColor(0x0f,0x2f,0x6e);
    startAngle=60;
    endAngle=60;
    minValue=0;
    maxValue=100;
    value=0;
    arcColor=QColor(0x00,0xfe,0xfe);
    scaleColor=QColor(0x0b,0x86,0xb2);
    scaleMajor=10;
    scaleMinor=5;
    scaleNumColor=QColor(0x00,0xfe,0xfe);
    precision=0;
    textColor=QColor(0x00,0xfe,0xfe);
    titleColor=QColor(0x00,0xfe,0xfe);
    title="测试值";
}

GaugePercent::~GaugePercent()
{

}

void GaugePercent::paintEvent(QPaintEvent *)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);

    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    //绘制背景
    if (bgColor != Qt::transparent) {
        painter.setPen(Qt::NoPen);
        painter.fillRect(this->rect(), bgColor);
    }

    painter.translate(width / 2, height / 2);
    painter.scale(side / 200.0, side / 200.0);

    //绘制圆弧
    drawArc(&painter);
    //绘制圆弧进度小球
    drawCircle(&painter);
    //绘制刻度线
    drawScale(&painter);
    //绘制刻度值
    drawScaleNum(&painter);
    //绘制当前值
    drawValue(&painter);
    //绘制标题
    drawTitle(&painter);
}

void GaugePercent::drawArc(QPainter *painter)
{
    int radius = 95;
    painter->save();
    painter->setBrush(Qt::NoBrush);

    QPen pen;
    pen.setWidthF(5);
    pen.setCapStyle(Qt::RoundCap);

    //计算总范围角度,当前值范围角度,剩余值范围角度
    double angleAll = 360.0 - startAngle - endAngle;
    double angleCurrent = angleAll * ((value - minValue) / (maxValue - minValue));
    double angleOther = angleAll - angleCurrent;
    QRectF rect = QRectF(-radius, -radius, radius * 2, radius * 2);

    //绘制当前进度圆弧
    pen.setColor(arcColor);
    painter->setPen(pen);
    painter->drawArc(rect, (270 - startAngle - angleCurrent) * 16, angleCurrent * 16);

    //绘制剩余进度圆弧
    pen.setColor(scaleColor);
    painter->setPen(pen);
    painter->drawArc(rect, (270 - startAngle - angleCurrent - angleOther) * 16, angleOther * 16);

    painter->restore();
}

void GaugePercent::drawCircle(QPainter *painter)
{
    int radius = 6;
    int offset = 83;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(arcColor);

    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (value - minValue);
    painter->rotate(degRotate);
    painter->drawEllipse(-radius, radius + offset, radius * 2, radius * 2);

    painter->restore();
}

void GaugePercent::drawScale(QPainter *painter)
{
    int radius = 90;
    painter->save();

    painter->rotate(startAngle);
    int steps = (scaleMajor * scaleMinor);
    double angleStep = (360.0 - startAngle - endAngle) / steps;

    QPen pen;
    pen.setColor(scaleColor);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(1.0);
    painter->setPen(pen);

    for (int i = 0; i <= steps; i++) {
        painter->drawLine(0, radius - 5, 0, radius);
        painter->rotate(angleStep);
    }

    painter->restore();
}

void GaugePercent::drawScaleNum(QPainter *painter)
{
    int radius = 75;
    painter->save();
    painter->setPen(scaleNumColor);

    double startRad = (360 - startAngle - 90) * (M_PI / 180);
    double deltaRad = (360 - startAngle - endAngle) * (M_PI / 180) / scaleMajor;

    for (int i = 0; i <= scaleMajor; i++) {
        double sina = qSin(startRad - i * deltaRad);
        double cosa = qCos(startRad - i * deltaRad);
        double value = 1.0 * i * ((maxValue - minValue) / scaleMajor) + minValue;

        QString strValue = QString("%1").arg((double)value, 0, 'f', precision);
        double textWidth = fontMetrics().horizontalAdvance(strValue);
        double textHeight = fontMetrics().height();
        int x = radius * cosa - textWidth / 2;
        int y = -radius * sina + textHeight / 4;
        painter->drawText(x, y, strValue);
    }

    painter->restore();
}

void GaugePercent::drawValue(QPainter *painter)
{
    int radius = 100;
    painter->save();
    painter->setPen(textColor);

    QFont font;
    font.setPixelSize(30);
    painter->setFont(font);

    QRectF textRect(-radius, 0, radius * 2, radius / 3);
    QString strValue = QString("%1%").arg((double)value, 0, 'f', precision);
    painter->drawText(textRect, Qt::AlignCenter, strValue);

    painter->restore();
}

void GaugePercent::drawTitle(QPainter *painter)
{
    int radius = 100;
    painter->save();
    painter->setPen(titleColor);

    QFont font;
    font.setPixelSize(20);
    painter->setFont(font);

    QRectF textRect(-radius, radius / 3, radius * 2, radius / 3);
    painter->drawText(textRect, Qt::AlignCenter, title);

    painter->restore();
}

double GaugePercent::getMinValue() const
{
    return minValue;
}

double GaugePercent::getMaxValue() const
{
    return maxValue;
}

double GaugePercent::getValue() const
{
    return value;
}

int GaugePercent::getPrecision() const
{
    return precision;
}

int GaugePercent::getScaleMajor() const
{
    return scaleMajor;
}

int GaugePercent::getScaleMinor() const
{
    return scaleMinor;
}

int GaugePercent::getStartAngle() const
{
    return startAngle;
}

int GaugePercent::getEndAngle() const
{
    return endAngle;
}

QColor GaugePercent::getArcColor() const
{
    return arcColor;
}

QColor GaugePercent::getScaleColor() const
{
    return scaleColor;
}

QColor GaugePercent::getScaleNumColor() const
{
    return scaleNumColor;
}

QColor GaugePercent::getTextColor() const
{
    return textColor;
}

QColor GaugePercent::getTitleColor() const
{
    return titleColor;
}

QColor GaugePercent::getBgColor() const
{
    return bgColor;
}

QString GaugePercent::getTitle() const
{
    return title;
}

QSize GaugePercent::sizeHint() const
{
    return size();
}

QSize GaugePercent::minimumSizeHint() const
{
    return minimumSize();
}

void GaugePercent::setRange(double minValue, double maxValue)
{
    this->minValue=minValue;
    this->maxValue=maxValue;
    update();
}

void GaugePercent::setRange(int minValue, int maxValue)
{
    this->minValue=minValue;
    this->maxValue=maxValue;
    update();
}

void GaugePercent::setMinValue(double minValue)
{
    this->minValue=minValue;
    update();
}

void GaugePercent::setMaxValue(double maxValue)
{
    this->maxValue=maxValue;
    update();
}

void GaugePercent::setValue(double value)
{
    this->value=value;
    update();
}

void GaugePercent::setValue(int value)
{
    this->value=value;
    update();
}

void GaugePercent::setPrecision(int precision)
{
    this->precision=precision;
    update();
}

void GaugePercent::setScaleMajor(int scaleMajor)
{
    this->scaleMajor=scaleMajor;
    update();
}

void GaugePercent::setScaleMinor(int scaleMinor)
{
    this->scaleMinor=scaleMinor;
    update();
}

void GaugePercent::setStartAngle(int startAngle)
{
    this->startAngle=startAngle;
    update();
}

void GaugePercent::setEndAngle(int endAngle)
{
    this->endAngle=endAngle;
    update();
}

void GaugePercent::setArcColor(const QColor &arcColor)
{
    this->arcColor=arcColor;
    update();
}

void GaugePercent::setScaleColor(const QColor &scaleColor)
{
    this->scaleColor=scaleColor;
    update();
}

void GaugePercent::setScaleNumColor(const QColor &scaleNumColor)
{
    this->scaleNumColor=scaleNumColor;
    update();
}

void GaugePercent::setTextColor(const QColor &textColor)
{
    this->textColor=textColor;
    update();
}

void GaugePercent::setTitleColor(const QColor &titleColor)
{
    this->titleColor=titleColor;
    update();
}

void GaugePercent::setBgColor(const QColor &bgColor)
{
    this->bgColor=bgColor;
    update();
}

void GaugePercent::setTitle(const QString &title)
{
    this->title=title;
    update();
}
