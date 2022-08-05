#ifndef MYUDPSOCKET_H
#define MYUDPSOCKET_H

#include <QThread>
#include <QUdpSocket>
#include <QByteArray>

class MyUdpSocket : public QThread
{
public:
    MyUdpSocket(QString host,int port);
    ~MyUdpSocket();

    void setData(QByteArray data);

protected:
    void run();

private:
    QUdpSocket *mUdpSocket;

    QByteArray mData;

    QString host;
    int port;
};

#endif // MYUDPSOCKET_H
