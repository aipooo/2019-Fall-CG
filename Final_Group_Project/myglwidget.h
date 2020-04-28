#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QTime>
#include "camera.h"
#include "maze.h"
#include "textures.h"
#include "player.h"
#include <iostream>
#include <algorithm>
class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();
    void paintMaze();
    void paintSkyBox();
    void paintMark();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
private:
    QTimer* timer;  //计时器
    Player* player; //玩家
    Textures skyBox;    //天空盒纹理
    std::vector<vec3> snow; //标记终点的纹理
    float lastX, lastY;     //鼠标位置
    bool firstMouse;
};
#endif // MYGLWIDGET_H
