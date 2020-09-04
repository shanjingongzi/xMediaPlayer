#ifndef AVDECODE_H
#define AVDECODE_H
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


class avdecode
{
public:
    avdecode();
    bool OpenVideo(const std::string&filename);
    void ReadPacket(AVPacket &packet);
    void Initialized();
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
