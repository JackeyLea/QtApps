#include "gaugecar.h"

#include <QPainter>
#include <QtMath>
#include <QPainterPath>

GaugeCar::GaugeCar(QWidget *parent)
    : QWidget(parent)
{
    outerCircleColor=QColor(0x50,0x50,0x50);
    innerCircleColor=QColor(0x40,0x40,0x40);

    pieStyle = PieStyle_Three;
    pieColorStart=QColor(0x17,0xbb,0x99);
    pieColorMid=QColor(0xd8,0xd8,0x00);
    pieColorEnd=QColor(0xfd,0x69,0x69);
    startAngle=60;
    endAngle=60;

    coverCircleColor=QColor(0x64,0x64,0x64);

    scaleMajor=10;
    scaleMinor=10;
    scaleColor=QColor(0xff,0xff,0xff);

    maxValue=100;
    minValue=0;
    precision=0;
    pointerColor=QColor(0xfd,0x69,0x69);

    centerCircleColor=QColor(255,255,255);

    textColor=QColor(0,0,0);

    overlayColor=QColor(255,255,255);
    showOverlay=true;
}

GaugeCar::~GaugeCar()
{}

double GaugeCar::getMinValue() const
{
    return minValue;
}

double GaugeCar::getMaxValue() const
{
    return maxValue;
}

double GaugeCar::getValue() const
{
    return currentValue;
}

int GaugeCar::getPrecision() const
{
    return precision;
}

int GaugeCar::getScaleMajor() const
{
    return scaleMajor;
}

int GaugeCar::getScaleMinor() const
{
    return scaleMinor;
}

int GaugeCar::getStartAngle() const
{
    return startAngle;
}

int GaugeCar::getEndAngle() const
{
    return endAngle;
}

bool GaugeCar::getAnimation() const
{
    return animation;
}

double GaugeCar::getAnimationStep() const
{
    return animationStep;
}

QColor GaugeCar::getOuterCircleColor() const
{
    return outerCircleColor;
}

QColor GaugeCar::getInnerCircleColor() const
{
    return innerCircleColor;
}

QColor GaugeCar::getPieColorStart() const
{
    return pieColorStart;
}

QColor GaugeCar::getPieColorMid() const
{
    return pieColorMid;
}

QColor GaugeCar::getPieColorEnd() const
{
    return pieColorEnd;
}

QColor GaugeCar::getCoverCircleColor() const
{
    return coverCircleColor;
}

QColor GaugeCar::getScaleColor() const
{
    return scaleColor;
}

QColor GaugeCar::getPointerColor() const
{
    return pointerColor;
}

QColor GaugeCar::getCenterCircleColor() const
{
    return centerCircleColor;
}

QColor GaugeCar::getTextColor() const
{
    return textColor;
}

bool GaugeCar::getShowOverlay() const
{
    return showOverlay;
}

QColor GaugeCar::getOverlayColor() const
{
    return overlayColor;
}

GaugeCar::PieStyle GaugeCar::getPieStyle() const
{
    return pieStyle;
}

GaugeCar::PointerStyle GaugeCar::getPointerStyle() const
{
    return pointerStyle;
}

QSize GaugeCar::sizeHint() const
{
    return size();
}

QSize GaugeCar::minimumSizeHint() const
{
    return minimumSize();
}

void GaugeCar::setRange(double minValue, double maxValue)
{
    this->minValue=minValue;
    this->maxValue=maxValue;
        update();
}

void GaugeCar::setRange(int minValue, int maxValue)
{
    this->minValue=minValue;
    this->maxValue=maxValue;
        update();
}

void GaugeCar::setMinValue(double minValue)
{
    this->minValue=minValue;
        update();
}

void GaugeCar::setMaxValue(double maxValue)
{
    this->maxValue=maxValue;
        update();
}

void GaugeCar::setValue(double value)
{
    this->currentValue=value;
        update();
}

void GaugeCar::setValue(int value)
{
    this->currentValue=value;
    update();
}

void GaugeCar::setPrecision(int precision)
{
    this->precision=precision;
        update();
}

void GaugeCar::setScaleMajor(int scaleMajor)
{
    this->scaleMajor=scaleMajor;
        update();
}

void GaugeCar::setScaleMinor(int scaleMinor)
{
    this->scaleMinor=scaleMinor;
        update();
}

void GaugeCar::setStartAngle(int startAngle)
{
    this->startAngle=startAngle;
        update();
}

void GaugeCar::setEndAngle(int endAngle)
{
    this->endAngle=endAngle;
        update();
}

void GaugeCar::setAnimation(bool animation)
{
    this->animation=animation;
        update();
}

