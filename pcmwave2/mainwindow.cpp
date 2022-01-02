#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ffAudio = new FFmpegAudio;
    connect(ffAudio,&FFmpegAudio::updateFrame,ui->frame,&WaveCurve::updateChannelData);
}

MainWindow::~MainWindow()
{
    delete ffAudio;
    delete ui;
}


void MainWindow::on_btnPlayer_clicked()
{
    ffAudio->start();
}

