#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget*parent):QOpenGLWidget(parent)
{

}
OpenGLWidget::~OpenGLWidget()
{

}
void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0,0,0,0);
}
void OpenGLWidget::paintGL()
{

}
void OpenGLWidget::resizeGL(int w, int h)
{

}
GLuint OpenGLWidget::MatToTexTure(const cv::Mat *const image,GLenum minFilter,GLenum magFilter,GLenum swarpFilter)
{
     GLuint textrueid;
     glGenTextures(0,&textrueid);
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D,textrueid);
}
