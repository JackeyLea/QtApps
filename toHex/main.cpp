#include <QCoreApplication>
#include <QDebug>
#include <QByteArray>

QChar toChar(int a){
    QChar b;
    switch(a){
    case 10:
        b='a';
        break;
    case 11:
        b='b';
        break;
    case 12:
        b='c';
        break;
    case 13:
        b='d';
        break;
    case 14:
        b='e';
        break;
    case 15:
        b='f';
        break;
    default:
        b=a+'0';
        break;
    }
    return b;
}

QString getSignedHex(qlonglong n,uint c){
    QString r;

    uint bitCnt = c*8;

    n = qAbs(n);
    qDebug()<<"n is: "<<(int)n;

    QByteArray ba;
    while(n){
        ba.append(n%2);
        n=n/2;
    }
    qDebug()<<ba.size();

    while(ba.size()<bitCnt){
        ba.append('\x00');
        qDebug()<<ba.size();
    }
    ba[bitCnt-1]=1;//1 -

    //swap sequence
    for(int i=0;i<ba.size()/2;++i){
        char t=ba.at(i);
        ba[i]=ba.at(ba.size()-1-i);
        ba[ba.size()-1-i]=t;
    }
    qDebug()<<ba.size()<<ba;

    // 0x00x00x00x0 -> 0x
    for(int j=0;j<ba.size();j=j+4){
        int t = ba[j]*8+ba[j+1]*4+ba[j+2]*2+ba[j+3];
        qDebug()<<t;
        r.append(toChar(t));
    }

    return r;
}

QString getUnsignedHex(qulonglong n,uint c){
    QString r;

    uint bitCnt = c*8;

    n = qAbs(n);
    qDebug()<<"n is: "<<(int)n;

    QByteArray ba;
    while(n){
        ba.append(n%2);
        n=n/2;
    }
    qDebug()<<ba.size();

    while(ba.size()<bitCnt){
        ba.append('\x00');
        qDebug()<<ba.size();
    }

    //swap sequence
    for(int i=0;i<ba.size()/2;++i){
        char t=ba.at(i);
        ba[i]=ba.at(ba.size()-1-i);
        ba[ba.size()-1-i]=t;
    }
    qDebug()<<ba.size()<<ba;

    // 0x00x00x00x0 -> 0x
    for(int j=0;j<ba.size();j=j+4){
        int t = ba[j]*8+ba[j+1]*4+ba[j+2]*2+ba[j+3];
        qDebug()<<t;
        r.append(toChar(t));
    }

    return r;
}

#define getHex(n,c) (n<0?getSignedHex(n,c):getSignedHex(n,c))

int main()
{
    //signed char -128 ~ 127
    QString r = getHex(-127,1);
    qDebug()<<"-127"<<r;

    //unsigned char 0-255
    r = getHex(255,sizeof(uchar));
    qDebug()<<r;

    //signed int -2
    r = getHex(255,sizeof(uchar));
    qDebug()<<r;

    //unsigned int 0-65535
    r = getHex(65535,sizeof(uint));
    qDebug()<<r;

}

