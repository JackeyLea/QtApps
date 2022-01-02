#ifndef WAVECURVE_H
#define WAVECURVE_H

// qwt headers
#include "qwt.h"
#include "qwt_plot.h"
#include "qwt_plot_canvas.h"
#include <qwt_plot_renderer.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>

#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>
#include <qwt_symbol.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_layout.h>

#include <QDebug>
#include <QTimer>
#include <QFrame>
#include <QRandomGenerator>
#include <QDateTime>

namespace Ui {
class WaveCurve;
}

class WaveCurve : public QwtPlot
{
    Q_OBJECT

public:
    explicit WaveCurve(QWidget *parent = nullptr);
    ~WaveCurve();

public slots:
    void updateChannelData(QVector<double> dl,QVector<double> dr);

private:
    Ui::WaveCurve *ui;
    QwtPlotCanvas *canvas;//画板
    QwtPlotCurve *curveChannelLeft;//左声道显示折线
    QwtPlotCurve *curveChannelRight;//右声道显示折线
    //数据部分
    QVector<double> xdata;//位于画面水平居中位置的x坐标显示数据
    QVector<double> channelDataL;//数据值，数值一样，但是符号不一样。
    QVector<double> channelDataR;

    int maxCnt;//x轴的坐标数量
};

#endif // WAVECURVE_H
