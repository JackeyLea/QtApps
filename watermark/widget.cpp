#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

//打开需要加水印图片的目录
void Widget::on_btnNormalOpen_clicked()
{
    //清空变量
    m_imgCnt=0;
    m_imgPath.clear();
    m_curIndex = 0;
    ui->lineWMDir->clear();

    QString dirPath = QFileDialog::getExistingDirectory(this,QString("打开"));
    if(dirPath.isEmpty()){
        QMessageBox::warning(this,QString("警告"),"没选");
        return;
    }

    ui->lineWMDir->setText(dirPath);

    //遍历文件夹中所有的图片
    QDir dir(dirPath);
    QStringList filters;
    filters<<"*.jpg"<<"*.png"<<"*.bmp"<<"*.JPG"<<"*.jpeg"<<"*.PNG";

    QFileInfoList fileInfoList = dir.entryInfoList(filters);
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName()==".."){
            continue;
        }

        if(fileInfo.isFile()){
            m_imgCnt++;
            m_imgPath.append(fileInfo.absoluteFilePath());
        }
    }
    qDebug()<<m_imgPath;
    ui->label->setText(QString("%1/%2").arg(m_curIndex+1).arg(m_imgPath.size()));
}

