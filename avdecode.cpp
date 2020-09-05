#include "avdecode.h"
using namespace std;
using namespace cv;

//static var 定义
uchar avdecode::B_Table[256][256];
uchar avdecode::G_Table[256][256];
uchar avdecode::R_Table[256][256];
uchar avdecode::G_Temp_Table[256][256];
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
            R_Table[i][j]=ADJUST(i+1.4075*(j-128));
            B_Table[i][j]=ADJUST(i+1.7790*(j-128));
            G_Temp_Table[i][j]=0.3455*(i-128)+0.7169*(j-128);
            G_Table[i][j]=ADJUST(i-j);
        }
    }
}
void avdecode::InitializeAudio()
{
    aformat.setCodec("audio/pcm");
    aformat.setSampleRate(sampleRate);
    aformat.setSampleSize(sampleSize);
    aformat.setChannelCount(channel);
    aformat.setSampleType(QAudioFormat::SignedInt);
    aformat.setByteOrder(QAudioFormat::LittleEndian);
    ainfo=QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());
    audioDeviceOk=ainfo.isFormatSupported(aformat);
    output=new QAudioOutput(aformat);
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
void avdecode::YuvToMat(uchar *y,uchar *u,uchar *v,Mat *dst,int width,int height)
{
    int uwidth=width>>1;
    int rgbWidth=width*3;
    int offset=0;
    uchar Y,U,V;
    int yIdx,uIdx,vIdx;
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            yIdx=i*width+j;
            uIdx=(i>>1)*uwidth+(j>>1);
            Y=y[yIdx];
            U=u[uIdx];
            V=v[uIdx];
            dst->data[offset++]=B_Table[Y][U];
            dst->data[offset++]=G_Table[Y][G_Temp_Table[U][V]];
            dst->data[offset++]=R_Table[Y][V];
        }
    }
}
int avdecode::GetWidth()
{
    return pCodecCtx->width;
}
int avdecode::GetHeight()
{
    return pCodecCtx->height;
}
void avdecode::ConvertAudio(const AVFrame *const aframe,char *out)
{
    SwrContext *aCtx = NULL;
    if(aCtx==NULL)
    {
        aCtx=swr_alloc();
        swr_alloc_set_opts(aCtx,
                           pACodecCtx->channel_layout,
                           AV_SAMPLE_FMT_S16,
                           pACodecCtx->sample_rate,
                           pACodecCtx->channels,
                           pACodecCtx->sample_fmt,
                           pACodecCtx->sample_rate,
                           0,0
                           );

         swr_init(aCtx);
    }
    uint8_t *data[1];
    data[0]=(uint8_t *)out;
    int len=swr_convert(aCtx,data,1000,
                        (const uint8_t **)aframe->data,
                        aframe->nb_samples);
    if(len<=0)
    {
        return;
    }
    int outsize=av_samples_get_buffer_size(NULL,pACodecCtx->channels,
                                           aframe->nb_samples,
                                           AV_SAMPLE_FMT_S16,
                                           0);
}

