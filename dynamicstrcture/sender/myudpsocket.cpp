#include "myudpsocket.h"

MyUdpSocket::MyUdpSocket(QString host, int port)
{
    mUdpSocket = new QUdpSocket;
    mData.clear();
    this->host=host;
    this->port=port;
}

MyUdpSocket::~MyUdpSocket()
{
    if(isRunning()){
        this->quit();
    }
    mUdpSocket->deleteLater();
}

void MyUdpSocket::setData(QByteArray data)
{
    mData = data;
}

void MyUdpSocket::run()
{
    while(!isInterruptionRequested()){
        qDebug()<<mData;
        mUdpSocket->writeDatagram(mData,QHostAddress(this->host),this->port);
        qDebug()<<"send.";
    }
}

