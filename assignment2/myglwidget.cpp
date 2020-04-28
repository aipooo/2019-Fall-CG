#include "myglwidget.h"
#include <iostream>

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
    timer->start(0.1); // 时间间隔设置为16ms，可以根据需要调整
	connect(timer, SIGNAL(timeout()), this, SLOT(update())); // 连接update()函数，每16ms触发一次update()函数进行重新绘图
    angleofllarm = angleofluarm = angleofrlarm = angleofruarm = angleofluleg = angleofruleg = angleofllleg = angleofrlleg = 0.0f;
    llarm = rlarm = llleg = rlleg = 0;
    uarm = 0.2;
    uleg = 0.1;
    angle = 0.3;
    c = 0.07;
    angle1 = 0.2;
    angle2 = 0.1;
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
	glViewport(0, 0, width(), height());  
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
	glDisable(GL_DEPTH_TEST);             
}



/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, width()/height(), 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -35.0f);   //将机器人初始位置沿z轴负方向平移35个单位
    glRotatef(-angle, 0, 1, 0);          //设置机器人每次绕y轴旋转-angle度
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 15.0f);   //设置机器人运动半径
        glRotatef(-90, 0, 1, 0);            //设置机器人初始朝向为x轴负方向
        DrawBody();                         //绘制机器人body
        //绘制机器人上半部分
        glPushMatrix();
            glTranslatef(0.0f, 1.2f, 0.0f);     	//移动至机器人肩膀中心S
            DrawShoulder();                     	//绘制机器人肩膀
            glPushMatrix();
                glTranslatef(0.8, -0.4, 0.0);		//移动至机器人左上臂LUA
                glRotatef(angleofluarm, 1, 0, 0);	//旋转左上臂
                DrawArm();     						//绘制机器人左上臂
                glPushMatrix();
                    glTranslatef(0.0, -1.1, 0.0);	//移动至机器人左下臂LLA
                    glRotatef(angleofllarm, 1, 0, 0);//旋转左下臂
                    DrawArm();     					//绘制机器人左下臂
                glPopMatrix();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(-0.8, -0.4, 0.0);		//移动至机器人右上臂RUA
                glRotatef(angleofruarm, 1, 0, 0);	//旋转右上臂
                DrawArm();     						//绘制机器人右上臂
                glPushMatrix();
                    glTranslatef(0.0, -1.1, 0.0);	//移动至机器人右下臂
                    glRotatef(angleofrlarm, 1, 0, 0);//旋转右下臂
                    DrawArm();     					//绘制机器人右下臂
                glPopMatrix();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.0, 0.3, 0.0);		//移动至机器人脖子中心N
                DrawNeck();                     	//绘制机器人脖子
                glPushMatrix();
                    glTranslatef(0.0f, 0.7f, 0.0f);	//移动至机器人头部中心Head
                    DrawHead();                 	//绘制机器人头部
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
        //绘制机器人下半部分
        glPushMatrix();
            glTranslatef(0.0f, -1.2f, 0.0f);		//移动至机器人髋部中心HIP
            DrawHip();      						//绘制机器人髋部
            glPushMatrix();
                glTranslatef(0.4f, -0.4f, 0.0f);	//移动至机器人左大腿LUL
                glRotatef(angleofluleg, 1, 0, 0);	//旋转左大腿
                DrawLeg();     						//绘制机器人右上腿
                glPushMatrix();
                    glTranslatef(0.0, -1.5, 0.0);	//移动至机器人左小腿
                    glRotatef(angleofllleg, 1, 0, 0);//旋转左小腿
                    DrawLeg();						//绘制机器人左小腿
                glPopMatrix();
            glPopMatrix();           
            glPushMatrix();
                glTranslatef(-0.4f, -0.4f, 0.0f);	//移动至机器人右大腿RUL
                glRotatef(angleofruleg, 1, 0, 0);	//旋转右大腿
                DrawLeg();							//绘制机器人右大腿
                glPushMatrix();						
                    glTranslatef(0.0, -1.5, 0.0);	//移动至机器人右小腿RLL
                    glRotatef(angleofrlleg, 1, 0, 0);//旋转右小腿
                    DrawLeg();						//绘制机器人右小腿
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();

    angleofllarm += llarm;
    angleofrlarm += rlarm;
    angleofluarm += uarm;
    angleofruarm -= uarm;

    angleofllleg += llleg;
    angleofrlleg += rlleg;
    angleofluleg -= uleg;
    angleofruleg += uleg;
    angle += c;
	
	if (uarm>0){
		if(0 < angleofluarm && angleofluarm < 50){
			uarm = angle1;
			uleg = angle2;
			llarm = -angle1*0.75;
			rlarm = -angle1;
			llleg = 0.75*angle2;
			rlleg = angle2*1.5;		
		}
		else if (angleofluarm >=50) {
			uarm = -angle1;
			uleg = -angle2;
		}
		else if (angleofluarm < 0) {
			uarm = angle1;
			uleg = angle2;
			llarm = angle1;
			rlarm = angle1*0.75;
			llleg = -angle2*1.5;
			rlleg = -angle2*0.75;
		}
	}
	else{
		if (-50<angleofluarm && angleofluarm <= 0) {
			uarm = -angle1;
			uleg = -angle2;
			llarm = -angle1;
			rlarm = -angle1*0.75;
			llleg = angle2*1.5;
			rlleg = angle2*0.75;
		}
		else if (angleofluarm > 0) {
			uarm = -angle1;
			uleg = -angle2;
			llarm = angle1*0.75;
			rlarm = angle1;
			llleg = -angle2*0.75;
			rlleg = -angle2*1.5;
		}
		else if (angleofluarm <= -50) {
			uarm = angle1;
			uleg = angle2;
		}	
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
##  函数: DrawBody
##  函数描述： 绘制机器人躯干
##  参数描述： 无
#####################################################*/
void MyGLWidget::DrawBody() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        //正面
        glVertex3f(-0.5f, 1.0f, 0.25f);
        glVertex3f(0.5f, 1.0f, 0.25f);
        glVertex3f(0.5f, -1.0f, 0.25f);
        glVertex3f(-0.5f, -1.0f, 0.25f);
        //背面
        glVertex3f(0.5f, 1.0f, -0.25f);
        glVertex3f(-0.5f, 1.0f, -0.25f);
        glVertex3f(-0.5f, -1.0f, -0.25f);
        glVertex3f(0.5f, -1.0f, -0.25f);
        //左面
        glVertex3f(-0.5f, 1.0f, 0.25f);
        glVertex3f(-0.5f, 1.0f, -0.25f);
        glVertex3f(-0.5f, -1.0f, -0.25f);
        glVertex3f(-0.5f, -1.0f, 0.25f);
        //右面
        glVertex3f(0.5f, 1.0f, 0.25f);
        glVertex3f(0.5f, 1.0f, -0.25f);
        glVertex3f(0.5f, -1.0f, -0.25f);
        glVertex3f(0.5f, -1.0f, 0.25f);
        //顶面
        glVertex3f(0.5f, 1.0f, 0.25f);
        glVertex3f(0.5f, 1.0f, -0.25f);
        glVertex3f(-0.5f, 1.0f, -0.25f);
        glVertex3f(-0.5f, 1.0f, 0.25f);
        //底面
        glVertex3f(0.5f, -1.0f, 0.25f);
        glVertex3f(0.5f, -1.0f, -0.25f);
        glVertex3f(-0.5f, -1.0f, -0.25f);
        glVertex3f(-0.5f, -1.0f, 0.25f);
    glEnd();
}


