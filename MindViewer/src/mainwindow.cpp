#include "mainwindow.h"
#include "ui_mainwindow.h"

///////////////////////////以下内容来自官方参考代码//////////////////////////////////////

/* Decoder states (Packet decoding) */
#define PARSER_STATE_NULL           0x00  /* NULL state */
#define PARSER_STATE_SYNC           0x01  /* Waiting for SYNC byte */
#define PARSER_STATE_SYNC_CHECK     0x02  /* Waiting for second SYNC byte */
#define PARSER_STATE_PAYLOAD_LENGTH 0x03  /* Waiting for payload[] length */
#define PARSER_STATE_CHKSUM         0x04  /* Waiting for chksum byte */
#define PARSER_STATE_PAYLOAD        0x05  /* Waiting for next payload[] byte */

/* Decoder states (2-byte raw decoding) */
#define PARSER_STATE_WAIT_HIGH      0x06  /* Waiting for high byte */
#define PARSER_STATE_WAIT_LOW       0x07  /* High r'cvd.  Expecting low part */

/* Other constants */
#define PARSER_SYNC_BYTE            0xAA  /* Syncronization byte */
#define PARSER_EXCODE_BYTE          0x55  /* EXtended CODE level byte */

/////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionGraph->setChecked(true);//默认使用图形模式
    resize(800,820);

    buff.clear();

    retriverWgt = new Retriver;

    connect(retriverWgt,&Retriver::rawData,this,&MainWindow::sltReceiveData);//实际串口数据

    ui->graphEEG->setGeometry(0,0,800,532);
    ui->graphCommon->setGeometry(0,532,800,266);
}

MainWindow::~MainWindow()
{
    if(gen!=nullptr){//停止模拟数据
        delete gen;
    }
    delete retriverWgt;
    delete ui;
}

