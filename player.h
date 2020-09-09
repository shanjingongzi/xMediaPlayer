#ifndef PLAYER_H
#define PLAYER_H

#include<queue>
#include<mutex>
#include<condition_variable>
#include<thread>
#include"openglwidget.h"
#include "avdecode.h"
#include"audioplayer.h"
#include"opencv2/opencv.hpp"
struct AudioFrame
{
    int size;
    char data[10000];
    double timestep;
    AudioFrame(char *_data,int _size,double time):size(_size),timestep(time){memcpy(data,_data,size);}
    AudioFrame(){}
    AudioFrame(const AudioFrame&rhs):size(rhs.size),timestep(rhs.timestep){memcpy(data,rhs.data,size);}

    AudioFrame operator=(const AudioFrame&rhs)
    {
        size=rhs.size;
        timestep=rhs.timestep;
        memcpy(data,rhs.data,size);
    }
};
struct VideoFrame
{
    cv::Mat image;
    double timestep;
    VideoFrame(const cv::Mat _image,double _timestep):image(_image),timestep(_timestep){};
};

class Player
{
public:
    Player();
    Player(OpenGLWidget*videoptr);
public:
    bool                    OpenFile(const std::string&filename);
    void                    StartVideoThread();
    void                    StartAudioThread();
    void                    StartDecodeThread();
    void                    Initalize();
    void                    Pause();
    void                    Start();
    virtual void            Replay();
    void                    Stop();
public:
    static bool             fileIsOpen;
    static bool             IsReady;
    static bool             IsPause;
    static bool             IsStop;
private:
    avdecode                *decode;
    AudioPlayer             *audioPlayer;
    OpenGLWidget            *videoPlayer;
    std::queue<VideoFrame>  videoFrame;
    std::queue<AudioFrame>  audioFrame;
    std::mutex              mtx;
    std::condition_variable cond;
};

#endif // PLAYER_H
