#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QColorDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , normalwm(new NormalWatermark)
    , blindwm(new BlindWatermark)
    , _mNormalColor(Qt::red)
{
    ui->setupUi(this);

    setAcceptDrops(true);

    //设置界面
    ui->btnNormalColor->setStyleSheet(QString("background:%1").arg(_mNormalColor.name()));
    ui->spinBoxNormalFontSize->setValue(20);
    ui->spinBoxNormalX->setValue(20);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::createNormalWM()
{
    if(_mImg.isNull()){
        qDebug()<<("Please input an image.");
        return;
    }

    normalwm->setImage(_mImg);
    normalwm->setColor(_mNormalColor);
    normalwm->setFontPointSize(ui->spinBoxNormalFontSize->value());
    normalwm->setFrameVisible(ui->checkNormalRect->isChecked());
    normalwm->setPosH(ui->spinBoxNormalH->value());
    normalwm->setPosW(ui->spinBoxNormalW->value());
    normalwm->setPosY(ui->spinBoxNormalY->value());
    normalwm->setPosX(ui->spinBoxNormalX->value());
    _mNormalResult = normalwm->result();
    if(_mNormalResult.width() <= ui->labelNormalWMDemo->width() &&
            _mNormalResult.height() <= ui->labelNormalWMDemo->height()){
        ui->labelNormalWMDemo->setPixmap(QPixmap::fromImage(_mNormalResult));
    }else{
        ui->labelNormalWMDemo->setPixmap(QPixmap::fromImage(_mNormalResult).scaled(ui->labelNormalWMDemo->size(),Qt::KeepAspectRatio));
    }
}

void Widget::createBlindWM()
{
    if(_mNormalResult.isNull()){
        qDebug()<<("Please input an image.");
        return;
    }

    blindwm->setImage(_mNormalResult);//设置需要处理的图像
    blindwm->setPosition(QPoint(ui->spinBlindPosX->value(),ui->spinBlindPosY->value()));//水印位置
    blindwm->setThickness(ui->spinBlindThickness->value());//水印字符粗细
    blindwm->setScale(ui->spinBlindScale->value());
    _mBlindResult = blindwm->getQImageWatermark();

    if(_mBlindResult.width() <= ui->labelBlindDemo->width() &&
            _mBlindResult.height() <= ui->labelBlindDemo->height()){
        ui->labelBlindDemo->setPixmap(QPixmap::fromImage(_mBlindResult));
    }else{
        ui->labelBlindDemo->setPixmap(QPixmap::fromImage(_mBlindResult).scaled(ui->labelBlindDemo->size(),Qt::KeepAspectRatio));
    }
}

void Widget::resizeEvent(QResizeEvent *e)
{

}

void Widget::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasUrls()){
        e->acceptProposedAction();
    }else{
        e->ignore();
    }
}

void Widget::dropEvent(QDropEvent *e)
{
    int index = ui->stackedWidget->currentIndex();
    const QMimeData *mimeData = e->mimeData();
    if(mimeData->hasUrls()){
        QList<QUrl> urlList = mimeData->urls();
        QString fileName = urlList.at(0).toLocalFile();
        if(!fileName.isEmpty()){
            //loadImage(fileName);
            if(index==1){
                ui->lineWMImgPath->setText(fileName);
            }
        }
    }
}

//打开需要加水印图片的目录
void Widget::on_btnNormalOpen_clicked()
{
    //清空变量
    m_imgPath.clear();
    m_curIndex = 1;
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
    filters<<"*.jpg"<<"*.png"<<"*.bmp"<<"*.JPG"<<"*.jpeg"<<"*.PNG"<<"*.webp";

    QFileInfoList fileInfoList = dir.entryInfoList(filters);
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName()==".."){
            continue;
        }

        if(fileInfo.isFile()){
            m_imgPath.append(fileInfo.absoluteFilePath());
        }
    }
    if(m_imgPath.size()==0) return;

    _mImg = QImage(m_imgPath.at(0));
    ui->label->setText(QString("%1/%2").arg(m_curIndex).arg(m_imgPath.size()));

    if(_mImg.width() <= ui->labelImgTest->width() &&
            _mImg.height() <= ui->labelImgTest->height()){
        ui->labelNormalWMDemo->setPixmap(QPixmap::fromImage(_mImg));
    }else{
        ui->labelNormalWMDemo->setPixmap(QPixmap::fromImage(_mImg).scaled(ui->labelNormalWMDemo->size(),Qt::KeepAspectRatio));
    }
}


void Widget::on_btnNormalToTest_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