//使用状态机解析原始数据
int MainWindow::parserData(QByteArray ba, bool &raw, short &rawValue, bool &common, bool &eeg, struct _eegPkt &pkt)
{
    raw=false;//此数据包是否包含原始数据
    eeg=false;//此数据包是否包含eeg数据
    common=false;//此数据包是否包含注意力/冥想等数据

    if(ba.isEmpty()) return -1;//如果没有数据就直接退出

    buff.append(ba);//将数据添加到处理缓冲区

    /////////////////处理开始/////////////////////////////////////
    int cnt=0;
    uchar state=PARSER_STATE_SYNC;
    uchar payloadLength;
    uchar payloadSum;
    while(buff.size()){
        //状态机处理
        switch(state){
        case PARSER_STATE_SYNC://第一个同步字节
            if((uchar)buff[0]==PARSER_SYNC_BYTE){
                //qDebug()<<"parser first aa "<<i<<(uchar)buff[i];
                state=PARSER_STATE_SYNC_CHECK;
            }
            buff.remove(0,1);//一直删除直到有0xaa
            break;
        case PARSER_STATE_SYNC_CHECK:
            if((uchar)buff[0]==PARSER_SYNC_BYTE){//包第二个0xaa
                //qDebug()<<"parser second aa "<<i<<(uchar)buff[i];
                state=PARSER_STATE_PAYLOAD_LENGTH;//准备解析负载长度
            }else{
                state=PARSER_STATE_SYNC;
            }
            buff.remove(0,1);//不管什么状态，都要把这个删除
            break;
        case PARSER_STATE_PAYLOAD_LENGTH:
            payloadLength=(uchar)buff[0];//接下来是长度
            //qDebug()<<"payloadlength"<<payloadLength;
            if(payloadLength>=170 || payloadLength<=0){//如果程度大于170就丢弃此包并查找下一个0xaa 0xaa
                qDebug()<<"this package payloadLength(the 3rd value) is wrong";
                state = PARSER_STATE_SYNC;
            }else{
                payloadSum=0;
                //qDebug()<<"parser payload length "<<i<<(uchar)buff[i];
                state=PARSER_STATE_CHKSUM;//准备解析有效数据
            }
            buff.remove(0,1);
            break;
        case PARSER_STATE_CHKSUM:
        {
            //首先校验数据是否有效
            for(int j=0;j<payloadLength;j++){
                payloadSum+=(uchar)buff[j];
            }
            payloadSum &= 0xff;
            payloadSum = ~payloadSum & 0xff;

            if(payloadSum!=(uchar)buff[payloadLength]){
                //如果与校验值不同就丢弃此包数据
                qDebug()<<"Checksum failed.";
                return -1;
            }
            //qDebug()<<"get data check sum is: "<<z<<(uchar)buff[z];
            //qDebug()<<"parser check sum "<<i;
            state=PARSER_STATE_PAYLOAD;
            break;
        }
        case PARSER_STATE_PAYLOAD://解析数据
        {
            if(cnt==payloadLength){
                buff.remove(0,1);//如果数据已经没了，那么还剩下一个校验值
                return 0;
            }
            if((uchar)buff[0]==0x01){//电源值，最大为127
                common=true;
                pkt.power=(uchar)buff[1];
                state=PARSER_STATE_PAYLOAD;
                buff.remove(0,2);
                cnt+=2;
            }else if((uchar)buff[0]==0x02){//数据信号强度值
                //qDebug()<<"signal value "<<(uchar)buff[1];
                common=true;
                pkt.signal=(uchar)buff[1];
                state=PARSER_STATE_PAYLOAD;
                buff.remove(0,2);
                cnt+=2;
            }else if((uchar)buff[0]==0x03){
                //ego上的心跳强度0-255
            }else if((uchar)buff[0]==0x04){//注意力值
                //qDebug()<<"attention value "<<(uchar)buff[1];
                common=true;
                pkt.attention=(uchar)buff[1];
                state=PARSER_STATE_PAYLOAD;
                buff.remove(0,2);
                cnt+=2;
            }else if((uchar)buff[0]==0x05){//冥想值
                //qDebug()<<"meditation value "<<(uchar)buff[1];
                common=true;
                pkt.meditation=(uchar)buff[1];
                state=PARSER_STATE_PAYLOAD;
                buff.remove(0,2);
                cnt+=2;
            }else if((uchar)buff[0]==0x06){//8bit raw value
            }else if((uchar)buff[0]==0x07){
                //raw_marker 固定值为0
            }else if((uchar)buff[0]==0x80 && (uchar)buff[1]==0x02){//16位原始数据
                raw=true;
                rawValue=((uchar)buff[2]<<8)|(uchar)buff[3];
                buff.remove(0,5);
                return 0;
            }else if((uchar)buff[0]==0x81 && (uchar)buff[1]==0x20){
                //eeg_power 8个大端四字节
            }else if((uchar)buff[0]==0x83 && (uchar)buff[1]==0x18){//eeg数据部分
                //0x83标志eeg部分开始，下一位表示为eeg部分程度默认为0x18,8个大端三字节
                //qDebug()<<"parser eeg data "<<i<<buff[i];
                //qDebug()<<"parser eeg length "<<(uchar)buff[i+1];
                eeg=true;
                //tgam数据默认为大端
                pkt.delta =((uint)buff[2]<<16)|((uint)buff[3]<<8)|((uint)buff[4]);
                pkt.theta =((uint)buff[5]<<16)|((uint)buff[6]<<8)|((uint)buff[7]);
                pkt.lowAlpha =((uint)buff[8]<<16)|((uint)buff[9]<<8)|((uint)buff[10]);
                pkt.highAlpha =((uint)buff[11]<<16)|((uint)buff[12]<<8)|((uint)buff[13]);
                pkt.lowBeta =((uint)buff[14]<<16)|((uint)buff[15]<<8)|((uint)buff[16]);
                pkt.highBeta =((uint)buff[17]<<16)|((uint)buff[18]<<8)|((uint)buff[19]);
                pkt.lowGamma =((uint)buff[20]<<16)|((uint)buff[21]<<8)|((uint)buff[22]);
                pkt.midGamma =((uint)buff[23]<<16)|((uint)buff[24]<<8)|((uint)buff[25]);
                state=PARSER_STATE_PAYLOAD;
                buff.remove(0,26);
                cnt+=26;
            }else if((uchar)buff[0]==0x86 && (uchar)buff[1]==0x02){
                //两个大端字节表示R峰的间隔
            }
            break;
        }
        case PARSER_STATE_NULL:
            break;
        default:
            break;
        }
    }

    return 0;
}

void MainWindow::resizeEvent(QResizeEvent */*event*/)
{
    int width = ui->stackedWidget->width();
    int height = ui->stackedWidget->height();
    if(height <700) return;//窗口必须大于700
    ui->graphEEG->setGeometry(0,0,width,height*2/3);
    ui->graphCommon->setGeometry(0,height*2/3,width,height/3);
    update();
}

