#ifndef FFMPEGAUDIO_H
#define FFMPEGAUDIO_H

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libavutil/avutil.h"
};

#include <QThread>
#include <QTime>
#include <QElapsedTimer>
#include <QDebug>
#include <QtGlobal>

class FFmpegAudio : public QThread
{
    Q_OBJECT
public:
    FFmpegAudio();
    ~FFmpegAudio();

signals:
    void updateFrame(QVector<double>,QVector<double>);

protected:
    void run();

private:
    QVector<double> vdataL;
    QVector<double> vdataR;
};

#endif // FFMPEGAUDIO_H
