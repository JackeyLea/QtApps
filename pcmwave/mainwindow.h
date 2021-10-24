#ifndef MAINWINDOW_H
#define MAINWINDOW_H

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libavutil/avutil.h"
};

#include <QMainWindow>
#include <QPaintEvent>
#include <QPushButton>
#include <stdint.h>
#include <QProcess>
#include <QDir>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QDebug>

#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void drawWave();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_btnPlay_clicked();

private:
    Ui::MainWindow *ui;

    int length;
    QVector<short> vdata;

    QImage image;
};
#endif // MAINWINDOW_H
