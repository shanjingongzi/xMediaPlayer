#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<string>
#include<queue>
#include<thread>
#include<QDialog>
#include<QFileDialog>
#include"avdecode.h"
#include"opencv2/opencv.hpp"
#include"player.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resizeEvent(QResizeEvent *event) override;
private slots:

    void on_btnOpenFIle_clicked();

    void on_btnPlay_clicked();
private:
    void closeEvent(QCloseEvent* event) override;
    Ui::MainWindow *ui;
    Player *player;
    void ResetLocation(QWidget*object,int x,int y);
};
#endif // MAINWINDOW_H
