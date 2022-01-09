#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QtMath>
#include <QTextCodec>

#include "zlib.h"

int getWordSize(QByteArray ba,int index){
    int i=0;
    while(ba.at(index+i)!='\0'){
        i++;
    }
    return i;
}

QString getCodedData(QByteArray ba){
    return QString::fromUtf8(ba);
}

int main(int argc, char* argv[])
{
    QFile file("CET6.idx");
    if(!file.exists()){
        qDebug()<<"File does not exists."<<file.fileName();
        return -1;
    }
    if(!file.open(QFile::ReadOnly)){
        qDebug()<<"File cannot open: "<<file.fileName();
        return -1;
    }
    QByteArray ba = file.readAll();
    file.close();
    qDebug()<<"idx size is: "<<ba.size();

    QFile filedict("CET6.dict");
    if(!filedict.exists()){
        qDebug()<<"File does not exists."<<filedict.fileName();
        return -1;
    }
    if(!filedict.open(QFile::ReadOnly)){
        qDebug()<<"File cannot open: "<<filedict.fileName();
    }
    QByteArray dictData = filedict.readAll();
    filedict.close();
    qDebug()<<"dict size is: "<<dictData.size();

    int index=0,len=0;
    int cnt=10;
    while(index<ba.size() && cnt--){
        len=getWordSize(ba,index);
        uint32_t pos = (uchar)ba[index+len+4]+(ba[index+len+3]<<8)+(ba[index+len+2]<<16)+(ba[index+len+1]<<24);
        uint32_t size = (uchar)ba[index+len+8]+(ba[index+len+7]<<8)+(ba[index+len+6]<<16)+(ba[index+len+5]<<24);

        //qDebug()<<ba.mid(index,len)<<ba.mid(index+len+1,4)<<ba.mid(index+len+5,4);
        qDebug()<<ba.mid(index,len)<<pos<<size;
        qDebug()<<getCodedData(dictData.mid(pos,size));
        index+= len + 9;
    }
}
