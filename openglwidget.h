#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include<QOpenGLWidget>
#include<QOpenGLFunctions>

class OpenGLWidget:public QOpenGLWidget,public QOpenGLFunctions
{
public:
    OpenGLWidget(QWidget *parent);

};

#endif // OPENGLWIDGET_H
