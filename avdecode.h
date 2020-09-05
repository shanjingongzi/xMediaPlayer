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
}
#include<string>
#include<queue>
#include"opencv2/opencv.hpp"


class avdecode
{
public:
    avdecode();
    bool             OpenVideo(const std::string&filename);
    inline bool      ReadPacket(AVPacket &packet);
    void             Initialized();
    inline FrameType ReadFrame(AVFrame&frame,const AVPacket*packet);
    void             YuvToMat(uchar *y,uchar *u,uchar *v,cv::Mat *dst,int width,int height);

private:
    AVFormatContext *pformatCtx;
    AVCodec *pCodec;
    AVCodecContext *pCodecCtx;
    AVCodec *pACodec;
    AVCodecContext *pACodecCtx;
    int videoIndex;
    int audioIndex;
    static uchar R_Table[255][255];
    static uchar G_Table[255][255];
    static uchar G_Temp_Table[255][255];
    static uchar B_Tbale[255][255];
};

#endif // AVDECODE_H