//处理上一张图片
void Widget::on_btnPre_clicked()
{
    if(m_curIndex<=1) return;

    m_curIndex--;

    ui->label->setText(QString("%1/%2").arg(m_curIndex).arg(m_imgPath.size()));

    _mImg = QImage(m_imgPath.at(m_curIndex-1));

    if(_mImg.width() <= ui->labelImgTest->width() &&
            _mImg.height() <= ui->labelImgTest->height()){
        ui->labelNormalWMDemo->setPixmap(QPixmap::fromImage(_mImg));
    }else{
        ui->labelNormalWMDemo->setPixmap(QPixmap::fromImage(_mImg).scaled(ui->labelNormalWMDemo->size(),Qt::KeepAspectRatio));
    }
}


void Widget::on_btnNext_clicked()
{
    if(m_curIndex>=m_imgPath.size()) return;

    m_curIndex++;
    ui->label->setText(QString("%1/%2").arg(m_curIndex).arg(m_imgPath.size()));

    _mImg = QImage(m_imgPath.at(m_curIndex-1));

    if(_mImg.width() <= ui->labelImgTest->width() &&
            _mImg.height() <= ui->labelImgTest->height()){
        ui->labelNormalWMDemo->setPixmap(QPixmap::fromImage(_mImg));
    }else{
        ui->labelNormalWMDemo->setPixmap(QPixmap::fromImage(_mImg).scaled(ui->labelNormalWMDemo->size(),Qt::KeepAspectRatio));
    }
}

//将处理后的图片保存
void Widget::on_btnNormalSave_clicked()
{
    QString path = m_imgPath.at(m_curIndex-1);
    QFileInfo fi(path);

    qDebug()<<"[Debug] save file to path"<<fi.baseName();
    QImage img = blindwm->getQImageResult();
    img.save(fi.baseName() + "1.png");
    //_mImg.save(fi.baseName() + "1.webp");
}

//字体颜色
void Widget::on_btnNormalColor_clicked()
{
    _mNormalColor= QColorDialog::getColor();
    ui->btnNormalColor->setStyleSheet(QString("background:%1").arg(_mNormalColor.name()));
    createNormalWM();
    createBlindWM();
}


void Widget::on_spinBoxNormalFontSize_valueChanged(int v)
{
    if(v<=0) return;

    ui->spinBoxNormalH->setValue(v*2);
    ui->spinBoxNormalW->setValue(v*20);
    createNormalWM();
    createBlindWM();
}


void Widget::on_spinBoxNormalX_valueChanged(int v)
{
    if(v<=0) return;
    createNormalWM();
    createBlindWM();
}


void Widget::on_spinBoxNormalY_valueChanged(int v)
{
    if(v<=0) return;
    createNormalWM();
    createBlindWM();
}


void Widget::on_spinBoxNormalW_valueChanged(int v)
{
    if(v<=0) return;
    createNormalWM();
    createBlindWM();
}


void Widget::on_spinBoxNormalH_valueChanged(int v)
{
    if(v<=0) return;
    createNormalWM();
    createBlindWM();
}


void Widget::on_checkNormalRect_stateChanged(int v)
{
    Q_UNUSED(v)
    createNormalWM();
    createBlindWM();
}


void Widget::on_spinBlindPosX_valueChanged(int v)
{
    if(v<=0) return;
    createBlindWM();
}


void Widget::on_spinBlindPosY_valueChanged(int v)
{
    if(v<=0) return;
    createBlindWM();
}


void Widget::on_spinBlindThickness_valueChanged(int v)
{
    if(v<=0) return;
    createBlindWM();
}


void Widget::on_spinBlindScale_valueChanged(double v)
{
    if(v==0.0) return;
    createBlindWM();
}

//打开已经加盲水印的图片
void Widget::on_btnOpenImgWM_clicked()
{

}

//解析图片盲水印
void Widget::on_btnParse_clicked()
{
    QString blindImg = ui->lineWMImgPath->text();
    if(blindImg.isEmpty()){
        qDebug()<<"Please input image.";
        return;
    }

    QImage img = blindwm->getQImageWatermark(blindImg);
    if(img.width() <= ui->labelImgTest->width() &&
            img.height() <= ui->labelImgTest->height()){
        ui->labelImgTest->setPixmap(QPixmap::fromImage(img));
    }else{
        ui->labelImgTest->setPixmap(QPixmap::fromImage(img).scaled(ui->labelImgTest->size(),Qt::KeepAspectRatio));
    }
}


void Widget::on_btnTestToWM_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Widget::on_spinBoxNormalOpacity_valueChanged(int arg1)
{
    if(v<=0) return;
    createNormalWM();
}

