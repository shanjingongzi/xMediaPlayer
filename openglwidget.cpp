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
    this->image=cv::imread("/home/shanjingongzi/picture/desktop.jpg");
}
void OpenGLWidget::paintGL()
{
    glEnable(GL_TEXTURE_2D);
    GLuint imageTex=MatToTexTure(&image,GL_LINEAR,GL_LINEAR,GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,imageTex);
    glBegin(GL_POLYGON);

    glTexCoord2f(0,0);
    glVertex2f(-1,1);

    glTexCoord2f(1,0);
    glVertex2f(1,1);

    glTexCoord2f(1,1);
    glVertex2f(1,-1);

    glTexCoord2f(0,1);
    glVertex2f(-1,-1);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1,&imageTex);
}
void OpenGLWidget::resizeGL(int w, int h)
{

}
void OpenGLWidget::Draw(cv::Mat &image)
{
    this->image=image;
    update();
}
GLuint OpenGLWidget::MatToTexTure(const cv::Mat *const image,GLenum minFilter,GLenum magFilter,GLenum swarpFilter)
{
     GLuint textrueid;
     glGenTextures(0,&textrueid);
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D,textrueid);
     glPixelStorei(GL_UNPACK_ALIGNMENT,1);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minFilter);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magFilter);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,swarpFilter);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,swarpFilter);
     glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
     GLenum inputColorFormat=GL_BGR;
     if(image->channels()==1)
     {
         inputColorFormat=GL_LUMINANCE;
     }
     glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image->cols,image->rows,0,inputColorFormat
                  ,GL_UNSIGNED_BYTE,image->data);
     return textrueid;
}
