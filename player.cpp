#include "player.h"
using namespace std;
using namespace cv;
bool Player::IsReady;
bool Player::IsStop;
bool Player::IsPause;
Player::Player()
{
    decode=new avdecode();
    audioPlayer=new AudioPlayer();
    videoPlayer=nullptr;
    IsReady=false;
    IsPause=true;
    IsStop=true;
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
    if(IsReady!=true)
    {
        return;
    }
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
    unique_lock<mutex>look(mtx);
    IsPause=false;
    IsStop=true;
}
bool Player::OpenFile(const string &filename)
{
    if(decode->OpenVideo(filename))
    {
        IsReady=true;
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
        AVPacket *packet=av_packet_alloc();
        AVFrame  *frame=av_frame_alloc();
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
                continue;
            }
            else
            {
                this->mtx.lock();
                if(this->videoFrame.size()<10&&this->audioFrame.size()<10)
                {
                    this->decode->ReadPacket(packet);
                    type=this->decode->ReadFrame(frame,packet);
                    if(type==VIDEO_FRAME)
                    {
                        Mat image(height,width,CV_8UC3);
                        this->decode->YuvToMat(frame->data[0],frame->data[1],frame->data[2],&image,width,height);
                        this->videoFrame.push(image);
                    }
                    else if(type==AUDIO_FRAME)
                    {
                        char *data=new char[10000];
                        this->decode->ConvertAudio(frame,data,&size);
                        this->audioFrame.push(AudioFrame(data,size));
                    }
                }
                this->mtx.unlock();
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
        while(IsReady)
        {
            Mat image;
            mtx.lock();
            if(this->videoFrame.size()>0)
            {
                 image=this->videoFrame.front();
                 this->videoFrame.pop();
            }
            mtx.unlock();
            if(!image.empty())
            this->videoPlayer->Draw(image);
            this_thread::sleep_for(chrono::milliseconds(23));
        }
    });
    t.detach();
}
void Player::StartAudioThread()
{
    thread t([this]()
    {
        AudioFrame audioframe;
        this->mtx.lock();
        while(this->IsReady)
        {
            if(this->audioFrame.size()>0)
            {
                audioframe=this->audioFrame.front();
                this->audioFrame.pop();
            }
            this->mtx.unlock();
            if(audioframe.data[0]!='\0')
            {
                this->audioPlayer->Run(audioframe.data,audioframe.size);
                //delete[] audioframe.data;
            }
            //audioframe.data=nullptr;
        }
    });
    t.detach();
}


























