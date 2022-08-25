#include "curvescommon.h"
#include "ui_curvescommon.h"

CurvesCommon::CurvesCommon(QWidget *parent) :
    QwtPlot(parent),
    ui(new Ui::CurvesCommon)
{
    ui->setupUi(this);

    maxCnt = 201;//x轴的最大值

    for(int i=1;i<maxCnt;i++){
        xdata.append(double(i));
    }

    //基本的画布
    canvas = new QwtPlotCanvas(this);
    canvas->setPalette(Qt::white);//画布背景
    canvas->setBorderRadius(10);//画布圆角
    setCanvas(canvas);
    plotLayout()->setAlignCanvasToScales(true);

    //设置坐标轴信息
    setAxisTitle(QwtPlot::yLeft,"value");
    setAxisTitle(QwtPlot::xBottom,"time: s");
    setAxisScale(QwtPlot::yLeft,0.0,256.0);
    setAxisScale(QwtPlot::xBottom,0.0,maxCnt);

    //子图
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableY(true);
    grid->setMajorPen(Qt::black,0,Qt::DotLine);
    grid->attach(this);

    //折线1
    curveAttention = new QwtPlotCurve("attention");
    curveAttention->setPen(Qt::red,2);
    curveAttention->setRenderHint(QwtPlotItem::RenderAntialiased,true);

    //折线2
    curveBlink = new QwtPlotCurve("blink");
    curveBlink->setPen(Qt::gray,2);
    curveBlink->setRenderHint(QwtPlotItem::RenderAntialiased,true);

    //折线3
    curveMeditation = new QwtPlotCurve("meditation");
    curveMeditation->setPen(Qt::green,2);
    curveMeditation->setRenderHint(QwtPlotItem::RenderAntialiased,true);

    //折线4
    curveSignal = new QwtPlotCurve("signal");
    curveSignal->setPen(Qt::blue,2);
    curveSignal->setRenderHint(QwtPlotItem::RenderAntialiased,true);

    //折线5
    curvePower = new QwtPlotCurve("power");
    curvePower->setPen(Qt::yellow,2);
    curvePower->setRenderHint(QwtPlotItem::RenderAntialiased,true);

    //--------------设置图例可以被点击来确定是否显示曲线-----------------------//
    QwtLegend *legend = new QwtLegend;
    legend->setDefaultItemMode( QwtLegendData::Checkable );//图例可被点击
    insertLegend( legend, QwtPlot::RightLegend );
    connect(legend,&QwtLegend::checked,this,&CurvesCommon::showItem);//点击图例操作

    QwtPlotItemList items = itemList( QwtPlotItem::Rtti_PlotCurve );//获取画了多少条曲线,如果为获取其他形状，注意改变参数
    for ( int i = 0; i < items.size(); i++ ){
        if(i==0){
        const QVariant itemInfo = itemToInfo(items[i]);
        QwtLegendLabel *legendLabel = qobject_cast<QwtLegendLabel*>(legend->legendWidget(itemInfo));
        if(legendLabel){
            legendLabel->setChecked(true);
        }
        items[i]->setVisible(true);
        }else{
            items[i]->setVisible(false);
        }
    }
    resize(800,266);
    replot();
    setAutoReplot(true);//设置自动重画，相当于更新
}

CurvesCommon::~CurvesCommon()
{
    delete ui;
}

//更新小值数据至界面
void CurvesCommon::updateCommonData(struct _eegPkt pkt)
{
    //注意力数据
    if(dataAttention.size()>maxCnt){
        dataAttention.pop_front();
    }
    dataAttention.append(pkt.attention);
    curveAttention->setSamples(xdata,dataAttention);
    curveAttention->attach(this);
    curveAttention->setLegendAttribute(curveAttention->LegendShowLine);//显示图例的标志，这里显示线的颜色。

    //眨眼数据
    if(dataBlink.size()>maxCnt){
        dataBlink.pop_front();
    }
    dataBlink.append(pkt.blink);
    curveBlink->setSamples(xdata,dataBlink);
    curveBlink->attach(this);
    curveBlink->setLegendAttribute(curveBlink->LegendShowLine);//显示图例的标志，这里显示线的颜色。

    //冥想数据
    if(dataMeditation.size()>maxCnt){
        dataMeditation.pop_front();
    }
    dataMeditation.append(pkt.meditation);
    curveMeditation->setSamples(xdata,dataMeditation);
    curveMeditation->attach(this);
    curveMeditation->setLegendAttribute(curveMeditation->LegendShowLine);//显示图例的标志，这里显示线的颜色。

    //信号强度
    if(dataSignal.size()>maxCnt){
        dataSignal.pop_front();
    }
    dataSignal.append(pkt.signal);
    curveSignal->setSamples(xdata,dataSignal);
    curveSignal->attach(this);
    curveSignal->setLegendAttribute(curveSignal->LegendShowLine);//显示图例的标志，这里显示线的颜色。

    //电源指示
    if(dataPower.size()>maxCnt){
        dataPower.pop_front();
    }
    dataPower.append(pkt.power);
    curvePower->setSamples(xdata,dataPower);
    curvePower->attach(this);
    curvePower->setLegendAttribute(curvePower->LegendShowLine);//显示图例的标志，这里显示线的颜色。

    replot();
}

void CurvesCommon::CurveClear()
{
    dataAttention.clear();
    dataBlink.clear();
    dataMeditation.clear();
    dataSignal.clear();
    dataPower.clear();
}

//是否显示此折线槽函数
void CurvesCommon::showItem(const QVariant &itemInfo, bool on)
{
    QwtPlotItem *plotItem = infoToItem(itemInfo);
    if(plotItem)
       plotItem->setVisible(on);
}
