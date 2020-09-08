#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include"opencv2/opencv.hpp"
#include<QOpenGLWidget>
#include<QOpenGLFunctions>


class OpenGLWidget:public QOpenGLWidget,public QOpenGLFunctions
{
public:
    OpenGLWidget(QWidget *parent);
    ~OpenGLWidget();
public:
    void         Draw(cv::Mat &image);
    void         initializeGL() override;
    void         paintGL() override;
    void         resizeGL(int w, int h) override;
    GLuint       MatToTexTure(const cv::Mat *const image,GLenum minFilter,GLenum magFilter,GLenum swarpFilter);
private:
    cv::Mat image;

};

#endif // OPENGLWIDGET_H
