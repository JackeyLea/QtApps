#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QtXml>
#include <QTimer>
#include <QTime>
#include <QDateTime>

#include "myudpsocket.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void initModel();

public slots:
    void sltUpdateData();

private slots:
    void on_btnOpen_clicked();

    void on_btnClear_clicked();

    void on_btnSend_clicked();

    void on_btnStop_clicked();

private:
    Ui::Widget *ui;

    MyUdpSocket *myUdp;

    QStandardItemModel *model;

    QList<QPair<QString,int>> typeMap;

    //QMap<QString,int> typeMap;//type map name: type size
    QTimer *timer;

    QUdpSocket *mUdpSocket;

    QByteArray mData;

    QHostAddress host;
    int port;
};

#endif // WIDGET_H
