#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    //myUdp(new MyUdpSocket("127.0.0.1",1234)),
    timer(new QTimer)
{
    ui->setupUi(this);

    QTime t = QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);

    mUdpSocket = new QUdpSocket;
    mData.clear();
    host=QHostAddress("127.0.0.1");
    port=1234;
    //this->port;

    //ui tableview
    model = new QStandardItemModel(ui->tableView);

    initModel();
    ui->tableView->setModel(model);
    //ui->tableView->setItemDelegateForColumn(1,cb);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);//select row
    ui->tableView->setSelectionMode(QTableView::SingleSelection);//select one row
    ui->tableView->setEditTriggers(QTableView::DoubleClicked);//double click to editor

    timer->setInterval(1000);
    connect(timer,&QTimer::timeout,this,&Widget::sltUpdateData);
}

Widget::~Widget()
{
    delete timer;
    delete model;
    //delete myUdp;
    delete ui;
}

void Widget::initModel()
{
    model->setColumnCount(6);
    model->setHorizontalHeaderItem(0,new QStandardItem("id"));
    model->setHorizontalHeaderItem(1,new QStandardItem("type"));
    model->setHorizontalHeaderItem(2,new QStandardItem("name"));
    model->setHorizontalHeaderItem(3,new QStandardItem("size(bits)"));
    model->setHorizontalHeaderItem(4,new QStandardItem("value"));
    model->setHorizontalHeaderItem(5,new QStandardItem("hex"));
}

//random_max 0-32767 2B

inline uchar getChar(){
    return qrand()%254;//0-0xff
}//1B
inline int getInt(){
    return (int)(qrand()+qrand()-32767);
}//4B
inline uint getUInt(){
    return (uint)(qrand()+qrand());
}//4B
inline float getFloat(){
    return (float)((qrand()+qrand()-32767)/0.3);
}//4B
inline double getDouble(){
    return (double)(qrand()+qrand()+qrand()+qrand());
}//8B
inline long getLong(){
    return (long)(qrand()+qrand());
}//8B
inline ulong getULong(){
    return (ulong)(qrand()+qrand());
}//8B

QString getHex(char n,uint c,bool isSigned=false){
    QString r;

    QByteArray ba;
    while(n){
        ba.append(n%2);
    }
    //swap sequence
    for(int i=0;i<ba.size()/2;++i){
        char t=ba.at(i);
        ba.at(i)=ba.at(ba.size()-1-i);
        ba.at(ba.size()-1-i)=t;
    }
    qDebug()<<ba;

    return r;
}

void Widget::sltUpdateData()
{
    if(typeMap.isEmpty()) return;
    //pak package
    QByteArray ba;
    int i=0;
    int bytesSize=0;
    while(i<typeMap.size()){
        QString typeName = typeMap.at(i).first;
        int size = typeMap.at(i).second;

        uint type=0;

        if(typeName=="char"){
            type=0;
        }else if(typeName=="int"){
            type=1;
        }else if(typeName=="uint"){
            type=2;
        }else if(typeName=="float"){
            type=3;
        }else if(typeName=="double"){
            type=4;
        }else if(typeName=="long"){
            type=5;
        }else if(typeName=="ulong"){
            type=6;
        }

        switch(type){
        case 0:{
            uchar a= getChar();
            QString t=QString("%1").arg((int)a,1,16,QLatin1Char('0'));
            ba.append(t);
            qDebug()<<"sizeof is: "<<sizeof(uchar);
            model->setData(model->index(i,4),a);
            model->setData(model->index(i,5),t);
            break;}
        case 1:{
            int a= getInt();
            int a_int16 = *(int*)&a;
            QString t=QString("%1").arg(a_int16,2,16,QLatin1Char('0'));
            qDebug()<<"sizeof is: "<<sizeof(int);
            ba.append(t);
            model->setData(model->index(i,4),a);
            model->setData(model->index(i,5),t);
            break;}
        case 2:{
            uint a= getUInt();
            QString t=QString("%1").arg((uint)a,2,16,QLatin1Char('0'));
            ba.append(t);
            model->setData(model->index(i,4),a);
            model->setData(model->index(i,5),t);
            break;}
        case 3:{//float -> hex
            float a= getFloat();
            uint f_uint = *(uint*)&a;
            QString t=QString("%1").arg(f_uint,4,16,QLatin1Char('0'));
            ba.append(t);
            model->setData(model->index(i,4),a);
            model->setData(model->index(i,5),t);
            break;}
        case 4:{//double -> hex
            double a= getDouble();
            qulonglong d_long = *(qulonglong*)&a;
            QString t=QString("%1").arg(d_long,8,16,QLatin1Char('0'));
            ba.append(t);
            model->setData(model->index(i,4),a);
            model->setData(model->index(i,5),t);
            break;}
        case 5:{
            long a= getLong();
            QString t=QString("%1").arg((long)a,8,16,QLatin1Char('0'));
            ba.append(t);
            model->setData(model->index(i,4),a);
            model->setData(model->index(i,5),t);
            break;}
        case 6:{
            ulong a= getULong();
            QString t=QString("%1").arg((ulong)a,8,16,QLatin1Char('0'));
            ba.append(t);
            model->setData(model->index(i,4),(qulonglong)a);
            model->setData(model->index(i,5),t);
            break;}
        }
        qDebug()<<ba.size();
        ++i;
        bytesSize+=size/8;
    }
    qDebug()<<"bytes is: "<<bytesSize;
    qDebug()<<ba.size()<<ba;
    mUdpSocket->writeDatagram(ba,host,this->port);
    qDebug()<<"send.";
}

void Widget::on_btnOpen_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,QObject::tr("Open"),
                                                    QString(),
                                                    QString("*.xml\n*"));
    if(!filePath.isEmpty()){
        qDebug()<<"Current select file is: "<<filePath;
    }else{
        QMessageBox::information(this,QObject::tr("Warning"),QObject::tr("Selected nothing."));
        return;
    }
    ui->lineFilePath->setText(filePath);

    QDomDocument doc;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    if (!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();//读取根节点
    QDomNode node = root.firstChild();//读取第一个子节点   QDomNode 节点

    //clear all data and display new data
    model->clear();
    initModel();

    typeMap.clear();

    while (!node.isNull())
    {
        QDomNodeList nodeList = node.childNodes();  //节点元素名称
        int cnt = nodeList.count();
        if(cnt!=4){
            QMessageBox::warning(this,QObject::tr("error"),QObject::tr("file format is wrong."));
            break;
        }
        QList<QStandardItem*> itemList;
        itemList<<new QStandardItem(nodeList.at(0).toElement().text())
               <<new QStandardItem(nodeList.at(1).toElement().text())
               <<new QStandardItem(nodeList.at(2).toElement().text())
              <<new QStandardItem(nodeList.at(3).toElement().text());
        //init type map
        //typeMap.insert(nodeList.at(1).toElement().text(),nodeList.at(3).toElement().text().toInt());
        typeMap.append(qMakePair(nodeList.at(1).toElement().text(),nodeList.at(3).toElement().text().toInt()));
        model->appendRow(itemList);
        node = node.nextSibling();//读取下一个兄弟节点
    }
}

void Widget::on_btnClear_clicked()
{
    ui->lineFilePath->clear();
}

void Widget::on_btnSend_clicked()
{
//    if(!myUdp->isRunning()){
//        myUdp->start();
//        timer->start();
//    }
    timer->start();
}

void Widget::on_btnStop_clicked()
{
//    if(myUdp->isRunning()){
//        myUdp->quit();
//        timer->stop();
//    }
    timer->stop();
}