void GaugeCar::setAnimationStep(double animationStep)
{
    this->animationStep=animationStep;
        update();
}

void GaugeCar::setOuterCircleColor(const QColor &outerCircleColor)
{
    this->outerCircleColor=outerCircleColor;
        update();
}

void GaugeCar::setInnerCircleColor(const QColor &innerCircleColor)
{
    this->innerCircleColor=innerCircleColor;
        update();
}

void GaugeCar::setPieColorStart(const QColor &pieColorStart)
{
    this->pieColorStart=pieColorStart;
        update();
}

void GaugeCar::setPieColorMid(const QColor &pieColorMid)
{
    this->pieColorMid=pieColorMid;
        update();
}

void GaugeCar::setPieColorEnd(const QColor &pieColorEnd)
{
    this->pieColorEnd=pieColorEnd;
        update();
}

void GaugeCar::setCoverCircleColor(const QColor &coverCircleColor)
{
    this->coverCircleColor=coverCircleColor;
        update();
}

void GaugeCar::setScaleColor(const QColor &scaleColor)
{
    this->scaleColor=scaleColor;
        update();
}

void GaugeCar::setPointerColor(const QColor &pointerColor)
{
    this->pointerColor=pointerColor;
        update();
}

void GaugeCar::setCenterCircleColor(const QColor &centerCircleColor)
{
    this->centerCircleColor=centerCircleColor;
        update();
}

void GaugeCar::setTextColor(const QColor &textColor)
{
    this->textColor=textColor;
        update();
}

void GaugeCar::setShowOverlay(bool showOverlay)
{
    this->showOverlay=showOverlay;
        update();
}

void GaugeCar::setOverlayColor(const QColor &overlayColor)
{
    this->overlayColor=overlayColor;
        update();
}

void GaugeCar::setPieStyle(const PieStyle &pieStyle)
{
    this->pieStyle=pieStyle;
        update();
}

void GaugeCar::setPointerStyle(const PointerStyle &pointerStyle)
{
    this->pointerStyle=pointerStyle;
        update();
}

void GaugeCar::paintEvent(QPaintEvent *event)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);

    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    painter.scale(side / 200.0, side / 200.0);
    //绘制外圆
    drawOuterCircle(&painter);
    //绘制内圆
    drawInnerCircle(&painter);
    //绘制饼圆
    drawColorPie(&painter);
    //绘制覆盖圆 用以遮住饼圆多余部分
    drawCoverCircle(&painter);
    //绘制刻度线
    drawScale(&painter);
    //绘制刻度值
    drawScaleNum(&painter);

    //根据指示器形状绘制指示器
    if (pointerStyle == PointerStyle_Circle) {
        drawPointerCircle(&painter);
    } else if (pointerStyle == PointerStyle_Indicator) {
        drawPointerIndicator(&painter);
    } else if (pointerStyle == PointerStyle_IndicatorR) {
        drawPointerIndicatorR(&painter);
    } else if (pointerStyle == PointerStyle_Triangle) {
        drawPointerTriangle(&painter);
    }

    //绘制指针中心圆外边框
    drawRoundCircle(&painter);
    //绘制指针中心圆
    drawCenterCircle(&painter);
    //绘制当前值
    drawValue(&painter);
    //绘制遮罩层
    drawOverlay(&painter);
}

