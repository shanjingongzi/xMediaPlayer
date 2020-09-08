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
    char *data;
    AudioFrame(char *_data,int _size):data(_data),size(_size){}
    AudioFrame(){}
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
private:
    avdecode                *decode;
    AudioPlayer             *audioPlayer;
    OpenGLWidget            *videoPlayer;
    static bool             IsReady;
    static bool             IsPause;
    static bool             IsStop;
    std::queue<cv::Mat>     videoFrame;
    std::queue<AudioFrame>      audioFrame;
    std::mutex              mtx;
    std::condition_variable cond;
};

#endif // PLAYER_H