/*###################################################
##  函数: DrawShoulder
##  函数描述： 绘制机器人肩膀
##  参数描述： 无
#####################################################*/
void MyGLWidget::DrawShoulder() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        //正面
        glVertex3f(-1.0f, 0.2f, 0.25f);
        glVertex3f(1.0f, 0.2f, 0.25f);
        glVertex3f(1.0f, -0.2f, 0.25f);
        glVertex3f(-1.0f, -0.2f, 0.25f);
        //背面
        glVertex3f(1.0f, 0.2f, -0.25f);
        glVertex3f(-1.0f, 0.2f, -0.25f);
        glVertex3f(-1.0f, -0.2f, -0.25f);
        glVertex3f(1.0f, -0.2f, -0.25f);
        //左面
        glVertex3f(-1.0f, 0.2f, 0.25f);
        glVertex3f(-1.0f, 0.2f, -0.25f);
        glVertex3f(-1.0f, -0.2f, -0.25f);
        glVertex3f(-1.0f, -0.2f, 0.25f);
        //右面
        glVertex3f(1.0f, 0.2f, 0.25f);
        glVertex3f(1.0f, 0.2f, -0.25f);
        glVertex3f(1.0f, -0.2f, -0.25f);
        glVertex3f(1.0f, -0.2f, 0.25f);
        //顶面
        glVertex3f(1.0f, 0.2f, 0.25f);
        glVertex3f(1.0f, 0.2f, -0.25f);
        glVertex3f(-1.0f, 0.2f, -0.25f);
        glVertex3f(-1.0f, 0.2f, 0.25f);
        //底面
        glVertex3f(1.0f, -0.2f, 0.25f);
        glVertex3f(1.0f, -0.2f, -0.25f);
        glVertex3f(-1.0f, -0.2f, -0.25f);
        glVertex3f(-1.0f, -0.2f, 0.25f);
    glEnd();
}


