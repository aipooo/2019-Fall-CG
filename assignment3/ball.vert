#version 330 core
layout (location = 0) in vec3 originalPointPos; //未经过变换的点
out vec3 PointPos;      //经过变换后的点
out mat3 normalMat; //法线变换矩阵
out vec3 Point2Camera;     //圆心到光源的向量

uniform float radius;       //点的初始大小
uniform mat4 model;         //MODEL矩阵
uniform mat4 view;          //VIEW矩阵
uniform mat4 projection;    //PROJECTION矩阵
uniform vec3 cameraPos;     //摄像机位置

const float constant = 1.0;     //点大小衰减常数系数
const float linear = 0.1;       //点大小衰减线性系数
const float quadratic = 0.01;   //点大小衰减二次系数

float calBallSize()
{
    //计算点到摄像机的距离
    float dist = distance(originalPointPos, cameraPos);
    Point2Camera = cameraPos - originalPointPos;
    //计算动态变化的小球大小
    float size = radius * (1.0 / (constant + linear * dist + quadratic * dist * dist));
    return size;
}


void main()
{
    gl_PointSize = calBallSize();
    //计算变换后的位置
    PointPos = vec3(model * vec4(originalPointPos, 1.0));
    gl_Position = projection * view * vec4(PointPos, 1.0);
    normalMat = mat3(transpose(inverse(model)));
}
