#include "mainwindow.h"
#include "ui_mainwindow.h"

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
}
