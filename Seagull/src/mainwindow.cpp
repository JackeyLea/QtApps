#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::log(QString str)
{
    QString curTime = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss dddd");
    qDebug()<<curTime+" "+str;
    if(!logFilePath.isEmpty()){
        QFile file(logFilePath);
        if(file.open(QIODevice::Text|QIODevice::WriteOnly|QIODevice::Append)){
            QTextStream writeOut(&file);
            writeOut<<QString("【%1】 %2").arg(curTime).arg(str);
        }
        else{
            qDebug()<<"Cannot open log file: "<<logFilePath;
        }
        file.close();
    }
}


void MainWindow::on_actionOpen_triggered()
{
    log("Trying to open something.");
    QString filePath = QFileDialog::getOpenFileName(this,tr("Open"),QString(),QString("*.torrent\n*.metadata\n*"));
    if(filePath.isEmpty()) {
        log("Select nothing in Open() action.");
        return;
    }
}

void MainWindow::on_actionAdd_URLs_triggered()
{
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit(0);
}

void MainWindow::on_actionAbout_triggered()
{
    QString msg=QString("<h1>Seagull</h1>"
                        "<h2>下载软件</h2>"
                        "<h3>支持http/https/ed2k/torrent格式、磁力搜索</h3>"
                        "<h3>作者：幽弥狂</h3>");
    QMessageBox::information(this,tr("About"),msg,QMessageBox::Ok);
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_actionIndex_triggered()
{

}
