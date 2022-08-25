#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopServices>
#include <QDateTime>
#include <QMessageBox>
#include <QUrl>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString logFilePath;//日志文件详细路径

private:
    Ui::MainWindow *ui;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void log(QString str);

private slots:
    void on_actionOpen_triggered();

    void on_actionAdd_URLs_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionIndex_triggered();
};
#endif // MAINWINDOW_H
