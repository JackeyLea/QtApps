#include "mainwidget.h"
#include "./ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    connect(ui->frame,&FIRWidget::showMsg,this,&MainWidget::showMessage);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::showMessage(int type)
{
    QString str;
    switch(type){
    case 1://白赢
        str = "White win.";
        break;
    case -1://黑赢
        str = "Black win.";
        break;
    case 0://平局
        str = "Dead end.";
        break;
    default:
        break;
    }
    QMessageBox::information(this,tr("Tip"),str);
    ui->frame->resetGame();
}

void MainWidget::on_btnStart_clicked()
{
    //先判断模式
    //处理角色
    bool isWhiteFirst=false;
    if((ui->radioBtnWhite->isChecked() && ui->checkBoxFirst->isChecked()) ||
            (ui->radioBtnBlack->isChecked() && !ui->checkBoxFirst->isChecked()))
        isWhiteFirst=true;
    ui->frame->setWhiteFirst(isWhiteFirst);

    //难度等级
    ui->frame->setStart(true);
    ui->frame->update();
}

void MainWidget::on_btnReset_clicked()
{
    ui->frame->setStart(false);
    ui->frame->resetGame();
}

void MainWidget::on_radioBtnPVE_clicked()
{
    ui->radioBtnHard->setEnabled(true);
    ui->radioBtnMaster->setEnabled(true);
    ui->radioBtnMedium->setEnabled(true);
    ui->radioBtnPremium->setEnabled(true);
    ui->radioBtnEasy->setEnabled(true);
}

void MainWidget::on_radioBtnPVP_clicked()
{
    ui->radioBtnHard->setEnabled(false);
    ui->radioBtnMaster->setEnabled(false);
    ui->radioBtnMedium->setEnabled(false);
    ui->radioBtnPremium->setEnabled(false);
    ui->radioBtnEasy->setEnabled(false);
}


void MainWidget::on_btnRegret_clicked()
{
    //悔棋
}


void MainWidget::on_spinBox_valueChanged(int value)
{
    ui->frame->setSize(value);
    update();
}

