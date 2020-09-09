#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;
using namespace cv;
using namespace this_thread;
using namespace chrono;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player=new Player(this->ui->openGLWidget);
    player->Initalize();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
}
void MainWindow::ResetLocation(QWidget *object,int x,int y)
{
    int width=object->width();
    int height=object->height();
    object->setGeometry(x-width/2,y-height/2,width,height);
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    this->ui->openGLWidget->setGeometry(0,0,this->width(),this->height());
    ResetLocation(ui->btnOpenFIle,this->width()/2,this->height()/2);
    ResetLocation(ui->btnPlay,this->width()/2,this->height()-100);
    ResetLocation(ui->btnPre,this->width()/10*4,this->height()-100);
    ResetLocation(ui->btnNext,this->width()/10*6,this->height()-100);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    player->Stop();
    sleep_for(milliseconds(50));
}
void MainWindow::on_btnOpenFIle_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,"打开视频文件",QDir::currentPath(),"(*.mp4 *.flv *.avi *.rmvb);;(*)");
    this->setWindowTitle(filename);
    this->player->Stop();
    player->OpenFile(filename.toStdString());
}

void MainWindow::on_btnPlay_clicked()
{
    if(!player->IsReady)
    {
        player->Start();
    }
    else if(!player->IsPause)
    {
        player->Pause();
    }
    else
    {
        player->Replay();
    }
}
