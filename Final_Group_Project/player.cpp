#include "player.h"
/*###################################################
##  函数: Player
##  函数描述： 初始化游戏信息
##  参数描述： 无
#####################################################*/
Player::Player()
{
    //生成迷宫
    maze = new Maze(30, 30);
    //计算世界坐标下的起点和终点信息
    posX = maze->wallsize * 2.0f * maze->yOrigin;
    posZ = maze->wallsize * 2.0f * maze->xOrigin;
    targetXL = maze->wallsize * (2.0f * maze->yEnd - 1.0f);
    targetXR = maze->wallsize * (2.0f * maze->yEnd + 1.0f);
    targetZF = maze->wallsize * (2.0f * maze->xEnd + 1.0f);
    targetZB = maze->wallsize * (2.0f * maze->xEnd - 1.0f);
    invWallSize = 0.5f / maze->wallsize;
    //初始化摄像机信息
    camera = new Camera(
                vec3(posX, 1.6f * maze->wallsize , posZ),
                vec3(0.0f, 0.0f, 1000.0f),
                vec3(-1.0f, 0.0f, 0.0f), 0.0f, 90.0f);
}
/*###################################################
##  函数: ~Player
##  函数描述： 析构函数
##  参数描述： 无
#####################################################*/
Player::~Player()
{
    delete maze;
    delete camera;
}
/*###################################################
##  函数: FBwardMove
##  函数描述： 玩家前后移动
##  参数描述： action 移动动作
#####################################################*/
void Player::FBwardMove(Action action)
{
    //记录当前位置
    posX = camera->position.x;
    posZ = camera->position.z;
    if (action == forward)
    {
        //向前移动
        camera->FBwardMove(forward);
    }
    else if (action == backward)
    {
        //向后移动
        camera->FBwardMove(backward);

    }
    //如果位置非法，则恢复为移动前位置
    if (!maze->isPath(int(camera->position.z*invWallSize+0.5f), int(camera->position.x*invWallSize+0.5f)))
    {
        camera->position.x = posX;
        camera->position.z = posZ;
    }
}
/*###################################################
##  函数: HorizontalMove
##  函数描述： 玩家左右移动
##  参数描述： action 移动动作
#####################################################*/
void Player::HorizontalMove(Action action)
{
    //记录当前位置
    posX = camera->position.x;
    posZ = camera->position.z;
    if (action == toleft)
    {
        //向左移动
        camera->HorizontalMove(toleft);
    }
    else if (action == toright)
    {
        //向右移动
        camera->HorizontalMove(toright);
    }
    //如果位置非法，则恢复为移动前位置
    if (!maze->isPath(int(camera->position.z*invWallSize+0.5f), int(camera->position.x*invWallSize+0.5f)))
    {
        camera->position.x = posX;
        camera->position.z = posZ;
    }
}
/*###################################################
##  函数: checkWin
##  函数描述： 检测玩家是否到达终点
##  参数描述： 无
#####################################################*/
bool Player::checkWin()
{
    return (posX >= targetXL) && (posX <= targetXR) && (posZ >= targetZB) && (posZ <= targetZF);
}
