#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;
using namespace cv;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player=new Player(this->ui->openGLWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    this->ui->openGLWidget->setGeometry(0,0,this->width(),this->height());
}

void MainWindow::on_actionopen_triggered()
{


}

void MainWindow::on_pushButton_clicked()
{
    player->Initalize();
    if(player->OpenFile("/home/shanjingongzi/video.mp4"))
    {
        player->Start();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
}
