#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadImage(QString filePath);

    void createNormalWM();
    void createBlindWM();

protected:
    void resizeEvent(QResizeEvent* e);
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent* e);

private slots:
    void on_btnOpen_clicked();
    void on_btnColor_clicked();

    void on_btnBlindColor_clicked();

    void on_spinBoxFontSize_valueChanged(int v);

    void on_lineWMCtx_editingFinished();

    void on_checkBoxRect_stateChanged(int arg1);

    void on_spinBoxX_valueChanged(int v);

    void on_spinBoxY_valueChanged(int v);

    void on_spinBoxW_valueChanged(int v);

    void on_spinBoxH_valueChanged(int v);

    void on_btnSave_clicked();

    void on_lineBlindWMCtx_editingFinished();

    void on_spinBlindPosX_valueChanged(int v);

    void on_spinBlindPosY_valueChanged(int v);

    void on_spinBlindThickness_valueChanged(int v);

    void on_btnResultTest_clicked();

    void on_spinBlindScale_valueChanged(double v);

private:
    Ui::MainWindow *ui;
    NormalWatermark *normalwm;
    BlindWatermark  *blindwm;

    QImage _mImg;//原始图片
    QImage _mNormalResult;//结果图片
    QImage _mBlindResult;
    QColor _mNormalColor;//当前设置的颜色
    QColor _mBlindColor;
    QString _mFilePath;
};
#endif // MAINWINDOW_H
