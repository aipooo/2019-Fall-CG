#include "myglwidget.h"
#include <math.h>

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent),
	scene_id(0)
{
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
	if (scene_id==0) {
		scene_0();
	}
	else {
		scene_1();
	}
}

void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_0) {
		scene_id = 0;
		update();
	}
	else if (e->key() == Qt::Key_1) {
		scene_id = 1;
		update();
	}
}

void MyGLWidget::scene_0()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 100.0f, 0.0f, 100.0f, -1000.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glTranslatef(50.0f, 50.0f, 0.0f);
	
	//draw a triangle
	glPushMatrix();
	glColor3f(0.839f, 0.153f, 0.157f);
    glTranslatef(-20.0f, -10.0f, 0.0f);
    glRotatef(45.0f, 1.0f, 0.0f, 1.0f);
    glTranslatef(-50.0f, -30.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(10.0f, 10.0f);
	glVertex2f(50.0f, 50.0f);
	glVertex2f(80.0f, 10.0f);
	glEnd();
	glPopMatrix();	

	//draw a quad
	glPushMatrix();
	glColor3f(0.122f, 0.467f, 0.706f);
	glTranslatef(20.0f, 20.0f, 0.0f);
	glRotatef(30.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-20.0f, -20.0f);
	glVertex2f(20.0f, -20.0f);
	glVertex2f(20.0f, 20.0f);
	glVertex2f(-20.0f, 20.0f);
	glEnd();
	glPopMatrix();
}

void MyGLWidget::scene_1()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 1000.0f, 0.0f, 1000.0f, -1000.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    //初始化仿射矩阵
    GLfloat compositeMatrix1[4][4], compositeMatrix2[4][4];
    mySetIdentityMatrix(compositeMatrix1);
    mySetIdentityMatrix(compositeMatrix2);
    myTranslate(compositeMatrix1, 500.0f, 500.0f, 0.0f);
    myTranslate(compositeMatrix2, 500.0f, 500.0f, 0.0f);

    //绘制三角形
    //根据scene0进行对应的仿射矩阵的变换
    myTranslate(compositeMatrix1, -200.0f, -100.0f, 0.0f);
    myRotate(compositeMatrix1, 45.0f, 1.0f, 0.0f, 1.0f);
    //myRotateByQuaternion(compositeMatrix1, 45.0f, 1.0f, 0.0f, 1.0f);
    myTranslate(compositeMatrix1, -500.0f, -300.0f, 0.0f);
    //设定未变换时各点的坐标
    GLfloat x1=100.0f, y1=100.0f, z1=0.0f;
    GLfloat x2=500.0f, y2=500.0f, z2=0.0f;
    GLfloat x3=800.0f, y3=100.0f, z3=0.0f;
    //将各点使用仿射矩阵进行变换，得到变换后的新坐标
    coordinateTransformation(compositeMatrix1, x1, y1, z1);
    coordinateTransformation(compositeMatrix1, x2, y2, z2);
    coordinateTransformation(compositeMatrix1, x3, y3, z3);
    //使用Bresenham画线算法绘制直线
    glBegin(GL_POINTS);
        glColor3f(0.839f, 0.153f, 0.157f);
        drawLine(int(x1), int(y1), int(x2), int(y2));
        drawLine(int(x2), int(y2), int(x3), int(y3));
        drawLine(int(x3), int(y3), int(x1), int(y1));
    glEnd();

    //绘制矩形
    //根据scene0进行对应的仿射矩阵的变换
    myTranslate(compositeMatrix2, 200.0f, 200.0f, 0.0f);
    myRotate(compositeMatrix2, 30.0f, 1.0f, 1.0f, 1.0f);
    //myRotateByQuaternion(compositeMatrix2, 30.0f, 1.0f, 1.0f, 1.0f);
    //设定未变换时各点的坐标
    GLfloat x4=-200.0f, y4=-200.0f, z4=0.0f;
    GLfloat x5=200.0f,  y5=-200.0f, z5=0.0f;
    GLfloat x6=200.0f,  y6=200.0f,  z6=0.0f;
    GLfloat x7=-200.0f, y7=200.0f,  z7=0.0f;
    //将各点使用仿射矩阵进行变换，得到变换后的新坐标
    coordinateTransformation(compositeMatrix2, x4, y4, z4);
    coordinateTransformation(compositeMatrix2, x5, y5, z5);
    coordinateTransformation(compositeMatrix2, x6, y6, z6);
    coordinateTransformation(compositeMatrix2, x7, y7, z7);
    //使用Bresenham画线算法绘制直线
    glBegin(GL_POINTS);
        glColor3f(0.122f, 0.467f, 0.706f);
        drawLine(int(x4), int(y4), int(x5), int(y5));
        drawLine(int(x5), int(y5), int(x6), int(y6));
        drawLine(int(x6), int(y6), int(x7), int(y7));
        drawLine(int(x7), int(y7), int(x4), int(y4));
    glEnd();
}


