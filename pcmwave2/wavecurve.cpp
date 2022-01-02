#include "wavecurve.h"
#include "ui_wavecurve.h"

WaveCurve::WaveCurve(QWidget *parent) :
    QwtPlot(parent),
    ui(new Ui::WaveCurve)
{
    ui->setupUi(this);

    maxCnt=5000;//x轴坐标的数量0-1000
    //设置x轴坐标值
    for(int i=0;i<maxCnt;i++){
        xdata.append(i);
    }

    canvas = new QwtPlotCanvas(this);
    canvas->setPalette(Qt::white);//背景色为白色
    canvas->setBorderRadius(10);//画面边框角为半径为10的圆角
    setCanvas(canvas);
    plotLayout()->setAlignCanvasToScales(true);//设置坐标轴数值自适应

    //坐标与图示说明
    setAxisTitle(QwtPlot::yLeft,"Value");
    setAxisTitle(QwtPlot::xBottom,"time: s");
    setAxisScale(QwtPlot::yLeft,-254,255);
    setAxisScale(QwtPlot::xBottom,0,maxCnt);

    //画板中的网格图
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableY(true);
    grid->setMajorPen(Qt::black,0,Qt::DotLine);
    grid->attach(this);
    //画板中的折线
    curveChannelLeft = new QwtPlotCurve("left");
    curveChannelLeft->setPen(Qt::red,2);
    curveChannelLeft->setRenderHint(QwtPlotItem::RenderAntialiased,true);

    curveChannelRight = new QwtPlotCurve("right");
    curveChannelRight->setPen(Qt::green,2);
    curveChannelRight->setRenderHint(QwtPlotItem::RenderAntialiased,true);

    QwtPlotItemList items=itemList(QwtPlotItem::Rtti_PlotCurve);
    for ( int i = 0; i < items.size(); i++ ){
        items[i]->setVisible(true);
    }

    replot();
    setAutoReplot( true );//设置自动重画，相当于更新
}

WaveCurve::~WaveCurve()
{
    delete curveChannelLeft;
    delete curveChannelRight;
    delete canvas;
    delete ui;
}

void WaveCurve::updateChannelData(QVector<double> dl, QVector<double> dr)
{
    if(channelDataL.size()>=maxCnt){
//        channelDataL.pop_front();
        channelDataL.remove(0,4086);
    }
    if(channelDataR.size()>=maxCnt){
//        channelDataR.pop_front();
        channelDataR.remove(0,4086);
    }
    channelDataL.append(dl);
    channelDataR.append(dr);

    curveChannelLeft->setSamples(xdata,channelDataL);
    curveChannelLeft->attach(this);
    curveChannelLeft->setLegendAttribute(curveChannelLeft->LegendShowLine);//显示图例的标志，这里显示线的颜色。

    curveChannelRight->setSamples(xdata,channelDataR);
    curveChannelRight->attach(this);
    curveChannelRight->setLegendAttribute(curveChannelLeft->LegendShowLine);//显示图例的标志，这里显示线的颜色。

    replot();
}
