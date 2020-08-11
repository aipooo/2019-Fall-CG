# 2019-Fall-CG
SYSU TJ 计算机图形学

**Assignment1：复现图中场景**
* 要求：不使用GL_LINES或GL_LINE_STRIP，glTranslate, glRotate, glMultMatrix
* 实现平移旋转矩阵的构造
* 使用矩阵乘法对点进行变换
  * 可选做：使用四元数实现旋转
* 实现二维画线算法
  * 将变换后的点投影至二维
  * 于屏幕空间中连接两个二维顶点
  * 可选做：抗锯齿
* 需保证在窗口大小改变时，绘制结果与模板一致
<br/>

**Assignment2：绘制一个沿固定线路运动的机器人**
* 线路可以是圆或任意其他闭合路径
* 机器人应该有头、躯干、四肢等基本部分
* 机器人在运动过程中应具有摆臂及抬腿两个基本动作
* 可选
  * 使用mesh模型（如obj文件）载入机器人模型
  * 或载入其他mesh模型围绕机器人运动
<br/>

**Assignment3：使用GL_POINTS绘制沿固定轨道运动的小球**
* 每个glVertex调用指明一个小球的球心位置
* 小球大小根据离观察点距离变化（近大远小）
* 使用Phong Shading
* 选做
  * 使用vbo进行绘制
<br/>

**Final Group Project**
* 期末大作业选题内容
  * 实现较复杂算法
    * 全局光照，网格细分与细化，几何模型的形变，压缩，顺序无关的半透明物体渲染等
  * 创建复杂场景，动画，交互
    * 短影片，游戏等
  * 可视化内容
    * 标量场，流场可视化，及其与几何模型的结合
  * 不可提交图像处理相关内容
* 小组选题：**基于OpenGL的3D迷宫游戏设计**
  * 项目的框架基于Qt的OpenGL模块
  * 利用Prim迷宫生成算法随机生成二维矩阵，以此构建3D迷宫
  * 系统设置了一架摄像机，来模拟用户在迷宫中的第一视角
  * 用户可以通过鼠标、键盘控制自己在3D迷宫中的移动
  * 游戏同时进行了碰撞检测
  * 对迷宫中的围墙、地板进行纹理贴图
  * 完成填空盒的创建
  * 具体内容参见实验报告
<br/>

**All assignments are based on OpenGL**

**If you have any question, you can e-mail to xuewh5@mail2.sysu.edu.cn**
