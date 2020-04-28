#include "myglwidget.h"

/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
    timer = new QTimer(this); // 实例化一个定时器
    timer->start(25); // 时间间隔设置为16ms，可以根据需要调整
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    setWindowTitle("TuWei Maze");
    this->resize(QSize(1200, 800));
    setMouseTracking(true);
    player = new Player;
    firstMouse = true;
}


/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
    delete timer;
    delete player;
}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());  
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    float baseX, baseZ;
    baseX = (player->maze->yEnd * 2.0f - 0.5f) * player->maze->wallsize;
    baseZ = (player->maze->xEnd * 2.0f - 0.5f) * player->maze->wallsize;
    for (int i = 0; i < 20; ++i)
    {
        snow.push_back(vec3(baseX+qrand()%int(player->maze->wallsize), 0.0f, baseZ+qrand()%int(player->maze->wallsize)));
    }
    //
    player->maze->textures.LoadTexture2D("E:/LearnCG/FinalProj/wall1.bmp");
    player->maze->textures.LoadTexture2D("E:/LearnCG/FinalProj/wall2.bmp");
    player->maze->textures.LoadTexture2D("E:/LearnCG/FinalProj/wall3.bmp");
    glDisable(GL_TEXTURE_2D);
    //skybox
    glEnable(GL_TEXTURE_CUBE_MAP);
    char skyboxFile[6][50] = {
      "E:/LearnCG/FinalProj/blue_rt.bmp",
      "E:/LearnCG/FinalProj/blue_lf.bmp",
      "E:/LearnCG/FinalProj/blue_up.bmp",
      "E:/LearnCG/FinalProj/blue_dn.bmp",
      "E:/LearnCG/FinalProj/blue_ft.bmp",
      "E:/LearnCG/FinalProj/blue_bk.bmp"
    };
    skyBox.LoadTextureCube(skyboxFile);
    glDisable(GL_TEXTURE_CUBE_MAP);
    //light
    GLfloat materialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat materialDiffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat materialSpecular[] = {0.8f, 0.8f, 0.8f, 0.0f};
    GLfloat shininess[] = {64.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);


    GLfloat lightSpecular[] = {0.7f, 0.7f, 0.8f, 1.0f};
    GLfloat lightDiffuse[] = {0.6f, 0.6f, 0.9f, 1.0f};
    GLfloat lightAmbient[] = {0.2f, 0.2f, 0.4f, 1.0f};
    GLfloat lightConstAttenuation[] = {1.0f};
    GLfloat lightLinearAttenuation[] = {0.0f};
    GLfloat lightQuadAttenuation[] = {0.00f};
    GLfloat lightPosition[] = {1200.0f, 300.0f, 1200.0f, 1.0f};
    GLfloat LightModelAmbient[] = { 0.6f, 0.6f, 0.6f, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, lightConstAttenuation);
    glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, lightLinearAttenuation);
    glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lightQuadAttenuation);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


    //Material

    glEnable(GL_DEPTH_TEST);

}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, width() / height(), 0.1, 60000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    vec3 target = player->camera->position + player->camera->front;
    gluLookAt(player->camera->position.x, player->camera->position.y, player->camera->position.z,
              target.x, target.y, target.z,
              player->camera->up.x, player->camera->up.y, player->camera->up.z);

    paintSkyBox();
    paintMaze();
    paintMark();
    if (player->checkWin())
    {
        glDisable(GL_DEPTH_TEST);
        QPainter painter;
        painter.begin(this);
        QPen pen;
        pen.setColor(Qt::red);
        QFont font;
        font.setPointSize(60);
        painter.setFont(font);
        painter.setPen(pen);
        painter.drawText(width()/2-120, height()/2, "Finish");
        painter.end();
        glEnable(GL_DEPTH_TEST);
        setMouseTracking(false);
    }

}


/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}


