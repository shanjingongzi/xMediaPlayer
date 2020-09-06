#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;
using namespace cv;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionopen_triggered()
{


}

void MainWindow::on_pushButton_clicked()
{
    string filename="/home/shanjingongzi/视频/video.mp4";
    avdecode *decode=new avdecode();
    decode->Initialized();
    decode->InitializeAudio();
    decode->OpenVideo(filename);
    avdecode::isPlay=true;
    std::thread t([filename,decode]()
    {
        AVPacket *packet;
        packet=av_packet_alloc();
        cv::Mat image(Size(decode->GetWidth(),decode->GetHeight()),CV_8UC3);
        AVFrame *frame=av_frame_alloc();
        char *out=new char[100000];
        while(avdecode::isPlay)
        {
            decode->ReadPacket(packet);
            FrameType type=decode->ReadFrame(frame,packet);

            if(type==VIDEO_FRAME)
            {
                decode->YuvToMat(frame->data[0],frame->data[1],frame->data[2],&image,decode->GetWidth(),decode->GetHeight());
                cv::imshow("mp4",image);
                cv::waitKey(23);
            }
            if(type==AUDIO_FRAME)
            {
                decode->ConvertAudio(frame,out);
            }
        }
        delete decode;
        delete [] out;
        av_packet_free(&packet);
        av_frame_free(&frame);
    }
    );
    t.detach();
}

void MainWindow::on_pushButton_2_clicked()
{
    unique_lock<mutex>lock(avdecode::mtx);
    avdecode::isPlay=false;
}