/*###################################################
##  函数: DrawHip
##  函数描述： 绘制机器人髋部
##  参数描述： 无
#####################################################*/
void MyGLWidget::DrawHip() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        //正面
        glVertex3f(-0.7f, 0.2f, 0.25f);
        glVertex3f(0.7f, 0.2f, 0.25f);
        glVertex3f(0.7f, -0.2f, 0.25f);
        glVertex3f(-0.7f, -0.2f, 0.25f);
        //背面
        glVertex3f(0.7f, 0.2f, -0.25f);
        glVertex3f(-0.7f, 0.2f, -0.25f);
        glVertex3f(-0.7f, -0.2f, -0.25f);
        glVertex3f(0.7f, -0.2f, -0.25f);
        //左面
        glVertex3f(-0.7f, 0.2f, 0.25f);
        glVertex3f(-0.7f, 0.2f, -0.25f);
        glVertex3f(-0.7f, -0.2f, -0.25f);
        glVertex3f(-0.7f, -0.2f, 0.25f);
        //右面
        glVertex3f(0.7f, 0.2f, 0.25f);
        glVertex3f(0.7f, 0.2f, -0.25f);
        glVertex3f(0.7f, -0.2f, -0.25f);
        glVertex3f(0.7f, -0.2f, 0.25f);
        //顶面
        glVertex3f(0.7f, 0.2f, 0.25f);
        glVertex3f(0.7f, 0.2f, -0.25f);
        glVertex3f(-0.7f, 0.2f, -0.25f);
        glVertex3f(-0.7f, 0.2f, 0.25f);
        //底面
        glVertex3f(0.7f, -0.2f, 0.25f);
        glVertex3f(0.7f, -0.2f, -0.25f);
        glVertex3f(-0.7f, -0.2f, -0.25f);
        glVertex3f(-0.7f, -0.2f, 0.25f);
    glEnd();
}


/*###################################################
##  函数: DrawArm
##  函数描述： 绘制机器人手臂
##  参数描述： 无
#####################################################*/
void MyGLWidget::DrawArm() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        //正面
        glVertex3f(-0.2f, 0.2f, 0.25f);
        glVertex3f(0.2f, 0.2f, 0.25f);
        glVertex3f(0.2f, -1.0f, 0.25f);
        glVertex3f(-0.2f, -1.0f, 0.25f);
        //背面
        glVertex3f(0.2f, 0.2f, -0.25f);
        glVertex3f(-0.2f, 0.2f, -0.25f);
        glVertex3f(-0.2f, -1.0f, -0.25f);
        glVertex3f(0.2f, -1.0f, -0.25f);
        //左面
        glVertex3f(-0.2f, 0.2f, 0.25f);
        glVertex3f(-0.2f, 0.2f, -0.25f);
        glVertex3f(-0.2f, -1.0f, -0.25f);
        glVertex3f(-0.2f, -1.0f, 0.25f);
        //右面
        glVertex3f(0.2f, 0.2f, 0.25f);
        glVertex3f(0.2f, 0.2f, -0.25f);
        glVertex3f(0.2f, -1.0f, -0.25f);
        glVertex3f(0.2f, -1.0f, 0.25f);
        //顶面
        glVertex3f(0.2f, 0.2f, 0.25f);
        glVertex3f(0.2f, 0.2f, -0.25f);
        glVertex3f(-0.2f, 0.2f, -0.25f);
        glVertex3f(-0.2f, 0.2f, 0.25f);
        //底面
        glVertex3f(-0.2f, -1.0f, 0.25f);
        glVertex3f(0.2f, -1.0f, 0.25f);
        glVertex3f(0.2f, -1.0f, -0.25f);
        glVertex3f(-0.2f, -1.0f, -0.25f);
    glEnd();
}