void GaugeCar::drawOuterCircle(QPainter *painter)
{
    int radius = 99;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(outerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawInnerCircle(QPainter *painter)
{
    int radius = 90;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(innerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawColorPie(QPainter *painter)
{
    int radius = 60;
    painter->save();
    painter->setPen(Qt::NoPen);

    QRectF rect(-radius, -radius, radius * 2, radius * 2);

    if (pieStyle == PieStyle_Three) {
        //计算总范围角度,根据占比例自动计算三色圆环范围角度
        //可以更改比例
        double angleAll = 360.0 - startAngle - endAngle;
        double angleStart = angleAll * 0.7;
        double angleMid = angleAll * 0.15;
        double angleEnd = angleAll * 0.15;

        //增加偏移量使得看起来没有脱节
        int offset = 3;

        //绘制开始饼圆
        painter->setBrush(pieColorStart);
        painter->drawPie(rect, (270 - startAngle - angleStart) * 16, angleStart * 16);

        //绘制中间饼圆
        painter->setBrush(pieColorMid);
        painter->drawPie(rect, (270 - startAngle - angleStart - angleMid) * 16 + offset, angleMid * 16);

        //绘制结束饼圆
        painter->setBrush(pieColorEnd);
        painter->drawPie(rect, (270 - startAngle - angleStart - angleMid - angleEnd) * 16 + offset * 2, angleEnd * 16);
    } else if (pieStyle == PieStyle_Current) {
        //计算总范围角度,当前值范围角度,剩余值范围角度
        double angleAll = 360.0 - startAngle - endAngle;
        double angleCurrent = angleAll * ((currentValue - minValue) / (maxValue - minValue));
        double angleOther = angleAll - angleCurrent;

        //绘制当前值饼圆
        painter->setBrush(pieColorStart);
        painter->drawPie(rect, (270 - startAngle - angleCurrent) * 16, angleCurrent * 16);

        //绘制剩余值饼圆
        painter->setBrush(pieColorEnd);
        painter->drawPie(rect, (270 - startAngle - angleCurrent - angleOther) * 16, angleOther * 16);
    }

    painter->restore();
}

void GaugeCar::drawCoverCircle(QPainter *painter)
{
    int radius = 50;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(coverCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawScale(QPainter *painter)
{
    int radius = 72;
    painter->save();

    painter->rotate(startAngle);
    int steps = (scaleMajor * scaleMinor);
    double angleStep = (360.0 - startAngle - endAngle) / steps;

    QPen pen;
    pen.setColor(scaleColor);
    pen.setCapStyle(Qt::RoundCap);

    for (int i = 0; i <= steps; i++) {
        if (i % scaleMinor == 0) {
            pen.setWidthF(1.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 10, 0, radius);
        } else {
            pen.setWidthF(0.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 5, 0, radius);
        }

        painter->rotate(angleStep);
    }

    painter->restore();
}

void GaugeCar::drawScaleNum(QPainter *painter)
{
    int radius = 82;
    painter->save();
    painter->setPen(scaleColor);

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

void GaugeCar::drawPointerCircle(QPainter *painter)
{
    int radius = 6;
    int offset = 30;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);

    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawEllipse(-radius, radius + offset, radius * 2, radius * 2);

    painter->restore();
}

void GaugeCar::drawPointerIndicator(QPainter *painter)
{
    int radius = 75;
    painter->save();
    painter->setOpacity(0.8);
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);

    QPolygon pts;
    pts.setPoints(3, -5, 0, 5, 0, 0, radius);

    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);

    painter->restore();
}

void GaugeCar::drawPointerIndicatorR(QPainter *painter)
{
    int radius = 75;
    painter->save();
    painter->setOpacity(1.0);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(pointerColor);
    painter->setPen(pen);
    painter->setBrush(pointerColor);

    QPolygon pts;
    pts.setPoints(3, -5, 0, 5, 0, 0, radius);

    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);

    //增加绘制圆角直线,与之前三角形重叠,形成圆角指针
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(4);
    painter->setPen(pen);
    painter->drawLine(0, 0, 0, radius);

    painter->restore();
}

void GaugeCar::drawPointerTriangle(QPainter *painter)
{
    int radius = 10;
    int offset = 38;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);

    QPolygon pts;
    pts.setPoints(3, -5, 0 + offset, 5, 0 + offset, 0, radius + offset);

    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);

    painter->restore();
}

void GaugeCar::drawRoundCircle(QPainter *painter)
{
    int radius = 18;
    painter->save();
    painter->setOpacity(0.8);
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawCenterCircle(QPainter *painter)
{
    int radius = 15;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(centerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawValue(QPainter *painter)
{
    int radius = 100;
    painter->save();
    painter->setPen(textColor);

    QFont font;
    font.setPixelSize(18);
    painter->setFont(font);

    QRectF textRect(-radius, -radius, radius * 2, radius * 2);
    QString strValue = QString("%1").arg((double)currentValue, 0, 'f', precision);
    painter->drawText(textRect, Qt::AlignCenter, strValue);

    painter->restore();
}

void GaugeCar::drawOverlay(QPainter *painter)
{
    if (!showOverlay) {
        return;
    }

    int radius = 90;
    painter->save();
    painter->setPen(Qt::NoPen);

    QPainterPath smallCircle;
    QPainterPath bigCircle;
    radius -= 1;
    smallCircle.addEllipse(-radius, -radius, radius * 2, radius * 2);
    radius *= 2;
    bigCircle.addEllipse(-radius, -radius + 140, radius * 2, radius * 2);

    //高光的形状为小圆扣掉大圆的部分
    QPainterPath highlight = smallCircle - bigCircle;

    QLinearGradient linearGradient(0, -radius / 2, 0, 0);
    overlayColor.setAlpha(100);
    linearGradient.setColorAt(0.0, overlayColor);
    overlayColor.setAlpha(30);
    linearGradient.setColorAt(1.0, overlayColor);
    painter->setBrush(linearGradient);
    painter->rotate(-20);
    painter->drawPath(highlight);

    painter->restore();
}

