#ifndef AVDECODE_H
#define AVDECODE_H
enum FrameType{VIDEO_FRAME,AUDIO_FRAME,EMPTY_FRAME,ERROR_FRAME};
using uchar=unsigned char;
#ifndef ADJUST(VALUE)
#define ADJUST(VALUE)(VALUE)>255?255:(VALUE)
#elif 
#define ADJUST(VALUE)(VALUE)>255?255:(VALUE)
#endif
extern "C"
{
#include"libavcodec/avcodec.h"
#include"libavfilter/avfilter.h"
#include"libavutil/imgutils.h"
#include"libavutil/imgutils.h"
#include"libavformat/avformat.h"
#include"libswscale/swscale.h"
#include"libswresample/swresample.h"
}
#include<string>
#include<queue>
#include"opencv2/opencv.hpp"
#include<QAudioOutput>
#include<QAudioDeviceInfo>
#include<mutex>
class avdecode
{
public:
    avdecode();
    bool             OpenVideo(const std::string&filename);
    inline bool      ReadPacket(AVPacket *packet);
    void             Initialized();
    inline FrameType ReadFrame(AVFrame*frame,const AVPacket*packet);
    void             YuvToMat(uchar *y,uchar *u,uchar *v,cv::Mat *dst,int width,int height);
    int              GetWidth();
    int              GetHeight();
    void             InitializeAudio();
    void             ConvertAudio(const AVFrame* const frame,char *out,int *size);
public:
    static bool isPlay;
    static std::mutex mtx;

private:
    static uchar     R_Table[256][256];
    static uchar     G_Table[256][256];
    static uchar     G_Temp_Table[256][256];
    static uchar     B_Table[256][256];
private:
    AVFormatContext  *pformatCtx;
    AVCodec          *pCodec;
    AVCodecContext   *pCodecCtx;
    AVCodec          *pACodec;
    AVCodecContext   *pACodecCtx;
    SwrContext       *aCtx;
    int              videoIndex;
    int              audioIndex;
public:
    QAudioOutput      *output=NULL;
    QIODevice         *io=NULL;
    QAudioFormat      aformat;
    QAudioDeviceInfo  ainfo;
    bool              audioDeviceOk;
    int               sampleRate=48000;
    int               sampleSize=16;
    int               channel=2;

};
inline bool avdecode::ReadPacket(AVPacket *packet)
{
    return av_read_frame(pformatCtx,packet)>=0;
}
inline FrameType avdecode::ReadFrame(AVFrame *frame, const AVPacket *packet)
{
    int got_frame=-1;
    if(packet->stream_index==videoIndex){
        if(avcodec_decode_video2(pCodecCtx,frame,&got_frame,packet)<0) return ERROR_FRAME;
        return got_frame>0?VIDEO_FRAME:EMPTY_FRAME;
    }
    else if(packet->stream_index==audioIndex){
        int re=avcodec_send_packet(pACodecCtx,packet);
        if(re!=0)return ERROR_FRAME;
        re=avcodec_receive_frame(pACodecCtx,frame);
        if(re!=0)return ERROR_FRAME;
        else return AUDIO_FRAME;
        return got_frame>0?AUDIO_FRAME:EMPTY_FRAME;
    }
}
#endif // AVDECODE_H
