#include "player.h"
using namespace std;
using namespace cv;
bool Player::IsReady;
bool Player::IsStop;
bool Player::IsPause;
bool Player::fileIsOpen;
Player::Player()
{
    decode=new avdecode();
    audioPlayer=new AudioPlayer();
    videoPlayer=nullptr;
    IsReady=false;
    IsPause=true;
    IsStop=true;
    fileIsOpen=false;
}
Player::Player(OpenGLWidget *videoptr)
{
    decode=new avdecode();
    audioPlayer=new AudioPlayer();
    videoPlayer=videoptr;
    IsReady=false;
    IsPause=true;
    IsStop=true;
}
void Player::Initalize()
{
    decode->Initialized();
    audioPlayer->Initalize();
}
void Player::Start()
{
    unique_lock<mutex>look(mtx);
    if(!fileIsOpen)
    {
        return;
    }
    IsReady=true;
    IsPause=false;
    IsStop=false;
    this->StartDecodeThread();
    this->StartVideoThread();
    this->StartAudioThread();
}
void Player::Pause()
{
    unique_lock<mutex>look(mtx);
    if(IsReady!=true)
    {
        return;
    }
    IsPause=true;
}
void Player::Replay()
{
    unique_lock<mutex>look(mtx);
    if(IsReady!=true)
    {
        return;
    }
    IsPause=false;
}
void Player::Stop()
{
    IsPause=false;
    IsStop=true;
    IsReady=false;
    fileIsOpen=false;
    this_thread::sleep_for(chrono::milliseconds(100));
    mtx.lock();
    while(!videoFrame.empty())
    {
        videoFrame.pop();
    }
    while(!audioFrame.empty())
    {
        audioFrame.pop();
    }
    mtx.unlock();
}
bool Player::OpenFile(const string &filename)
{
    if(decode->OpenVideo(filename))
    {
        fileIsOpen=true;
        return true;
    }
    else
    {
        return false;
    }
}
void Player::StartDecodeThread()
{
    thread t([this]()
    {

        FrameType type;
        int width=this->decode->GetWidth();
        int height=this->decode->GetHeight();
        int size=-1;
        while(IsReady)
        {
            if(IsStop){
                break;
            }
            if(IsPause){
                this_thread::sleep_for(chrono::microseconds(50));
                continue;
            }
            else
            {
                this->mtx.lock();
                if(this->videoFrame.size()<10||this->audioFrame.size()<10)
                {
                    this->mtx.unlock();
                    AVPacket *packet=av_packet_alloc();
                    AVFrame  *frame=av_frame_alloc();
                    bool ret=this->decode->ReadPacket(packet);
                    if(!ret)this->IsReady=false;
                    type=this->decode->ReadFrame(frame,packet);
                    if(type==VIDEO_FRAME)
                    {
                        double time=frame->pts;
                        Mat image(height,width,CV_8UC3);
                        this->decode->YuvToMat(frame->data[0],frame->data[1],frame->data[2],&image,width,height);
                        mtx.lock();
                        this->videoFrame.push(VideoFrame(image,time));
                        mtx.unlock();
                    }
                    else if(type==AUDIO_FRAME)
                    {
                        double time=frame->pts;
                        char data[10000];
                        this->decode->ConvertAudio(frame,data,&size);
                        mtx.lock();
                        this->audioFrame.push(AudioFrame(data,size,time));
                        mtx.unlock();
                    }
                    av_packet_free(&packet);
                    av_frame_free(&frame);
                    continue;
                }
                this->mtx.unlock();
                continue;
            }
            this_thread::sleep_for(chrono::milliseconds(5));
        }
    });
    t.detach();
}
void Player::StartVideoThread()
{

    thread t([this]()
    {
        double lastTime=0;
        while(IsReady)
        {
            int timeStep=0;
            mtx.lock();
            if(this->videoFrame.size()>0)
            {
                 VideoFrame videoframe=this->videoFrame.front();
                 this->videoFrame.pop();
                 mtx.unlock();
                 this->videoPlayer->Draw(videoframe.image);
                 timeStep=videoframe.timestep-lastTime;
                 lastTime=videoframe.timestep;
                 double delay=av_q2d(this->decode->pCodecCtx->time_base);
                 this_thread::sleep_for(chrono::milliseconds((int)(timeStep*delay)));
                 continue;
            }
            mtx.unlock();
            this_thread::sleep_for(chrono::milliseconds(20));
        }
    });
    t.detach();
}
void Player::StartAudioThread()
{
    thread t([this]()
    {
        AudioFrame audioframe;
        double lastTime=0;
        while(this->IsReady)
        {
            double sleeptime=0;
            this->mtx.lock();
            if(this->audioFrame.size()>0)
            {
                audioframe=this->audioFrame.front();
                this->audioFrame.pop();
                this->mtx.unlock();
                sleeptime=(audioframe.timestep-lastTime);
                lastTime=audioframe.timestep;
                this->audioPlayer->Run(audioframe.data,audioframe.size);
                double delay=av_q2d(this->decode->pACodecCtx->time_base);
                std::this_thread::sleep_for(chrono::milliseconds((int)(sleeptime*delay)));
                continue;
            }
            this->mtx.unlock();   
            this_thread::sleep_for(chrono::milliseconds(5));
        }
    });
    t.detach();
}


























