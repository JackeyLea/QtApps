#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    curRow(-1)
{
    ui->setupUi(this);

    supportType<<"char"<<"int"<<"uint"
              <<"float"<<"double"<<"long"<<"ulong";

    //ui tableview
    model = new QStandardItemModel(ui->tableView);
    ComboDelegate *cb = new ComboDelegate(this);

    initModel();
    ui->tableView->setItemDelegateForColumn(1,cb);
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);//select row
    ui->tableView->setSelectionMode(QTableView::SingleSelection);//select one row
    ui->tableView->setEditTriggers(QTableView::DoubleClicked);//double click to editor

    // click blank area
    //connect(ui->tableView,&MyTableView::clickBlankArea,this,&MainWindow::on_tableView_clickedBlankArea);
}

MainWindow::~MainWindow()
{
    model->deleteLater();
    delete ui;
}

void MainWindow::initModel()
{
    model->setColumnCount(3);
    model->setHorizontalHeaderItem(0,new QStandardItem("id"));
    model->setHorizontalHeaderItem(1,new QStandardItem("type"));
    model->setHorizontalHeaderItem(2,new QStandardItem("name"));
    model->setHorizontalHeaderItem(3,new QStandardItem("size(bits)"));
}

void MainWindow::on_tableView_clickedBlankArea()
{
    qDebug()<<"blank area clicked.";
    curRow = -1;
}

//add one row into tableview
void MainWindow::on_btnAdd_clicked()
{
    int row = model->rowCount();
    qDebug()<<"Current tableview row cnt is: "<<row;
    QList<QStandardItem*> newRow;
    newRow<<new QStandardItem(QString::number(row))
         <<new QStandardItem("char")
        <<new QStandardItem("default")
       <<new QStandardItem("8");
    model->appendRow(newRow);
}

void MainWindow::on_btnRemove_clicked()
{
    if(curRow<0){
        qDebug()<<"Please select one row to remove.";
        return;
    }

    model->removeRow(curRow);
    qDebug()<<curRow<<"row removed.";
}

void MainWindow::on_btnSave_clicked()
{
    int rowCnt = model->rowCount();
    if(rowCnt==0){
        QMessageBox::warning(this,QObject::tr("Error"),QObject::tr("No data to save."));
        return;
    }

    //name column cannot be same
    if(rowCnt!=1){
        for(int i=0;i<rowCnt-1;++i){
            for(int j=i+1;j<rowCnt;++j){
                QString top = model->data(model->index(i,2)).toString();
                QString down = model->data(model->index(j,2)).toString();
                if(top==down){
                    QMessageBox::information(this,QObject::tr("Error"),QObject::tr(QString("%1 and %2 has same name.").arg(i).arg(j).toLocal8Bit()));
                    return;
                }
            }
        }
    }

    QString filePath = QFileDialog::getSaveFileName(this,QObject::tr("Save"),
                                                    QString(),
                                                    QString("*.xml"));
    if(filePath.isEmpty()){
        qDebug()<<"Save nothing.";
        return;
    }
    qDebug()<<"save sturct to file"<<filePath;

    bool isValid=true;

    QDomDocument doc;  //QDomDocument类
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("struct");//创建根节点  QDomElemet元素
    doc.appendChild(root);//添加根节点

    for(int i=0;i<rowCnt;i++){
        QDomElement strNode = doc.createElement("node");
        root.appendChild(strNode);

        //id
        QDomElement strId = doc.createElement("id");//创建元素节点
        strNode.appendChild(strId);//添加元素节点到根节点
        QDomText strIdText = doc.createTextNode(model->data(model->index(i,0)).toString());//创建元素文本
        strId.appendChild(strIdText);//添加元素文本到元素节点

        //type
        QDomElement strType = doc.createElement("type");//创建元素节点
        strNode.appendChild(strType);//添加元素节点到根节点
        QString typeInfo = model->data(model->index(i,1)).toString();
        QDomText strTypeText = doc.createTextNode(typeInfo);//创建元素文本
        if(!supportType.contains(typeInfo)){
            QMessageBox::warning(this,QObject::tr("Type Error"),QObject::tr("No such type."));
            isValid=false;
            break;
        }
        strType.appendChild(strTypeText);//添加元素文本到元素节点

        //name
        QDomElement strName = doc.createElement("name");//创建元素节点
        strNode.appendChild(strName);//添加元素节点到根节点
        QDomText strNameText = doc.createTextNode(model->data(model->index(i,2)).toString());//创建元素文本
        strName.appendChild(strNameText);//添加元素文本到元素节点

        //name
        QDomElement strSize = doc.createElement("size");//创建元素节点
        strNode.appendChild(strSize);//添加元素节点到根节点
        QDomText strSizeText = doc.createTextNode(model->data(model->index(i,3)).toString());//创建元素文本
        strSize.appendChild(strSizeText);//添加元素文本到元素节点
    }
    if(isValid){
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
            return;
        QTextStream out(&file);
        out.setCodec("UTF-8");
        doc.save(out, 4, QDomNode::EncodingFromTextStream);
        file.close();
        QMessageBox::information(this,QObject::tr("Tips"),QObject::tr("Saved."));
    }
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    curRow = index.row();
    qDebug()<<"Current select row is: "<<curRow;
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    int col = index.column();
    qDebug()<<"Current clicked cell is: "<<row<<col;
}

void MainWindow::on_btnOpen_clicked()
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
        model->appendRow(itemList);
        node = node.nextSibling();//读取下一个兄弟节点
    }
}