/*################################################################
##	函数：mySetIdentityMatrix
##	函数描述：将矩阵初始化为对角线为1的单位矩阵
##	参数描述：
##	matrix[4][4]：待初始化的4*4大小的矩阵
#################################################################*/
void MyGLWidget::mySetIdentityMatrix(GLfloat matrix[4][4])
{
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(i == j)
                matrix[i][j] = 1;
            else
                matrix[i][j] = 0;
        }
    }
}


/*################################################################
##	函数：myMultMatrix
##	函数描述：该函数对两个利用二维数组存储的矩阵进行矩阵乘法操作
##	参数描述：
##	m1[4][4]：4*4大小的二维数组，矩阵乘法左边矩阵，存储矩阵乘法结果
##	m2[4][4]：4*4大小的二维数组，矩阵乘法右边矩阵
#################################################################*/	
void MyGLWidget::myMultMatrix(GLfloat m1[4][4], GLfloat m2[4][4])
{
    GLfloat result[4][4]={{0.0f}};
    for(int i=0; i<4; i++)
        for(int k=0; k<4; k++)
            for(int j=0; j<4; j++)
                result[i][j] += m1[i][k] * m2[k][j];
    memcpy(m1, result, sizeof(GLfloat)*16);
}


/*################################################################
##	函数：myTranslate
##	函数描述：该函数通过给定的平移信息构造平移变换矩阵，并将平移矩
##			  阵与复合矩阵相乘得到新的复合矩阵
##	参数描述：
##	compositeMatrix[4][4]：多个仿射变换矩阵相乘得到复合矩阵
##	x, y, z：分别表示沿x轴, y轴, z轴方向平移的距离
#################################################################*/	
void MyGLWidget::myTranslate(GLfloat compositeMatrix[4][4], GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat translateMatrix[4][4]=
        {{1.0f, 0.0f, 0.0f, x},
         {0.0f, 1.0f, 0.0f, y},
         {0.0f, 0.0f, 1.0f, z},
         {0.0f, 0.0f, 0.0f, 1.0f}};
    myMultMatrix(compositeMatrix, translateMatrix);
}


/*################################################################
##	函数：myRotate
##	函数描述：该函数通过给定的旋转信息构造旋转变换矩阵，并将旋转矩
##			  阵与复合矩阵相乘得到新的复合矩阵
##	参数描述：
##	compositeMatrix[4][4]：多个仿射变换矩阵相乘得到复合矩阵
##	angle：旋转角大小，采用角度制
##	a, b, c：表示绕过原点的向量(a,b,c)进行旋转
#################################################################*/	
void MyGLWidget::myRotate(GLfloat compositeMatrix[4][4], GLfloat angle, GLfloat a, GLfloat b, GLfloat c)
{
	//将旋转角由角度制转为弧度制
    GLfloat radian = M_PI/180.0f*angle;
    //将向量进行归一化处理
    GLfloat vectorLength = sqrt(a*a+b*b+c*c);
    a /= vectorLength;
    b /= vectorLength;
    c /= vectorLength;
    GLfloat rotateMatrix[4][4]=
        {{a*a*(1-cos(radian))+cos(radian), b*a*(1-cos(radian))-c*sin(radian), c*a*(1-cos(radian))+b*sin(radian), 0.0f},
         {a*b*(1-cos(radian))+c*sin(radian), b*b*(1-cos(radian))+cos(radian), c*b*(1-cos(radian))-a*sin(radian), 0.0f},
         {a*c*(1-cos(radian))-b*sin(radian), b*c*(1-cos(radian))+a*sin(radian), c*c*(1-cos(radian))+cos(radian), 0.0f},
         {0.0f, 0.0f, 0.0f, 1.0f}};
    myMultMatrix(compositeMatrix, rotateMatrix);
}


/*################################################################
##	函数：myScale
##	函数描述：该函数通过给定的缩放信息构造缩放变换矩阵，并将缩放矩
##			  阵与复合矩阵相乘得到新的复合矩阵
##	参数描述：
##	compositeMatrix[4][4]：多个仿射变换矩阵相乘得到复合矩阵
##	x, y, z：分别表示沿x轴, y轴, z轴方向缩放的倍数
#################################################################*/	
void MyGLWidget::myScale(GLfloat compositeMatrix[4][4], GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat translateMatrix[4][4]=
        {{x, 0.0f, 0.0f, 0.0f},
         {0.0f, y, 0.0f, 0.0f},
         {0.0f, 0.0f, z, 0.0f},
         {0.0f, 0.0f, 0.0f, 1.0f}};
    myMultMatrix(compositeMatrix, translateMatrix);
}


/*################################################################
##	函数：coordinateTransformation
##	函数描述：该函数通过给定的仿射变换矩阵，将原坐标进行仿射变化后，
##			  得到变换之后的新坐标
##	参数描述：
##	matrix[4][4]：多个仿射变换组合得到的复合矩阵
##	x, y, z：表示原坐标，变换得到新坐标之后也用来存放新坐标
#################################################################*/	
void MyGLWidget::coordinateTransformation(GLfloat matrix[4][4], GLfloat &x, GLfloat &y, GLfloat &z)
{
    GLfloat tx=x, ty=y, tz=z;
    x = tx*matrix[0][0] + ty*matrix[0][1] + tz*matrix[0][2] + matrix[0][3];
    y = tx*matrix[1][0] + ty*matrix[1][1] + tz*matrix[1][2] + matrix[1][3];
    z = tx*matrix[2][0] + ty*matrix[2][1] + tz*matrix[2][2] + matrix[2][3];
}


