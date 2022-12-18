#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QFile>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QImage>
#include <QColorDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QSpinBox>

#include "blindwatermark.h"
#include "normalwatermark.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void createNormalWM();
    void createBlindWM();

protected:
    void resizeEvent(QResizeEvent* e);
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent* e);

private slots:
    void on_btnNormalOpen_clicked();

    void on_btnNormalToTest_clicked();

    void on_btnPre_clicked();

    void on_btnNext_clicked();

    void on_btnNormalSave_clicked();

    void on_btnNormalColor_clicked();

    void on_spinBoxNormalFontSize_valueChanged(int v);

    void on_spinBoxNormalX_valueChanged(int v);

    void on_spinBoxNormalY_valueChanged(int v);

    void on_spinBoxNormalW_valueChanged(int v);

    void on_spinBoxNormalH_valueChanged(int v);

    void on_checkNormalRect_stateChanged(int v);

    void on_spinBlindPosX_valueChanged(int v);

    void on_spinBlindPosY_valueChanged(int v);

    void on_spinBlindThickness_valueChanged(int v);

    void on_spinBlindScale_valueChanged(double v);

    void on_btnOpenImgWM_clicked();

    void on_btnParse_clicked();

    void on_btnTestToWM_clicked();

    void on_spinBoxNormalOpacity_valueChanged(int arg1);

private:
    Ui::Widget *ui;

    NormalWatermark *normalwm;
    BlindWatermark  *blindwm;

    QStringList m_imgPath;
    int32_t m_curIndex=1;

    QImage _mImg;//原始图片
    QImage _mNormalResult;//结果图片
    QImage _mBlindResult;
    QColor _mNormalColor;//当前设置的颜色
};
#endif // WIDGET_H
