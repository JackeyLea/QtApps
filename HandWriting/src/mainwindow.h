#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>

#include "opencv2/opencv.hpp"

#include "global.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSave_clicked();
    void on_btnGo_clicked();
    void on_btnAbout_clicked();
    void on_btnAboutQt_clicked();
    void on_btnIndex_clicked();
    void on_btnClear_clicked();

private:
    Ui::MainWindow *ui;

    cv::Mat _matImg;
};

#endif // MAINWINDOW_H
