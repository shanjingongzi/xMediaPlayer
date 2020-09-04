#ifndef AVDECODE_H
#define AVDECODE_H
enum FrameType{VIDEO_FRAME,AUDIO_FRAME,EMPTY_FRAME,ERROR_FRAME};
using uchar=unsigned char;

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
    void             YuvToMat(uchar *y,uchar *u,uchar *v,const cv::Mat *dst);

private:
    AVFormatContext *pformatCtx;
    AVCodec *pCodec;
    AVCodecContext *pCodecCtx;
    AVCodec *pACodec;
    AVCodecContext *pACodecCtx;
    int videoIndex;
    int audioIndex;
};

#endif // AVDECODE_H