/*###################################################
##  函数: DrawLeg
##  函数描述： 绘制机器人腿部
##  参数描述： 无
#####################################################*/
void MyGLWidget::DrawLeg() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        //正面
        glVertex3f(-0.3f, 0.25f, 0.255f);
        glVertex3f(0.3f, 0.25f, 0.255f);
        glVertex3f(0.3f, -1.25f, 0.255f);
        glVertex3f(-0.3f, -1.25f, 0.255f);
        //背面
        glVertex3f(0.3f, 0.25f, -0.255f);
        glVertex3f(-0.3f, 0.25f, -0.255f);
        glVertex3f(-0.3f, -1.25f, -0.255f);
        glVertex3f(0.3f, -1.25f, -0.255f);
        //左面
        glVertex3f(-0.3f, 0.25f, 0.255f);
        glVertex3f(-0.3f, 0.25f, -0.255f);
        glVertex3f(-0.3f, -1.25f, -0.255f);
        glVertex3f(-0.3f, -1.25f, 0.255f);
        //右面
        glVertex3f(0.3f, 0.25f, 0.255f);
        glVertex3f(0.3f, 0.25f, -0.255f);
        glVertex3f(0.3f, -1.25f, -0.255f);
        glVertex3f(0.3f, -1.25f, 0.255f);
        //顶面
        glVertex3f(0.3f, 0.25f, 0.255f);
        glVertex3f(0.3f, 0.25f, -0.255f);
        glVertex3f(-0.3f, 0.25f, -0.255f);
        glVertex3f(-0.3f, 0.25f, 0.255f);
        //底面
        glVertex3f(0.3f, -1.25f, 0.255f);
        glVertex3f(0.3f, -1.25f, -0.255f);
        glVertex3f(-0.3f, -1.25f, -0.255f);
        glVertex3f(-0.3f, -1.25f, 0.255f);
    glEnd();
}


/*###################################################
##  函数: DrawHead
##  函数描述： 绘制机器人头部
##  参数描述： 无
#####################################################*/
void MyGLWidget::DrawHead() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        //正面
        glVertex3f(-0.6f, 0.6f, 0.35f);
        glVertex3f(0.6f, 0.6f, 0.35f);
        glVertex3f(0.6f, -0.6f, 0.35f);
        glVertex3f(-0.6f, -0.6f, 0.35f);
        //背面
        glVertex3f(0.6f, 0.6f, -0.35f);
        glVertex3f(-0.6f, 0.6f, -0.35f);
        glVertex3f(-0.6f, -0.6f, -0.35f);
        glVertex3f(0.6f, -0.6f, -0.35f);
        //左面
        glVertex3f(-0.6f, 0.6f, 0.35f);
        glVertex3f(-0.6f, 0.6f, -0.35f);
        glVertex3f(-0.6f, -0.6f, -0.35f);
        glVertex3f(-0.6f, -0.6f, 0.35f);
        //右面
        glVertex3f(0.6f, 0.6f, 0.35f);
        glVertex3f(0.6f, 0.6f, -0.35f);
        glVertex3f(0.6f, -0.6f, -0.35f);
        glVertex3f(0.6f, -0.6f, 0.35f);
        //顶面
        glVertex3f(0.6f, 0.6f, 0.35f);
        glVertex3f(0.6f, 0.6f, -0.35f);
        glVertex3f(-0.6f, 0.6f, -0.35f);
        glVertex3f(-0.6f, 0.6f, 0.35f);
        //底面
        glVertex3f(0.6f, -0.6f, 0.35f);
        glVertex3f(0.6f, -0.6f, -0.35f);
        glVertex3f(-0.6f, -0.6f, -0.35f);
        glVertex3f(-0.6f, -0.6f, 0.35f);
    glEnd();
}


/*###################################################
##  函数: DrawNeck
##  函数描述： 绘制机器人颈部
##  参数描述： 无
#####################################################*/
void MyGLWidget::DrawNeck() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        //正面
        glVertex3f(-0.2f, 0.1f, 0.25f);
        glVertex3f(0.2f, 0.1f, 0.25f);
        glVertex3f(0.2f, -0.1f, 0.25f);
        glVertex3f(-0.2f, -0.1f, 0.25f);
        //背面
        glVertex3f(0.2f, 0.1f, -0.25f);
        glVertex3f(-0.2f, 0.1f, -0.25f);
        glVertex3f(-0.2f, -0.1f, -0.25f);
        glVertex3f(0.2f, -0.1f, -0.25f);
        //左面
        glVertex3f(-0.2f, 0.1f, 0.25f);
        glVertex3f(-0.2f, 0.1f, -0.25f);
        glVertex3f(-0.2f, -0.1f, -0.25f);
        glVertex3f(-0.2f, -0.1f, 0.25f);
        //右面
        glVertex3f(0.2f, 0.1f, 0.25f);
        glVertex3f(0.2f, 0.1f, -0.25f);
        glVertex3f(0.2f, -0.1f, -0.25f);
        glVertex3f(0.2f, -0.1f, 0.25f);
        //顶面
        glVertex3f(0.2f, 0.1f, 0.25f);
        glVertex3f(0.2f, 0.1f, -0.25f);
        glVertex3f(-0.2f, 0.1f, -0.25f);
        glVertex3f(-0.2f, 0.1f, 0.25f);
        //底面
        glVertex3f(0.2f, -0.1f, 0.25f);
        glVertex3f(0.2f, -0.1f, -0.25f);
        glVertex3f(-0.2f, -0.1f, -0.25f);
        glVertex3f(-0.2f, -0.1f, 0.25f);
    glEnd();
}

