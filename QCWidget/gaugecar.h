#ifndef GAUGECAR_H
#define GAUGECAR_H

/**
 * 汽车仪表盘控件 作者:feiyangqingyun(QQ:517216493) 2016-11-20
 * 1:可设置范围值,支持负数值
 * 2:可设置精确度,最大支持小数点后3位
 * 3:可设置大刻度数量/小刻度数量
 * 4:可设置开始旋转角度/结束旋转角度
 * 5:可设置是否启用动画效果以及动画效果每次移动的步长
 * 6:可设置外圆背景/内圆背景/饼圆三种颜色/刻度尺颜色/文字颜色
 * 7:自适应窗体拉伸,刻度尺和文字自动缩放
 * 8:可自由拓展各种渐变色,各圆的半径
 * 9:三色圆环按照比例设置范围角度 用户可以自由设置三色占用比例
 * 10:圆环样式可选择 三色圆环 当前圆环
 * 11:指示器样式可选择 圆形指示器 指针指示器 圆角指针指示器 三角形指示器
 */

#include <QWidget>

class GaugeCar : public QWidget{
    Q_OBJECT

        Q_PROPERTY(double minValue READ getMinValue WRITE setMinValue CONSTANT)
        Q_PROPERTY(double maxValue READ getMaxValue WRITE setMaxValue CONSTANT)
        Q_PROPERTY(double value READ getValue WRITE setValue CONSTANT)
        Q_PROPERTY(int precision READ getPrecision WRITE setPrecision CONSTANT)

        Q_PROPERTY(int scaleMajor READ getScaleMajor WRITE setScaleMajor CONSTANT)
        Q_PROPERTY(int scaleMinor READ getScaleMinor WRITE setScaleMinor CONSTANT)
        Q_PROPERTY(int startAngle READ getStartAngle WRITE setStartAngle CONSTANT)
        Q_PROPERTY(int endAngle READ getEndAngle WRITE setEndAngle CONSTANT)

        Q_PROPERTY(bool animation READ getAnimation WRITE setAnimation CONSTANT)
        Q_PROPERTY(double animationStep READ getAnimationStep WRITE setAnimationStep CONSTANT)

        Q_PROPERTY(QColor outerCircleColor READ getOuterCircleColor WRITE setOuterCircleColor CONSTANT)
        Q_PROPERTY(QColor innerCircleColor READ getInnerCircleColor WRITE setInnerCircleColor CONSTANT)

        Q_PROPERTY(QColor pieColorStart READ getPieColorStart WRITE setPieColorStart CONSTANT)
        Q_PROPERTY(QColor pieColorMid READ getPieColorMid WRITE setPieColorMid CONSTANT)
        Q_PROPERTY(QColor pieColorEnd READ getPieColorEnd WRITE setPieColorEnd CONSTANT)

        Q_PROPERTY(QColor coverCircleColor READ getCoverCircleColor WRITE setCoverCircleColor CONSTANT)
        Q_PROPERTY(QColor scaleColor READ getScaleColor WRITE setScaleColor CONSTANT)
        Q_PROPERTY(QColor pointerColor READ getPointerColor WRITE setPointerColor CONSTANT)
        Q_PROPERTY(QColor centerCircleColor READ getCenterCircleColor WRITE setCenterCircleColor CONSTANT)
        Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor CONSTANT)

        Q_PROPERTY(bool showOverlay READ getShowOverlay WRITE setShowOverlay CONSTANT)
        Q_PROPERTY(QColor overlayColor READ getOverlayColor WRITE setOverlayColor CONSTANT)

        Q_PROPERTY(PieStyle pieStyle READ getPieStyle WRITE setPieStyle CONSTANT)
        Q_PROPERTY(PointerStyle pointerStyle READ getPointerStyle WRITE setPointerStyle CONSTANT)

public:
    enum PieStyle {
        PieStyle_Three = 0,         	//三色圆环
        PieStyle_Current = 1        	//当前圆环
    };

    enum PointerStyle {
        PointerStyle_Circle = 0,        //圆形指示器
        PointerStyle_Indicator = 1,     //指针指示器
        PointerStyle_IndicatorR = 2,    //圆角指针指示器
        PointerStyle_Triangle = 3       //三角形指示器
    };

    Q_ENUM(PieStyle)
    Q_ENUM(PointerStyle)

    GaugeCar(QWidget *parent = nullptr);
    ~GaugeCar();

    double getMinValue()            const;
    double getMaxValue()            const;
    double getValue()               const;
    int getPrecision()              const;

    int getScaleMajor()             const;
    int getScaleMinor()             const;
    int getStartAngle()             const;
    int getEndAngle()               const;

    bool getAnimation()             const;
    double getAnimationStep()       const;

    QColor getOuterCircleColor()    const;
    QColor getInnerCircleColor()    const;

    QColor getPieColorStart()       const;
    QColor getPieColorMid()         const;
    QColor getPieColorEnd()         const;

    QColor getCoverCircleColor()    const;
    QColor getScaleColor()          const;
    QColor getPointerColor()        const;
    QColor getCenterCircleColor()   const;
    QColor getTextColor()           const;

    bool getShowOverlay()           const;
    QColor getOverlayColor()        const;

    PieStyle getPieStyle()          const;
    PointerStyle getPointerStyle()  const;

    QSize sizeHint()                const;
    QSize minimumSizeHint()         const;

