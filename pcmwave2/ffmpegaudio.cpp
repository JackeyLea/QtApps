////////////
/// Author: JackeyLea
/// Function: 解码音频数据
///////////

#include "ffmpegaudio.h"

FFmpegAudio::FFmpegAudio()
{

}

FFmpegAudio::~FFmpegAudio()
{
    if(isRunning()){
        requestInterruption();
    }
    quit();
}

void FFmpegAudio::run()
{
    AVFormatContext	*pFormatCtx=avformat_alloc_context();
    size_t				i;
    int audioStream=-1;
    AVCodecContext	*pCodecCtx=NULL;
    AVCodec			*pCodec=NULL;
    AVCodecParameters *pCodecPara=NULL;
    AVPacket *packet=av_packet_alloc(); // 分配AVPacket结构体
    AVFrame	*pFrame=av_frame_alloc();
    char filename[]="/home/jackey/Music/test.mp3";

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
    pCodecCtx->pkt_timebase = pFormatCtx->streams[audioStream]->time_base;

    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
        printf("Could not open codec.\n");
        return;
    }

    printf("Channels:  %d \n", pCodecCtx->channels);
    printf("Sample per Second  %d \n", pCodecCtx->sample_rate);
    pCodecCtx->channels = 1;
    while(av_read_frame(pFormatCtx, packet)>=0){
        if(packet->stream_index==audioStream){
            if(avcodec_send_packet(pCodecCtx,packet)>=0){
                while(avcodec_receive_frame(pCodecCtx,pFrame)>=0){
                    uint8_t *ptr =pFrame->data[0];
                    for (int i = 0; i < pFrame->linesize[0]; i ++)
                    {
                        vdataL.append(ptr[i]);
                        vdataR.append(-ptr[i]);
                    }
                }
                msleep(17);
                emit updateFrame(vdataL,vdataR);
                vdataL.clear();
                vdataR.clear();
            }
        }
        av_packet_unref(packet);
    }
    // Close the codec
    avcodec_close(pCodecCtx);
    // Close the video file
    avformat_close_input(&pFormatCtx);
    av_packet_free(&packet);
    av_frame_free(&pFrame);

    printf("Decode done.\n");
}
