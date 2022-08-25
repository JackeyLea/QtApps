#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>
#include <QThread>
#include <QFile>
#include <QDesktopServices>
#include <QMessageBox>
#include <QtXml>
#include <QTreeView>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    QDomDocument doc;
    QString fileFullPath;
    QString basePath;
    QString dstFullPath;
    QString resultFullPath;
    int fileCnt;
    int dirCnt;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void log(QString str);

    void findAllFiles(QString dirPath);

    bool initXML();

    bool addKeywordsNode(QString content);

    bool addFilesNode(bool isValid, QString title, QString content);

    bool addTocNode(QString content);

    bool save2file(QString dstPath);

    bool generateQch();

private slots:
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_actionIndex_triggered();
    void on_actionAbout_Qt_triggered();

    void on_btnFolder_clicked();
    void on_btnFile_clicked();
    void on_btnGo_clicked();

private:
    Ui::MainWindow *ui;

    QStandardItemModel *treeModel;
};
#endif // MAINWINDOW_H
