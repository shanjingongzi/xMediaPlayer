#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H
#include<QAudio>
#include<QAudioOutput>
#include<QAudioDeviceInfo>
class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();
    void Initalize();
    void Run(const char *,const int size);
    void Close();
private:
    QAudioOutput      *output=NULL;
    QIODevice         *io=NULL;
    QAudioFormat      aformat;
    QAudioDeviceInfo  ainfo;
    bool              audioDeviceOk;
    int               sampleRate=48000;
    int               sampleSize=16;
    int               channel=2;
};

#endif // AUDIOPLAYER_H
