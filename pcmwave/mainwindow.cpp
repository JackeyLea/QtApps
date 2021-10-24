#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(800,600);
    length = 0;

    vdata.resize(102400000);

    image = QImage(3000,600,QImage::Format_RGB888);
    image.fill(Qt::white);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawWave()
{
    QPainter painter(&image);
    painter.setPen(QPen(Qt::green));
    int index = 0;
    int length = this->length;
    if (length>0)
    {
        int winterval = ceil(length/this->width()+0.5);
        int halfHeight = this->height()>>1;
        int hinterval = 32767/halfHeight;
        int indexWave = 0;
        int yValue = 0;
        int yValue2 = 0;
        int stepIndex = 0;
        for (index = 0; index<length ; index+=winterval){
            int value = 0;
            for (stepIndex = 1; stepIndex < winterval; stepIndex++){
                value++;
                yValue = vdata[indexWave*winterval+stepIndex];
                yValue2 = vdata[indexWave*winterval+stepIndex+1];
                yValue = yValue/hinterval;
                yValue2 = yValue2/hinterval;
                if (yValue<0){
                    yValue = halfHeight+abs(yValue);
                }else{
                    yValue = halfHeight-yValue;
                }

                if (yValue2<0){
                    yValue2 = halfHeight+abs(yValue2);
                }else{
                    yValue2 = halfHeight-yValue2;
                }
                painter.drawLine(indexWave,yValue,indexWave,yValue2);
            }
            indexWave++;
        }
    }
    image.save("1","png");
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0,0,image);
    update();
}

void MainWindow::on_btnPlay_clicked()
{
    AVFormatContext	*pFormatCtx=avformat_alloc_context();
    size_t				i;
    int audioStream=-1;
    AVCodecContext	*pCodecCtx=NULL;
    AVCodec			*pCodec=NULL;
    AVCodecParameters *pCodecPara=NULL;
    AVPacket *packet=av_packet_alloc(); // 分配AVPacket结构体
    AVFrame	*pFrame=av_frame_alloc();
    char filename[]="test.mp3";

    //支持网络流
    avformat_network_init();
    //Open
    if(avformat_open_input(&pFormatCtx,filename,NULL,NULL)!=0){
        printf("Couldn't open file.\n");
        return;
    }

    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        printf("Couldn't find stream information.\n");
        return;
    }
    // Dump valid information onto standard error
    av_dump_format(pFormatCtx, 0, filename, false);

    // Find the first audio stream
    for(i=0; i < pFormatCtx->nb_streams; i++){
        if(pFormatCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_AUDIO){
            audioStream=i;
            break;
        }
    }

    if(audioStream==-1){
        printf("Didn't find a audio stream.\n");
        return;
    }

    // Find the decoder for the audio stream
    pCodecPara = pFormatCtx->streams[audioStream]->codecpar;
    pCodec=avcodec_find_decoder(pCodecPara->codec_id);
    if(pCodec==NULL){
        printf("Codec not found.\n");
        return;
    }

    //创建解码器
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if(avcodec_parameters_to_context(pCodecCtx,pCodecPara)<0){
        printf("Cannot alloc context.");
        return;
    }

    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
        printf("Could not open codec.\n");
        return;
    }

    printf("Bitrate: %3ld\n", pFormatCtx->bit_rate);
    printf("Codec Name: %s\n", pCodecCtx->codec->long_name);
    printf("Channels:  %d \n", pCodecCtx->channels);
    printf("Sample per Second  %d \n", pCodecCtx->sample_rate);
    pCodecCtx->channels = 1;
    while(av_read_frame(pFormatCtx, packet)>=0){
        if(packet->stream_index==audioStream){
            if(avcodec_send_packet(pCodecCtx,packet)>=0){
                while(avcodec_receive_frame(pCodecCtx,pFrame)>=0){
                    uint8_t *ptr =pFrame->data[0];
                    short val;
                    for (int i = 0; i < pFrame->linesize[0]; i += 1024)
                    {
                        val = (short)(
                                    ((unsigned char)ptr[i]) << 8 |
                                    ((unsigned char)ptr[i+1])
                                );
                        vdata[length] = val;
                        length++;
                    }
                }
            }
        }
        av_packet_unref(packet);
    }
    // Close the codec
    avcodec_close(pCodecCtx);
    // Close the video file
    avformat_close_input(&pFormatCtx);
    //avcodec_parameters_free(&pCodecPara);
    av_packet_free(&packet);
    av_frame_free(&pFrame);
    drawWave();
    return ;
}