public Q_SLOTS:
    //设置范围值
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);

    //设置最大最小值
    void setMinValue(double minValue);
    void setMaxValue(double maxValue);

    //设置目标值
    void setValue(double value);
    void setValue(int value);

    //设置精确度
    void setPrecision(int precision);

    //设置主刻度数量
    void setScaleMajor(int scaleMajor);
    //设置小刻度数量
    void setScaleMinor(int scaleMinor);
    //设置开始旋转角度
    void setStartAngle(int startAngle);
    //设置结束旋转角度
    void setEndAngle(int endAngle);

    //设置是否启用动画显示
    void setAnimation(bool animation);
    //设置动画显示的步长
    void setAnimationStep(double animationStep);

    //设置外圆背景颜色
    void setOuterCircleColor(const QColor &outerCircleColor);
    //设置内圆背景颜色
    void setInnerCircleColor(const QColor &innerCircleColor);

    //设置饼圆三种颜色
    void setPieColorStart(const QColor &pieColorStart);
    void setPieColorMid(const QColor &pieColorMid);
    void setPieColorEnd(const QColor &pieColorEnd);

    //设置覆盖圆背景颜色
    void setCoverCircleColor(const QColor &coverCircleColor);
    //设置刻度尺颜色
    void setScaleColor(const QColor &scaleColor);
    //设置指针颜色
    void setPointerColor(const QColor &pointerColor);
    //设置中心圆颜色
    void setCenterCircleColor(const QColor &centerCircleColor);
    //设置文本颜色
    void setTextColor(const QColor &textColor);

    //设置是否显示遮罩层
    void setShowOverlay(bool showOverlay);
    //设置遮罩层颜色
    void setOverlayColor(const QColor &overlayColor);

    //设置饼图样式
    void setPieStyle(const GaugeCar::PieStyle &pieStyle);
    //设置指针样式
    void setPointerStyle(const GaugeCar::PointerStyle &pointerStyle);

Q_SIGNALS:
    void valueChanged(int value);

protected:
    void paintEvent(QPaintEvent* event);
    void drawOuterCircle(QPainter* painter);
    void drawInnerCircle(QPainter* painter);
    void drawColorPie(QPainter* painter);
    void drawCoverCircle(QPainter* painter);
    void drawScale(QPainter*painter);
    void drawScaleNum(QPainter* painter);
    void drawPointerCircle(QPainter* painter);
    void drawPointerIndicator(QPainter* painter);
    void drawPointerIndicatorR(QPainter* painter);
    void drawPointerTriangle(QPainter* painter);
    void drawRoundCircle(QPainter* painter);
    void drawCenterCircle(QPainter* painter);
    void drawValue(QPainter* painter);
    void drawOverlay(QPainter* painter);

private slots:
    //void updateValue();

private:
    double minValue;                //最小值
    double maxValue;                //最大值
    double value;                   //目标值
    int precision;                  //精确度,小数点后几位

    int scaleMajor;                 //大刻度数量
    int scaleMinor;                 //小刻度数量

    QColor outerCircleColor;        //外圆背景颜色
    QColor innerCircleColor;        //内圆背景颜色

    int startAngle;                 //开始旋转角度
    int endAngle;                   //结束旋转角度

    QColor pieColorStart;           //饼圆开始颜色
    QColor pieColorMid;             //饼圆中间颜色
    QColor pieColorEnd;             //饼圆结束颜色

    PieStyle pieStyle;              //饼图样式
    PointerStyle pointerStyle;      //指针样式

    bool animation;                 //是否启用动画显示
    double animationStep;           //动画显示时步长

    QColor coverCircleColor;        //覆盖圆背景颜色
    QColor scaleColor;              //刻度尺颜色
    QColor pointerColor;            //指针颜色
    QColor centerCircleColor;       //中心圆颜色
    QColor textColor;               //文字颜色

    bool showOverlay;               //显示遮罩层
    QColor overlayColor;            //遮罩层颜色

    bool reverse;                   //是否往回走
    double currentValue;            //当前值
    QTimer *timer;                  //定时器绘制动画
};

#endif // GAUGECAR_H
