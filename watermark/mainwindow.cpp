#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , normalwm(new NormalWatermark)
    , blindwm(new BlindWatermark)
    , _mNormalColor(Qt::red)
    , _mBlindColor(Qt::white)
{
    ui->setupUi(this);
    setAcceptDrops(true);

    //设置界面
    ui->btnColor->setStyleSheet(QString("background:%1").arg(_mNormalColor.name()));
    ui->spinBoxFontSize->setValue(20);
    ui->spinBoxX->setValue(20);

    QString tmp = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    ui->lineWMCtx->setText("feater.top "+tmp);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage(QString filePath)
{
    _mFilePath = filePath;
    ui->lineImgPath->setText(filePath);
    //显示原图
    _mImg = QImage(filePath);
    ui->labelWMDemo->setPixmap(QPixmap::fromImage(_mImg));

    int w = _mImg.width() /4;
    int h = _mImg.height() /7;
    ui->spinBoxW->setValue(w);
    ui->spinBoxH->setValue(h);
    ui->spinBoxY->setValue(_mImg.height() - h);
    //盲水印部分
    ui->spinBlindPosX->setValue(w);
    ui->spinBlindPosY->setValue(h);
}

//插件普通水印
void MainWindow::createNormalWM()
{
    if(_mImg.isNull()){
        qDebug()<<("Please input an image.");
        return;
    }

    normalwm->setImage(_mImg);
    normalwm->setColor(_mNormalColor);
    normalwm->setFontPointSize(ui->spinBoxFontSize->value());
    normalwm->setFrameVisible(ui->checkBoxRect->isChecked());
    normalwm->setPosH(ui->spinBoxH->value());
    normalwm->setPosW(ui->spinBoxW->value());
    normalwm->setPosY(ui->spinBoxY->value());
    normalwm->setPosX(ui->spinBoxX->value());
    normalwm->setWatermarkCtx(ui->lineWMCtx->text());
    _mNormalResult = normalwm->result();
    if(_mNormalResult.width() <= ui->labelWMDemo->width() &&
            _mNormalResult.height() <= ui->labelWMDemo->height()){
        ui->labelWMDemo->setPixmap(QPixmap::fromImage(_mNormalResult));
    }else{
        ui->labelWMDemo->setPixmap(QPixmap::fromImage(_mNormalResult).scaled(ui->labelWMDemo->size(),Qt::KeepAspectRatio));
    }
}

//插件盲水印
void MainWindow::createBlindWM()
{
    if(_mNormalResult.isNull()){
        qDebug()<<("Please input an image.");
        return;
    }

    blindwm->setImage(_mNormalResult);//设置需要处理的图像
    blindwm->setWatermarkCtx(ui->lineBlindWMCtx->text());//水印内容
    blindwm->setColor(_mBlindColor);//水印颜色，DFT是灰度图，所以除了黑白色应该没什么用
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

void MainWindow::resizeEvent(QResizeEvent *e)
{
    ui->frame->setGeometry(0,0,e->size().width(),30);
    ui->frame_2->setGeometry(0,30,e->size().width()/2,e->size().height()-30);
    ui->frame_3->setGeometry(e->size().width()/2,30,e->size().width()/2,e->size().height()-30);

    if(_mNormalResult.width() <= ui->labelWMDemo->width() &&
            _mNormalResult.height() <= ui->labelWMDemo->height()){
        ui->labelWMDemo->setPixmap(QPixmap::fromImage(_mNormalResult));
    }else{
        ui->labelWMDemo->setPixmap(QPixmap::fromImage(_mNormalResult).scaled(ui->labelWMDemo->size(),Qt::KeepAspectRatio));
    }

    if(_mBlindResult.width() <= ui->labelBlindDemo->width() &&
            _mBlindResult.height() <= ui->labelBlindDemo->height()){
        ui->labelBlindDemo->setPixmap(QPixmap::fromImage(_mBlindResult));
    }else{
        ui->labelBlindDemo->setPixmap(QPixmap::fromImage(_mBlindResult).scaled(ui->labelBlindDemo->size(),Qt::KeepAspectRatio));
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasUrls()){
        e->acceptProposedAction();
    }else{
        e->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    const QMimeData *mimeData = e->mimeData();
    if(mimeData->hasUrls()){
        QList<QUrl> urlList = mimeData->urls();
        QString fileName = urlList.at(0).toLocalFile();
        if(!fileName.isEmpty()){
            loadImage(fileName);
        }
    }
}

//选择需要处理的图片完整路径
void MainWindow::on_btnOpen_clicked()
{
    _mFilePath = QFileDialog::getOpenFileName(this,QStringLiteral("Open"),QString(),QString("*"));
    if(_mFilePath.isEmpty()){
        ui->lineImgPath->setText(QStringLiteral("Selected nothing."));
        return;
    }

    loadImage(_mFilePath);
}

void MainWindow::on_btnColor_clicked()
{
    _mNormalColor = QColorDialog::getColor(Qt::red,this,QStringLiteral("Color"));
    ui->btnColor->setStyleSheet(QString("background:%1").arg(_mNormalColor.name()));
    createNormalWM();
    createBlindWM();
}

void MainWindow::on_btnBlindColor_clicked()
{
    _mBlindColor = QColorDialog::getColor(Qt::red,this,QStringLiteral("Color"));
    ui->btnBlindColor->setStyleSheet(QString("background:%1").arg(_mBlindColor.name()));
    createBlindWM();
}

void MainWindow::on_spinBoxFontSize_valueChanged(int v)
{
    if(v<=0) return;
    createNormalWM();
    createBlindWM();
}

void MainWindow::on_lineWMCtx_editingFinished()
{
    if(ui->lineWMCtx->text().isEmpty()) return;
    createNormalWM();
    createBlindWM();
}

void MainWindow::on_checkBoxRect_stateChanged(int arg1)
{
    Q_UNUSED(arg1)
    createNormalWM();
    createBlindWM();
}

void MainWindow::on_spinBoxX_valueChanged(int v)
{
    if(v<=0) return;
    createNormalWM();
    createBlindWM();
}

void MainWindow::on_spinBoxY_valueChanged(int v)
{
    if(v<=0) return;
    createNormalWM();
    createBlindWM();
}

void MainWindow::on_spinBoxW_valueChanged(int v)
{
    if(v<=0) return;
    createNormalWM();
    createBlindWM();
}

void MainWindow::on_spinBoxH_valueChanged(int v)
{
    if(v<=0) return;
    createNormalWM();
    createBlindWM();
}

void MainWindow::on_btnSave_clicked()
{
    QDir dir(_mFilePath);
    qDebug()<<"[Debug] save file to path"<<dir.dirName();
    QImage img = blindwm->getQImageResult();
    img.save(dir.dirName());
}

void MainWindow::on_lineBlindWMCtx_editingFinished()
{
    if(ui->lineBlindWMCtx->text().isEmpty()) return;
    createBlindWM();
}

void MainWindow::on_spinBlindPosX_valueChanged(int v)
{
    if(v<=0) return;
    createBlindWM();
}

void MainWindow::on_spinBlindPosY_valueChanged(int v)
{
    if(v<=0) return;
    createBlindWM();
}

void MainWindow::on_spinBlindThickness_valueChanged(int v)
{
    if(v<=0) return;
    createBlindWM();
}

void MainWindow::on_btnResultTest_clicked()
{
    if(_mFilePath.isEmpty()){
        qDebug()<<"Please input image.";
        return;
    }
    QDir dir(_mFilePath);
    QImage img = blindwm->getQImageWatermark(dir.dirName());

    if(img.width() <= ui->labelBlindDemo->width() &&
            img.height() <= ui->labelBlindDemo->height()){
        ui->labelBlindDemo->setPixmap(QPixmap::fromImage(img));
    }else{
        ui->labelBlindDemo->setPixmap(QPixmap::fromImage(img).scaled(ui->labelBlindDemo->size(),Qt::KeepAspectRatio));
    }
}


void MainWindow::on_spinBlindScale_valueChanged(double v)
{
    if(v==0.0) return;
    createBlindWM();
}

