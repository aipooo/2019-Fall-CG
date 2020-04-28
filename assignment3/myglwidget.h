#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

    void initShader(const char* vsource, const char* fsource);
    void initShaderVariables();
    void drawBall();
    void initVBO();
    void drawBallVBO();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QTimer *timer;
    // 你可以将你的shader或者vbo声明为私有变量
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;
    double angle;
    GLuint pointVBO;
    float point[3] = {0.0f, 0.0f, 0.0f};
};
#endif // MYGLWIDGET_H
