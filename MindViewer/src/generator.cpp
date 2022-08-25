#include "generator.h"

Generator::Generator()
{
    isStop=false;

    timer = new QTimer();
    timer->setInterval(2);
    connect(timer,&QTimer::timeout,[=](){
        buff.clear();
        if(i%511==0){
            buff = getPkg(true);
        }else{
            buff = getPkg(false);
        }

        emit sendData(buff);

        if(i > 3000000){
            i=0;
        }
        ++i;
    });
    timer->start();
}

Generator::~Generator()
{
    timer->stop();
    delete timer;
}

bool Generator::getBool()
{
    return getNum(2);
}

int Generator::getNum(int max)
{
    return QRandomGenerator::global()->bounded(0,max);
}

QByteArray Generator::getOne(uchar mn,int max)
{
    QByteArray data;
    data.append((uchar)mn);
    data.append(getNum(max));
    return data;
}

QByteArray Generator::getRaw()
{
    QByteArray data;
    data.append(0x80);//
    data.append(0x02);
    data.append(getNum());//low
    data.append(getNum());//high
    return data;
}

QByteArray Generator::getEEG()
{
    QByteArray data;
    data.append(0x83); //eeg
    data.append(0x18); //eeg count value
    //delta
    data.append(getNum());
    data.append(getNum());
    data.append(getNum());
    //theta
    data.append(getNum());
    data.append(getNum());
    data.append(getNum());
    //lowalpha
    data.append(getNum());
    data.append(getNum());
    data.append(getNum());
    //highalpha
    data.append(getNum());
    data.append(getNum());
    data.append(getNum());
    //lowbeta
    data.append(getNum());
    data.append(getNum());
    data.append(getNum());
    //highbeta
    data.append(getNum());
    data.append(getNum());
    data.append(getNum());
    //lowgamma
    data.append(getNum());
    data.append(getNum());
    data.append(getNum());
    //middlegamma
    data.append(getNum());
    data.append(getNum());
    data.append(getNum());

    return data;
}

//组包
//各个模块对应的部分不一定出现，顺序也不一定
//处理流程为
//随机选择一个模块，判断这个模块是否已经有了
//如果还没有这个模块，获取随机book决定是否添加
QByteArray Generator::getPkg(bool status)
{
    QByteArray pkg;

    while(true){
            ms.clear();
        pkg.clear();
        pkg.append(0xAA);//0
        pkg.append(0xAA);//1
        pkg.append((int)0x00);// payload，先为空，等模块确定后才能确定

        //使用两个循环，外层决定是否使用，内层决定使用哪一个模块
        for(int i=0;i<6;i++){//是否使用
            bool isUse = getBool();
            if(!isUse){
                continue;
            }
            for(int j=0;j<6;j++){
                int module = getNum(6);
                qDebug()<<module;
                if(!ms.contains(module)){
                    ms.append(module);
                    switch(TGAModules[module]){
                    case 0x80:
                        pkg.append(getRaw());
                        break;
                    case 0x83:
                        if(status){
                            pkg.append(getEEG());
                        }
                        break;
                    case 0x01:
                        pkg.append(getOne(0x01,128));
                        break;
                    case 0x02:
                        pkg.append(getOne(0x02,256));
                        break;
                    case 0x04:
                        pkg.append(getOne(0x04,100));
                        break;
                    case 0x05:
                        pkg.append(getOne(0x05,100));
                        break;
                    }
                }
            }
        }
        //update plength
        pkg[2]=pkg.size()-3;
        if((int)pkg[2]==0){//如果模块未被使用，就重新生成
            continue;
        }
        if(status && !ms.contains(5)){//如果应该有eeg数据但是没有就重新生成
            continue;
        }
        //calculate sumcheck
        int checksum = pkg[3];
        for (int i = 4; i < pkg.size(); i++)
        {
            checksum += pkg[i];
        }
        checksum &= 0xff;
        checksum = ~checksum & 0xff;
        pkg.append(checksum);
        break;
    }

    //添加随机干扰数据
    int cnt = getNum();
    for(int n=0;n<cnt;n++){
        pkg.append(getNum());
    }

    return pkg;
}
