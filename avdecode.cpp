#include "avdecode.h"
using namespace std;
using namespace cv;
avdecode::avdecode()
{
    pCodec=nullptr;
    pCodecCtx=nullptr;
    pformatCtx=avformat_alloc_context();
    pACodec=nullptr;
    pACodecCtx=nullptr;
    videoIndex=-1;
    audioIndex=-1;
}
void avdecode::Initialized()
{
    av_register_all();
    for(int i=0;i<256;i++)
    {
        for(int j=0;j<256;j++)
        {
            
        }
    }
}
bool avdecode::OpenVideo(const std::string &filename)
{
    int ret=avformat_open_input(&pformatCtx,filename.c_str(),NULL,NULL);
    if(ret!=0)
    {
        return false;
    }
    if(avformat_find_stream_info(pformatCtx,NULL)<0)
    {
        return false;
    }
    for(int i=0;i<pformatCtx->nb_streams;i++)
    {
        if(pformatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
        {
            videoIndex=i;
        }
        else if(pformatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO)
        {
            audioIndex=i;
        }

    }
    if(videoIndex==-1)
    {
        return false;
    }
    if(audioIndex==-1)
    {
        return false;
    }
    pCodecCtx=pformatCtx->streams[videoIndex]->codec;
    pACodecCtx=pformatCtx->streams[audioIndex]->codec;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==nullptr)
    {
        return false;
    }
    pACodec=avcodec_find_decoder(pACodecCtx->codec_id);
    if(pACodec==nullptr)
    {
            return false;
    }
    if(avcodec_open2(pCodecCtx,pCodec,NULL)<0)
    {
        return false;
    }
    if(avcodec_open2(pACodecCtx,pACodec,NULL)<0)
    {
        return false;
    }
    av_dump_format(pformatCtx,0,filename.c_str(),0);
    return true;
}
inline bool avdecode::ReadPacket(AVPacket &packet)
{
    return av_read_frame(pformatCtx,&packet)>=0;
}
inline FrameType avdecode::ReadFrame(AVFrame &frame, const AVPacket *packet)
{
    int got_frame=-1;
    if(packet->stream_index==videoIndex)
    {
        int ret=avcodec_decode_video2(pCodecCtx,&frame,&got_frame,packet);
        if(ret<0)
        {
            return ERROR_FRAME;
        }
        if(got_frame<0)
        {
            return EMPTY_FRAME;
        }
        else
        {
            return VIDEO_FRAME;
        }
    }
    if(packet->stream_index==audioIndex)
    {
        int ret=avcodec_decode_audio4(pACodecCtx,&frame,&got_frame,packet);
        if(ret<0)
        {
            return ERROR_FRAME;
        }
        if(got_frame<0)
        {
            return EMPTY_FRAME;
        }
        else
        {
            return AUDIO_FRAME;
        }

    }
}
void avdecode::YuvToMat(uchar *y,uchar *u,uchar *v,Mat *dst,int width,int height)
{

}