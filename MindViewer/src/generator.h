/////////////////////////////////////////////
///
/// \author jacky lea
/// \date 2020-12-26
/// \note 用于产生测试数据
///
////////////////////////////////////////////

#ifndef GENERATOR_H
#define GENERATOR_H

#include <QThread>
#include <QByteArray>
#include <QDebug>
#include <QtMath>
#include <QRandomGenerator>
#include <QTimer>
#include <QVector>
#include <QMutex>

const QList<uchar> TGAModules={0x01,0x02,0x04,0x05,0x80,0x83};

class Generator : public QThread
{
    Q_OBJECT
public:
    Generator();
    ~Generator();

    bool getBool();//return true/false
    int getNum(int max=256);//[0,max)

    QByteArray getOne(uchar mn, int max);//输入为modulename,max
    QByteArray getRaw();//
    QByteArray getEEG();
    QByteArray getPkg(bool status);

signals:
    void sendData(QByteArray ba);

private:
    QTimer *timer;
    QMutex mutex;
    bool isStop;

    int i=0;
    QByteArray ms;//记录已经有的模块
    QByteArray buff;
};

#endif // GENERATOR_H
