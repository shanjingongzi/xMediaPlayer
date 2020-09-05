#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;
using namespace cv;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    decoder=new avdecode();
}

MainWindow::~MainWindow()
{
    delete decoder;
    delete ui;
}


void MainWindow::on_actionopen_triggered()
{
    decoder->Initialized();
    decoder->OpenVideo("/home/shanjingongzi/视频/video.mp4");
    AVFrame *frame=av_frame_alloc();
    cv::Mat frameRbg=cv::Mat(cv::Size(decoder->GetWidth(),decoder->GetWidth()),CV_8UC3);
    AVPacket *packet;
    av_malloc(sizeof(AVPacket));
    while(true)
    {
        bool ret=decoder->ReadPacket(packet);
        if(!ret)
        {
            continue;
        }
        FrameType type=decoder->ReadFrame(frame,packet);
        if(type==VIDEO_FRAME)
        {
            decoder->YuvToMat(frame->data[0],frame->data[1],frame->data[2],&frameRbg,decoder->GetWidth(),decoder->GetHeight());
            cv::imshow("video",frameRbg);
            cv::waitKey(23);
        }
    }
}
