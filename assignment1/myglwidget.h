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

class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
	void keyPressEvent(QKeyEvent *e);

    void mySetIdentityMatrix(GLfloat matrix[4][4]);
    void myMultMatrix(GLfloat m1[4][4], GLfloat m2[4][4]);
    void myTranslate(GLfloat matrix[4][4], GLfloat x, GLfloat y, GLfloat z);
    void myScale(GLfloat matrix[4][4], GLfloat x, GLfloat y, GLfloat z);
    void myRotate(GLfloat matrix[4][4], GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
    void myRotateByQuaternion(GLfloat matrix[4][4], GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
    void coordinateTransformation(GLfloat matrix[4][4], GLfloat &x, GLfloat &y, GLfloat &z);
    void drawLine(int x1, int y1, int x2, int y2);

private:
	int scene_id;
	void scene_0();
	void scene_1();
};
#endif // MYGLWIDGET_H
