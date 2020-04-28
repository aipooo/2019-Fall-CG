#include "maze.h"
/*###################################################
##  函数: Maze
##  函数描述： 初始化迷宫参数，随机生成迷宫
##  参数描述： height, width 迷宫宽高
#####################################################*/
Maze::Maze(int height, int width)
{
    Height = height;
    Width = width;
    xOrigin = 0;
    yOrigin = 1;
    xEnd = Height - 2;
    yEnd = Width - 1;
    std::fill(maze[0], maze[0] + Height*Width, 1);
    wallsize = 70.0f;
    //随机生成迷宫
    randGo(1, 1);
}
Maze::~Maze()
{

}
/*###################################################
##  函数: isArea
##  函数描述： 判断位置是否在迷宫内
##  参数描述： x, y 目标位置
#####################################################*/
bool Maze::isArea(int x, int y)
{
    return x >= 0 && x < Height && y >= 0 && y < Width;
}
/*###################################################
##  函数: isWall
##  函数描述： 判断位置是否是墙体
##  参数描述： x, y 目标位置
#####################################################*/
bool Maze::isWall(int x, int y)
{
    return isArea(x, y) && maze[x][y] == 1;
}
/*###################################################
##  函数: isPath
##  函数描述： 判断位置是否是路径
##  参数描述： x, y 目标位置
#####################################################*/
bool Maze::isPath(int x, int y)
{
    return isArea(x, y) && maze[x][y] == 0;
}
/*###################################################
##  函数: randGo
##  函数描述： 随机迷宫生成算法
##  参数描述： x, y 迷宫起点
#####################################################*/
void Maze::randGo(int x, int y)
{
    //设置一个中间的二维数组保存迷宫信息
    int tmpMaze[100][100];
    //将迷宫的边缘初始化为路，防止出现多个开口
    //其余地方全部初始化为墙
    for (int i = 0; i < Height+2; i++)
    {
        for (int j = 0; j < Width+2; j++)
        {
            tmpMaze[i][j] = 1;
            if (i == 0 || i == Height+1) tmpMaze[i][j] = 0;
            if (j == 0 || j == Width+1) tmpMaze[i][j] = 0;
        }
    }

    std::vector<std::pair<int, int>> wall;          //设置墙队列
    wall.push_back(std::pair<int, int>(x+1, y+1));  //将起点添加进墙队列
    srand(time(NULL));
    while(!wall.empty())    //如果墙队列不为空
    {
        //从墙队列中随机选择一个墙体出来
        int index = rand() % wall.size();
        std::pair<int, int> pos = wall[index];
        //统计该墙体上下左右是路的个数
        int count = 0;
        for (int i = 0; i < 4; i++)
        {
            int newX = pos.first + dir[i][0];
            int newY = pos.second + dir[i][1];
            if (tmpMaze[newX][newY] == 0) count++;
        }
        //如果该位置上下左右中是路的个数小于1，则可以将该位置设为路
        //同时将该位置上下左右中是墙的位置添加进墙队列
        if (count <= 1)
        {
            tmpMaze[pos.first][pos.second] = 0;
            for(int i = 0; i < 4; i++)
            {
                int newX = pos.first + dir[i][0];
                int newY = pos.second + dir[i][1];
                if (tmpMaze[newX][newY] != 0) wall.push_back(std::pair<int, int>(newX, newY));
            }
        }
        //将该墙体从墙队列中移除
        wall.erase(wall.begin() + index);
    }
    //由于tmpMaze比实际的迷宫大了一圈无用的边缘
    //我们将tmpMaze边缘一圈去掉，得到实际生成的迷宫
    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            maze[i][j] = tmpMaze[i+1][j+1];
        }
    }
    //计算起点和终点位置
    maze[xOrigin][yOrigin] = 0;
    for (int i = Height-1; i >= 0; i--)
    {
        if (maze[i][Width-2] == 0)
        {
            maze[i][Width-1] = 0;
            xEnd = i;
            yEnd = Width - 1;
            break;
        }
    }
}