//接收原始数据
void MainWindow::sltReceiveData(QByteArray ba)
{
    //如果是每秒512个包，眼睛跟不上，以1:10过滤
    //注意每512个包才有1个eeg包，不要把eeg包过滤了

    //16进制模式
    if(ui->actionHex->isChecked()){
        QString s;
        for(int i=0;i<ba.size();i++){
            QString ss,sss;
            sss=ss.asprintf("0x%02X ",(unsigned char)ba.at(i));
            s+=sss;
        }
        ui->textHex->appendPlainText(s);
    }else{//图形模式
        bool raw,eeg,common;
        short rawValue;
        struct _eegPkt pkt;
        pkt.init();
        if(parserData(ba,raw,rawValue,common,eeg,pkt)!=0){
            return;
        }
        if(common){
            ui->graphCommon->updateCommonData(pkt);//将数据发送到界面
            if(!isEEGResized){//绘图之后需要手动调整窗口大小，我们自己调整一下
                resize(ui->stackedWidget->currentWidget()->size().width()+40,ui->stackedWidget->currentWidget()->size().height());
                isEEGResized=true;
                update();
            }
        }
        if(eeg){
            ui->graphEEG->updateEEGData(pkt);//将数据发送到界面
            if(!isEEGResized){//绘图之后需要手动调整窗口大小，我们自己调整一下
                resize(ui->stackedWidget->currentWidget()->size().width()+40,ui->stackedWidget->currentWidget()->size().height());
                isEEGResized=true;
                update();
            }
        }
        if(raw){
            ui->graphEEG->updateRawData(rawValue);
            if(!isCommonResized){
                resize(ui->stackedWidget->currentWidget()->size().width()+40,ui->stackedWidget->currentWidget()->size().height());
                isCommonResized=true;
                update();
            }
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    exit(1);
}

void MainWindow::on_actionAbout_triggered()
{
    QString msg = QString("<h1>MindViewer</h1>"
                          "<h2>TGAM module tools</h2>"
                          "<h3>Author: JackeyLea</h3>"
                          "<h3>E-mail: 1768478912@qq.com</h3>"
                          "<h3>GitHub: https://github.com/JackeyLea/MindViewer</h3>"
                          "<h3>Gitee: https://gitee.com/JackeyLea/MindViewer</h3>"
                          "<h4>Do what you want but please obey the LGPL3 rules</h4>"
                          "<h4>And keep those message within application</h4>");
    QMessageBox::information(this, tr("About"), msg, QMessageBox::Ok);
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_actionGithub_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/JackeyLea/MindViewer"));
}

void MainWindow::on_actionSerialPort_triggered()
{
    if(ui->actionSerialPort->isChecked()){
        ui->actionTest->setChecked(false);
        on_actionTest_triggered();
        ui->graphCommon->CurveClear();
        ui->graphEEG->CurveClear();
        retriverWgt->showWgt();
    }else{
        retriverWgt->stopCOM();
    }
}

void MainWindow::on_actionTest_triggered()
{
    if(ui->actionTest->isChecked()){
        ui->actionSerialPort->setChecked(false);
        on_actionSerialPort_triggered();
        gen=new Generator();
        connect(gen,&Generator::sendData,this,&MainWindow::sltReceiveData);//模拟数据
    }else{
        if(gen){
            disconnect(gen,&Generator::sendData,this,&MainWindow::sltReceiveData);
            delete gen;
        }
    }
}

void MainWindow::on_actionHex_triggered(bool checked)
{
    if(checked){//文本模式
        ui->stackedWidget->setCurrentIndex(0);
        ui->actionGraph->setChecked(false);
        ui->textHex->clear();
    }else{
        ui->stackedWidget->setCurrentIndex(1);
        ui->actionGraph->setChecked(true);
        ui->textHex->clear();
    }
}

void MainWindow::on_actionGraph_triggered(bool checked)
{
    if(checked){
        ui->stackedWidget->setCurrentIndex(1);
        ui->actionHex->setChecked(false);
        ui->graphCommon->CurveClear();
        ui->graphEEG->CurveClear();
    }else{
        ui->stackedWidget->setCurrentIndex(0);
        ui->actionHex->setChecked(true);
    }
}
