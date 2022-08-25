#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileFullPath = QDir::currentPath();
    ui->lineUrl->setText(fileFullPath);
    ui->lineResultPath->setText(fileFullPath+QDir::separator()+"default.xml");
    fileCnt=0;
    dirCnt=0;

    //treeview
    treeModel=new QStandardItemModel(ui->treeView);
    QStringList headers;
    headers<<"Files"<<"Names";
    treeModel->setColumnCount(2);
    treeModel->setHorizontalHeaderLabels(headers);
    ui->treeView->setModel(treeModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::log(QString str)
{
    qDebug()<<str;
    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(str,10000);
}

void MainWindow::findAllFiles(QString dirPath)
{
    if(dirPath.isEmpty()) return;
    QDir dir(dirPath);
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach(QFileInfo fileInfo , fileInfoList){
        if(fileInfo.fileName() == "."||fileInfo.fileName()==".."){
            continue;
        }else if(fileInfo.isDir()){
            findAllFiles(fileInfo.filePath());
            int row=treeModel->rowCount();
            treeModel->setItem(row++,1,new QStandardItem("asdl"));
            dirCnt++;
        }else if(fileInfo.isFile()){
            bool isValid = false;
            if(fileInfo.suffix()=="html") isValid=true;
            QString crtFilePath = fileInfo.filePath();
            QString content = crtFilePath.remove(basePath+QDir::separator());
            QString title = fileInfo.fileName().split(".").at(0);
            addFilesNode(isValid,title,content);
            fileCnt++;
        }
    }
}

bool MainWindow::initXML()
{
    QDomProcessingInstruction instruction;
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("QtHelpProject");
    root.setAttribute("version","1.0");
    doc.appendChild(root);

    QDomElement ns=doc.createElement("namespace");
    QDomText text=doc.createTextNode(ui->lineNamespace->text());
    ns.appendChild(text);
    root.appendChild(ns);

    QDomElement vf = doc.createElement("virtualFolder");
    QDomText vfText = doc.createTextNode("doc");
    vf.appendChild(vfText);
    root.appendChild(vf);

    QDomElement fs = doc.createElement("filterSection");
    QDomElement toc = doc.createElement("toc");
    //索引内容
    fs.appendChild(toc);
    QDomElement kw = doc.createElement("keywords");
    //关键词
    fs.appendChild(kw);
    QDomElement files = doc.createElement("files");
    //所有文件
    fs.appendChild(files);
    root.appendChild(fs);

    log("The basic structure is completed!");

    return true;
}

bool MainWindow::addFilesNode(bool isValid,QString title,QString content="test")
{
    QDomElement root=doc.documentElement();
    //qDebug()<<root.nodeName();
    QDomNodeList nodeList = root.childNodes();
    for(int i=0;i<nodeList.count();i++){
        QDomNode node = nodeList.at(i);
        if(node.nodeName()!="filterSection"){
            continue;
        }
        //qDebug()<<node.nodeName();
        QDomNodeList childList = node.childNodes();
        //qDebug()<<childList.count();
        for(int j=0;j<childList.count();j++){
            QDomNode child  = childList.at(j);
            if(child.nodeName()=="toc"){
                if(!isValid) continue;
                QDomElement newNode = doc.createElement("section");
                newNode.setAttribute("title",title);
                newNode.setAttribute("ref",content);
                child.appendChild(newNode);
//            }
//            else if(child.nodeName()=="keywords"){
//                if(!isValid) continue;
//                QDomElement newNode = doc.createElement("keyword");
//                newNode.setAttribute("name",name);
//                newNode.setAttribute("ref",content);
//                child.appendChild(newNode);
            }else if(child.nodeName()=="files"){
                QDomNode newNode = doc.createElement("file");
                QDomText t=doc.createTextNode(content);
                newNode.appendChild(t);
                child.appendChild(newNode);
            }
        }
    }

    return true;
}

bool MainWindow::save2file(QString dstPath)
{
    if(dstPath.isEmpty()) throw "There must has dstination file path!";
    QFile file(dstPath);
    if(!file.open(QFile::WriteOnly)){
        file.close();
        return false;
    }

    QTextStream writeOut(&file);
    doc.save(writeOut,4);
    file.close();

    log("All data has been saved!");
    return true;
}

bool MainWindow::generateQch()
{
    QStringList cmdList;
    cmdList<<resultFullPath<<"-o"<<dstFullPath;
    QProcess pro;
    pro.start("qhelpgenerator",cmdList,QProcess::ReadWrite);
    pro.waitForFinished();
    log(pro.readAllStandardOutput());

    return true;
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit(0);
}

void MainWindow::on_actionAbout_triggered()
{
    QString msg = QString("<h1>QtHelp</h1>"
                          "<h2>用于快速生成Qt Assistant需要的帮助文档</h2>");
    QMessageBox::information(this,tr("About"),msg,QMessageBox::Ok);
}

void MainWindow::on_actionIndex_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/JackeyLea/QtHelp"));
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_btnFolder_clicked()
{
    basePath = QFileDialog::getExistingDirectory(this,tr("Open"),fileFullPath);
    if(basePath.isEmpty()) return;//如果没有选择文件夹就退出
    ui->lineUrl->setText(basePath);//将获取的文件夹地址显示出来
}

void MainWindow::on_btnFile_clicked()
{
    ui->lineResultPath->clear();
    QString resultPath = QFileDialog::getOpenFileName(this,tr("Open"),fileFullPath);
    if(resultPath.isEmpty()) return;//没有选择就退出
    resultFullPath = resultPath;//全局变量，保存文件的时候会用到
    ui->lineResultPath->setText(resultPath);//显示在界面上
}

void MainWindow::on_btnGo_clicked()
{
    if(basePath.isEmpty()) basePath=ui->lineUrl->text();
    log("Starting initialize xml file content!");
    if(ui->lineNamespace->text().isEmpty()){
        QMessageBox::information(this,"Error","Namespace cannot be empty",QMessageBox::Ok);
        return;
    }
    initXML();

    findAllFiles(ui->lineUrl->text());
    QString result=QString("There are %1 files and %2 dirs").arg(fileCnt).arg(dirCnt);
    log(result);

    if(resultFullPath.isEmpty()) resultFullPath=ui->lineResultPath->text();
    save2file(resultFullPath);

    dstFullPath = ui->lineUrl->text()+QDir::separator()+"opencv.qch";

    bool isOk = generateQch();
    if(isOk){
        QMessageBox::information(this,tr("Tip"),tr("All operation has been done!\nYou need to edit details by yourself."),QMessageBox::Ok);
    }else{
        QMessageBox::information(this,tr("Warning"),"Error in generate xml file.\n",QMessageBox::Ok);
    }
}