/*################################################################
##	函数：swap
##	函数描述：交换两个int变量的值
##	参数描述：
##	x, y:待交换的两个int类型变量
#################################################################*/	
void swap(int &x, int &y)
{
    int tmp = x;
    x = y;
    y = tmp;
}


/*################################################################
##	函数：drwaLine
##	函数描述：利用Bresenham算法在两个点之间进行线段绘制
##	参数描述：
##	x1, y1：顶点1坐标
##	x2, y2：顶点2坐标
#################################################################*/	
void MyGLWidget::drawLine(int x1, int y1, int x2, int y2)
{
    //让x1始终小于x2
    if(x1 > x2){
        swap(x1, x2);
        swap(y1, y2);
    }

    //设置x和y为当前绘制的点的坐标
    int x, y;

    //如果斜率为0
    if(y1 == y2)
        for(x=x1; x<=x2; x++)
            glVertex2i(x, y1);
    //如果斜率不存在
    else if(x1 == x2){
        if(y1 > y2){
            for(y=y2; y<=y1; y++)
                glVertex2i(x1, y);
        }
        else{
            for(y=y1; y<=y2; y++)
                glVertex2i(x1, y);
        }
    }
    //如果斜率为1
    else if(x1-x2 == y1-y2){
        for(x=x1, y=y1; x<=x2 && y<=y2; x++, y++)
            glVertex2i(x, y);
    }
    //如果斜率为-1
    else if(x1-x2 == y2-y1){
        for(x=x1, y=y2; x<=x2 && y<=y1; x++, y++)
            glVertex2i(x, y);
    }
    //如果斜率绝对值小于1
    else if(abs(x1-x2) > abs(y1-y2)){
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int twody = 2 * dy;
        int twodiff = 2 * (dy - dx);
        int p = 2 * dy - dx;
        x = x1;
        y = y1;
        //如果斜率为正
        if(y1 < y2){
            while(x <= x2){
                glVertex2i(x, y);
                x++;
                if(p >= 0){
                    y++;
                    p += twodiff;
                }
                else
                    p += twody;
            }
        }
        //斜率为负
        else{
            while(x <= x2){
                glVertex2i(x, y);
                x++;
                if(p < 0){
                    y--;
                    p -= twodiff;
                }
                else
                    p -= twody;
            }
        }
    }
    //如果斜率绝对值大于1
    else{
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int twody = 2 * dx;
        int twodiff = 2 * (dx - dy);
        int p = 2 * dx - dy;
        x = x1;
        y = y1;
        //如果斜率为正
        if(y1 < y2){
            while(y <= y2){
                glVertex2i(x, y);
                y++;
                if(p >= 0){
                    x++;
                    p += twodiff;
                }
                else
                    p += twody;
            }
        }
        //斜率为负
        else{
            while(y >= y2){
                glVertex2i(x, y);
                y--;
                if(p < 0){
                    x++;
                    p -= twodiff;
                }
                else
                    p -= twody;
            }
        }
    }
}


/*################################################################
##	函数：myRotateByQuaternion
##	函数描述：该函数通过给定的旋转轴和角度得到对应的四元数，计算得到四元数对应的
##			 矩阵，与复合矩阵相乘得到新的复合矩阵
##	参数描述：
##	compositeMatrix[4][4]：多个仿射变换矩阵相乘得到复合矩阵
##	angle：旋转角大小，采用角度制
##	a, b, c：表示绕过原点的向量(a,b,c)进行旋转
#################################################################*/
void MyGLWidget::myRotateByQuaternion(GLfloat compositeMatrix[4][4], GLfloat angle, GLfloat a, GLfloat b, GLfloat c)
{
    //将旋转角由角度制转为弧度制
    GLfloat radian = M_PI/180.0f*angle/2;
    //将旋转轴向量进行归一化处理
    GLfloat vectorLength = sqrt(a*a+b*b+c*c);
    a /= vectorLength;
    b /= vectorLength;
    c /= vectorLength;
    GLfloat x=a*sin(radian), y=b*sin(radian), z=c*sin(radian), w=cos(radian);
    GLfloat rotateMatrix[4][4]=
        {{1-2*y*y-2*z*z, 2*(x*y-w*z), 2*(x*z+w*y), 0.0f},
         {2*(x*y+w*z), 1-2*x*x-2*z*z, 2*(y*z-w*x), 0.0f},
         {2*(x*z-w*y), 2*(y*z+w*x), 1-2*x*x-2*y*y, 0.0f},
         {0.0f, 0.0f, 0.0f, 1.0f}};
    myMultMatrix(compositeMatrix, rotateMatrix);
}
