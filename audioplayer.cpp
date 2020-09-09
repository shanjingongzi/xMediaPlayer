#include "audioplayer.h"

AudioPlayer::AudioPlayer()
{
    output=nullptr;
}
AudioPlayer::~AudioPlayer()
{
    delete output;
}
void AudioPlayer::Initalize()
{

    aformat.setCodec("audio/pcm");
    aformat.setSampleRate(sampleRate);
    aformat.setSampleSize(sampleSize);
    aformat.setChannelCount(channel);
    aformat.setSampleType(QAudioFormat::SignedInt);
    aformat.setByteOrder(QAudioFormat::LittleEndian);
    aformat.setSampleType(QAudioFormat::SignedInt);
    ainfo=QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());
    audioDeviceOk=ainfo.isFormatSupported(aformat);
    output=new QAudioOutput(aformat);
    io=output->start();
}
void AudioPlayer::Run(const char *data,const int size)
{
    io->write(data,size);
}
void AudioPlayer::Close()
{
    io->close();
    output->stop();
}