/*###################################################
##  函数: keyPressEvent
##  函数描述： 键盘控制玩家移动
##  参数描述： 无
#####################################################*/
void MyGLWidget::keyPressEvent(QKeyEvent *e)
{
    //Key Control
    if (e->key() == Qt::Key_W)
        player->FBwardMove(forward);
    if (e->key() == Qt::Key_S)
        player->FBwardMove(backward);
    if (e->key() == Qt::Key_A)
        player->HorizontalMove(toleft);
    if (e->key() == Qt::Key_D)
        player->HorizontalMove(toright);
    if (e->key() == Qt::Key_I)
        player->camera->ProcessMouseMovement(0.0f, 5.0f);
    if (e->key() == Qt::Key_K)
        player->camera->ProcessMouseMovement(0.0f, -5.0f);
    if (e->key() == Qt::Key_J)
        player->camera->ProcessMouseMovement(-5.0f, 0.0f);
    if (e->key() == Qt::Key_L)
        player->camera->ProcessMouseMovement(5.0f, 0.0f);
    update();
}
/*###################################################
##  函数: mouseMoveEvent
##  函数描述： 鼠标控制玩家视角移动
##  参数描述： 无
#####################################################*/
void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    float xpos = e->x();
    float ypos = e->y();
    if (firstMouse)
    {
        //捕获鼠标位置
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    //获取鼠标位移
    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    //更新鼠标位置
    lastX = xpos;
    lastY = ypos;
    //调用视角控制函数
    player->camera->ProcessMouseMovement(xOffset, yOffset);
    update();
}
/*###################################################
##  函数: paintMaze
##  函数描述： 绘制3D迷宫
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintMaze()
{
    //开启纹理
    glEnable(GL_TEXTURE_2D);
    float baseZ, baseX;
    float wallsize = player->maze->wallsize;
    float dwallsize = wallsize * 2.0f;
    //绘制
    for (int i = 0; i < player->maze->Height; i++)
    {
        for (int j = 0; j < player->maze->Width; j++)
        {
            //绘制墙
            if (player->maze->maze[i][j] == 1)
            {
                //选取纹理
                glBindTexture(GL_TEXTURE_2D, player->maze->textures.textures2D[j % 3]);
                baseZ = i * dwallsize;
                baseX = j * dwallsize;
                glBegin(GL_QUADS);
                //Front
                glTexCoord2f(0.0f, 0.0f);
                glNormal3f(0.0f, 0.0f, 1.0f);
                glVertex3f(baseX-wallsize, 0.0f, baseZ+wallsize);
                glTexCoord2f(1.0f, 0.0f);
                glNormal3f(0.0f, 0.0f, 1.0f);
                glVertex3f(baseX+wallsize, 0.0f, baseZ+wallsize);
                glTexCoord2f(1.0f, 1.0f);
                glNormal3f(0.0f, 0.0f, 1.0f);
                glVertex3f(baseX+wallsize, dwallsize, baseZ+wallsize);
                glTexCoord2f(0.0f, 1.0f);
                glNormal3f(0.0f, 0.0f, 1.0f);
                glVertex3f(baseX-wallsize, dwallsize, baseZ+wallsize);
                //top

                glTexCoord2f(0.0f, 0.0f);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(baseX-wallsize, dwallsize, baseZ+wallsize);
                glTexCoord2f(1.0f, 0.0f);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(baseX+wallsize, dwallsize, baseZ+wallsize);
                glTexCoord2f(1.0f, 1.0f);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(baseX+wallsize, dwallsize, baseZ-wallsize);
                glTexCoord2f(0.0f, 1.0f);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(baseX-wallsize, dwallsize, baseZ-wallsize);
                //back
                glTexCoord2f(0.0f, 0.0f);
                glNormal3f(0.0f, 0.0f, -1.0f);
                glVertex3f(baseX-wallsize, 0.0f, baseZ-wallsize);
                glTexCoord2f(1.0f, 0.0f);
                glNormal3f(0.0f, 0.0f, -1.0f);
                glVertex3f(baseX+wallsize, 0.0f, baseZ-wallsize);
                glTexCoord2f(1.0f, 1.0f);
                glNormal3f(0.0f, 0.0f, -1.0f);
                glVertex3f(baseX+wallsize, dwallsize, baseZ-wallsize);
                glTexCoord2f(0.0f, 1.0f);
                glNormal3f(0.0f, 0.0f, -1.0f);
                glVertex3f(baseX-wallsize, dwallsize, baseZ-wallsize);
                //left

                glTexCoord2f(0.0f, 0.0f);
                glNormal3f(-1.0f, 0.0f, 0.0f);
                glVertex3f(baseX-wallsize, 0.0f, baseZ-wallsize);
                glTexCoord2f(1.0f, 0.0f);
                glNormal3f(-1.0f, 0.0f, 0.0f);
                glVertex3f(baseX-wallsize, 0.0f, baseZ+wallsize);
                glTexCoord2f(1.0f, 1.0f);
                glNormal3f(-1.0f, 0.0f, 0.0f);
                glVertex3f(baseX-wallsize, dwallsize, baseZ+wallsize);
                glTexCoord2f(0.0f, 1.0f);
                glNormal3f(-1.0f, 0.0f, 0.0f);
                glVertex3f(baseX-wallsize, dwallsize, baseZ-wallsize);
                //right

                glTexCoord2f(0.0f, 0.0f);
                glNormal3f(1.0f, 0.0f, 0.0f);
                glVertex3f(baseX+wallsize, 0.0f, baseZ+wallsize);
                glTexCoord2f(1.0f, 0.0f);
                glNormal3f(1.0f, 0.0f, 0.0f);
                glVertex3f(baseX+wallsize, 0.0f, baseZ-wallsize);
                glTexCoord2f(1.0f, 1.0f);
                glNormal3f(1.0f, 0.0f, 0.0f);
                glVertex3f(baseX+wallsize, dwallsize, baseZ-wallsize);
                glTexCoord2f(0.0f, 1.0f);
                glNormal3f(1.0f, 0.0f, 0.0f);
                glVertex3f(baseX+wallsize, dwallsize, baseZ+wallsize);
                glEnd();
            }
            else
            {   //绘制通道
                glBindTexture(GL_TEXTURE_2D, player->maze->textures.textures2D[0]);
                baseZ = i * dwallsize;
                baseX = j * dwallsize;
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(baseX-wallsize, 0.0f, baseZ+wallsize);
                glTexCoord2f(1.0f, 0.0f);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(baseX+wallsize, 0.0f, baseZ+wallsize);
                glTexCoord2f(1.0f, 1.0f);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(baseX+wallsize, 0.0f, baseZ-wallsize);
                glTexCoord2f(0.0f, 1.0f);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(baseX-wallsize, 0.0f, baseZ-wallsize);
                glEnd();
            }
        }
    }
    glDisable(GL_TEXTURE_2D);
}
/*###################################################
##  函数: paintSkyBox
##  函数描述： 绘制天空盒
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintSkyBox()
{
    //开启立方体贴图
    glEnable(GL_TEXTURE_CUBE_MAP);
    float baseZ, baseX;
    float wallsize = player->maze->wallsize;
    float fwallsize = wallsize * float(player->maze->Width) * 1.5f;
    baseX = float(player->maze->Width) * wallsize;
    baseZ = float(player->maze->Height) * wallsize;
    //绑定纹理
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox.texturesCube[0]);
    glBegin(GL_QUADS);
    //right
    glTexCoord3f(1.0f, 1.0f, -1.0f);
    glVertex3f(baseX+fwallsize, -fwallsize, baseZ+fwallsize);
    glTexCoord3f(1.0f, 1.0f, 1.0f);
    glVertex3f(baseX+fwallsize, -fwallsize, baseZ-fwallsize);
    glTexCoord3f(1.0f, -1.0f, 1.0f);
    glVertex3f(baseX+fwallsize, fwallsize, baseZ-fwallsize);
    glTexCoord3f(1.0f, -1.0f, -1.0f);
    glVertex3f(baseX+fwallsize, fwallsize, baseZ+fwallsize);
    //left
    glTexCoord3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(baseX-fwallsize, -fwallsize, baseZ-fwallsize);
    glTexCoord3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(baseX-fwallsize, -fwallsize, baseZ+fwallsize);
    glTexCoord3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(baseX-fwallsize, fwallsize, baseZ+fwallsize);
    glTexCoord3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(baseX-fwallsize, fwallsize, baseZ-fwallsize);
    //top
    glTexCoord3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(baseX-fwallsize, fwallsize, baseZ+fwallsize);
    glTexCoord3f(1.0f, 1.0f, 1.0f);
    glVertex3f(baseX+fwallsize, fwallsize, baseZ+fwallsize);
    glTexCoord3f(1.0f, 1.0f, -1.0f);
    glVertex3f(baseX+fwallsize, fwallsize, baseZ-fwallsize);
    glTexCoord3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(baseX-fwallsize, fwallsize, baseZ-fwallsize);
    //down
    glTexCoord3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(baseX-fwallsize, -fwallsize, baseZ-fwallsize);
    glTexCoord3f(1.0f, -1.0f, -1.0f);
    glVertex3f(baseX+fwallsize, -fwallsize, baseZ-fwallsize);
    glTexCoord3f(1.0f, -1.0f, 1.0f);
    glVertex3f(baseX+fwallsize, -fwallsize, baseZ+fwallsize);
    glTexCoord3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(baseX-fwallsize, -fwallsize, baseZ+fwallsize);
    //front
    glTexCoord3f(1.0f, 1.0f, 1.0f);
    glVertex3f(baseX-fwallsize, -fwallsize, baseZ+fwallsize);
    glTexCoord3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(baseX+fwallsize, -fwallsize, baseZ+fwallsize);
    glTexCoord3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(baseX+fwallsize, fwallsize, baseZ+fwallsize);
    glTexCoord3f(1.0f, -1.0f, 1.0f);
    glVertex3f(baseX-fwallsize, fwallsize, baseZ+fwallsize);
    //back
    glTexCoord3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(baseX+fwallsize, -fwallsize, baseZ-fwallsize);
    glTexCoord3f(1.0f, 1.0f, -1.0f);
    glVertex3f(baseX-fwallsize, -fwallsize, baseZ-fwallsize);
    glTexCoord3f(1.0f, -1.0f, -1.0f);
    glVertex3f(baseX-fwallsize, fwallsize, baseZ-fwallsize);
    glTexCoord3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(baseX+fwallsize, fwallsize, baseZ-fwallsize);
    glEnd();
    glDisable(GL_TEXTURE_CUBE_MAP);
}
/*###################################################
##  函数: paintMark
##  函数描述： 绘制重点标志
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintMark()
{
    //更改点的大小
    glPointSize(15.0f);
    glBegin(GL_POINTS);
    for (unsigned int i = 0; i < snow.size(); ++i)
    {
        //标记随即升高
        snow[i].y += float(qrand() % 100);
        //达到高度返回
        if (snow[i].y > 2000.0f)
        {
            snow[i].y = 0.0f;
        }
        else
        {
            //绘制
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(snow[i].x, snow[i].y, snow[i].z);
        }
    }
    glEnd();
}
