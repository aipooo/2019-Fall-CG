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
    timer->start(16); // 时间间隔设置为16ms，可以根据需要调整
    connect(timer, SIGNAL(timeout()), this, SLOT(update())); // 连接update()函数，每16ms触发一次update()函数进行重新绘图
}


/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
	delete this->timer;
}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // 着色器程序文件名
    const char* vertfile = "C:\\Users\\Administrator\\Desktop\\CG\\CGTemplate3\\ball.vert";
    const char* fragfile = "C:\\Users\\Administrator\\Desktop\\CG\\CGTemplate3\\ball.frag";
    //初始化着色器
    initShader(vertfile, fragfile);
    //初始化angle为0
    angle = 0.0f;

    glViewport(0, 0, width(), height());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
    // Your Implementation.
    glClearColor(0.0f, 0.0f,  0.0f, 0.0f);
    drawBall();
    //drawBallVBO();
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
##  函数: initShader
##  函数描述： 完成对两个着色器程序的加载、编译及绑定工作
##  参数描述：
##  vertfilename: 顶点着色器文件名
##  fragfilename: 片段着色器文件名
#####################################################*/
void MyGLWidget::initShader(const char* vertfilename, const char* fragfilename)
{
    //获取着色器程序代码
    char ch;
    string vertcode, fragcode;
    ifstream vertfile, fragfile;
    ostringstream vbuf, fbuf;
    vertfile.open(vertfilename);
    fragfile.open(fragfilename);
    while(vbuf && vertfile.get(ch))
        vbuf.put(ch);
    while(fbuf && fragfile.get(ch))
        fbuf.put(ch);
    vertcode = vbuf.str();
    fragcode = fbuf.str();
    vertfile.close();
    fragfile.close();
    const char* vsource = vertcode.c_str();
    const char* fsource = fragcode.c_str();

    //创建一个顶点着色器对象和一个片段着色器对象
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
    //将源代码链接到每个着色器对象
    glShaderSource(vertexShader, 1, &vsource, nullptr);
    glShaderSource(fragmentShader, 1, &fsource, nullptr);
	
    //编译着色器对象
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
	
    //创建一个program对象
    program = glCreateProgram();
	
    //将编译后的着色器对象链接到程序对象
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
	
    //链接程序对象
    glLinkProgram(program);
	
    //删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    initShaderVariables();
    //initVBO();
}

/*###################################################
##  函数: drawBall
##  函数描述： 更新angle，改变球心的位置，模拟小球的运动
##  参数描述： 无
#####################################################*/
void MyGLWidget::drawBall()
{
    glBegin(GL_POINTS);
        GLfloat x = 2.0f * sin(angle);
        GLfloat y = 0.0f;
        GLfloat z = 2.0f * cos(angle);
        glVertex3f(x, y, z);
        angle += 0.01;
    glEnd();
}

/*###################################################
##  函数: initShaderVariable
##  函数描述： 初始化传递到着色器里的各个变量的值，包括有关矩阵
##           以及相关参数的值
##  参数描述： 无
#####################################################*/
void MyGLWidget::initShaderVariables()
{
    //开启功能
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glUseProgram(this->program);
    float mat[16];

    glUniform1f(glGetUniformLocation(program, "radius"), 100.0f);
    glUniform3f(glGetUniformLocation(program, "cameraPos"), 0.0f, 2.0f, 8.0f);
    glUniform3f(glGetUniformLocation(program, "lightPos"), 0.0f, 4.0f, 5.0f);

    //获取、设置model矩阵、view矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, mat);
    gluLookAt(0.0, 4.0, 8.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, mat);

    //获取、设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glGetFloatv(GL_PROJECTION_MATRIX, mat);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, mat);
}

/*###################################################
##  函数: drawBallVBO
##  函数描述： 采用VBO方式，更新angle，改变球心的位置，模拟小球的运动
##  参数描述： 无
#####################################################*/
void MyGLWidget::drawBallVBO()
{
    //更新位置
    point[0] = 2.0f * sin(angle);
    point[1] = 0.0f;
    point[2] = 2.0f * cos(angle);
    angle += 0.01;

    //绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_POINTS, 0, 1);
}


/*###################################################
##  函数: initVBO
##  函数描述： 初始化VBO，以及传递到着色器里的各个变量的值，
##           包括有关矩阵和相关参数的值
##  参数描述： 无
#####################################################*/
void MyGLWidget::initVBO()
{
    //生成VBO
    glGenBuffers(1, &pointVBO);
    //绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    //绑定VBO数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    //指定读取Buffer里面的数据输入到Location=0的变量
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //开启功能
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glUseProgram(this->program);
    float mat[16];

    glUniform1f(glGetUniformLocation(program, "radius"), 100.0f);
    glUniform3f(glGetUniformLocation(program, "cameraPos"), 0.0f, 2.0f, 8.0f);
    glUniform3f(glGetUniformLocation(program, "lightPos"), 0.0f, 4.0f, 5.0f);

    //获取、设置model矩阵、view矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, mat);
    gluLookAt(0.0, 4.0, 8.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, mat);

    //获取、设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glGetFloatv(GL_PROJECTION_MATRIX, mat);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, mat);
}

