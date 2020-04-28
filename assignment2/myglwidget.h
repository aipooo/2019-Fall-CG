#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void DrawBody();
    void DrawShoulder();
    void DrawHip();
    void DrawArm();
    void DrawLeg();
    void DrawHead();
    void DrawNeck();

private:
    QTimer *timer;
    float angleofllarm, angleofluarm, angleofrlarm, angleofruarm, angleofluleg, angleofruleg,angleofllleg, angleofrlleg;
    float llarm, rlarm, llleg, rlleg, uarm, uleg, angle, c;
    float angle1, angle2;
};
#endif // MYGLWIDGET_H
