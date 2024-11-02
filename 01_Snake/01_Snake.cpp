// 禁用特定的编译器警告
#pragma warning(disable:4996)

// 包含所需的头文件
#include<iostream>
#include<windows.h> // 用于系统调用，如清屏
#include<time.h>    // 用于生成随机数和时间函数
#include<conio.h>   // 用于键盘输入，如kbhit()和getch()

// 定义棋盘的尺寸
#define H 22
#define W 22

// 使用标准命名空间
using namespace std;

// 定义棋盘类
class chessboard
{
public:
    char qp[H][W]; // 存储棋盘状态的二维数组
    int i, j, x1, y1; // 循环索引和食物坐标
    // 构造函数
    chessboard();
    // 生成食物的函数
    void food();
    // 打印棋盘和游戏信息的函数
    void prt(int grade, int score, int gamespeed);
};

// 棋盘类的构造函数实现
chessboard::chessboard()
{
    // 初始化棋盘内部为空白
    for (i = 1; i <= H - 2; i++)
        for (j = 1; j <= W - 2; j++)
            qp[i][j] = ' ';
    // 初始化棋盘边界为'#'
    for (i = 0; i <= H - 1; i++)
        qp[0][i] = qp[H - 1][i] = '#';
    for (i = 1; i <= H - 2; i++)
        qp[i][0] = qp[i][W - 1] = '#';
    // 调用生成食物的函数
    food();
}

// 生成食物的函数实现
void chessboard::food()
{
    // 初始化随机数种子
    srand(time(0));
    do
    {
        // 随机生成食物位置，直到找到一个空白位置
        x1 = rand() % W - 2 + 1;
        y1 = rand() % H - 2 + 1;
    } while (qp[x1][y1] != ' ');
    // 将食物位置标记为'$'
    qp[x1][y1] = '$';
}

// 打印棋盘和游戏信息的函数实现
void chessboard::prt(int grade, int score, int gamespeed)
{
    // 清屏
    system("cls");
    cout << endl;
    // 打印棋盘和游戏信息
    for (i = 0; i < H; i++)
    {
        cout << "\t";
        for (j = 0; j < W; j++)
            cout << qp[i][j] << ' ';
        if (i == 0) cout << "\tGrade:" << grade;
        if (i == 2) cout << "\tScore:" << score;
        if (i == 4) cout << "\tAutomatic forward";
        if (i == 5) cout << "\ttime interval:" << gamespeed << "ms";
        cout << endl;
    }
}

// 定义蛇类，继承自棋盘类
class snake :public chessboard
{
public:
    int zb[2][100]; // 存储蛇身体各部分位置的二维数组
    long start;     // 计时开始的时间
    int head, tail, grade, score, gamespeed, length, timeover, x, y; // 蛇的属性
    char direction; // 蛇的移动方向
    // 构造函数
    snake();
    // 蛇的移动函数
    void move();
};

// 蛇类的构造函数实现
snake::snake()
{
    // 打印游戏开始信息
    cout << "\n\n\t\tThe game is about to begin!" << endl;
    // 倒计时
    for (i = 3; i >= 0; i--)
    {
        start = clock();
        while (clock() - start <= 1000);
        system("cls");
        if (i > 0)
            cout << "\n\n\t\tCountdown:" << i << endl;
    }
    // 初始化蛇的身体
    for (i = 1; i <= 3; i++)
        qp[1][i] = '*';
    qp[1][4] = '@';
    // 初始化蛇身体位置数组
    for (i = 0; i < 4; i++)
    {
        zb[0][i] = 1;
        zb[1][i] = i + 1;
    }
}

// 蛇的移动函数实现
void snake::move()
{
    // 初始化得分、蛇的头部和尾部索引、等级、长度和游戏速度
    score = 0;
    head = 3, tail = 0;
    grade = 1, length = 4;
    gamespeed = 500;
    direction = 77;
    // 游戏主循环
    while (1)
    {
        timeover = 1;
        start = clock();
        while ((timeover = (clock() - start <= gamespeed)) && !kbhit());
        if (timeover)
        {
            getch();
            direction = getch();
        }
        // 根据方向键更新蛇头的位置
        switch (direction)
        {
        case 72: x = zb[0][head] - 1; y = zb[1][head]; break; // 上
        case 80: x = zb[0][head] + 1; y = zb[1][head]; break; // 下
        case 75: x = zb[0][head]; y = zb[1][head] - 1; break; // 左
        case 77: x = zb[0][head]; y = zb[1][head] + 1; break; // 右
        }
        // 检查蛇是否撞墙或撞到自己
        if (x == 0 || x == 21 || y == 0 || y == 21)
        {
            cout << "\tGame over!" << endl; break;
        }
        if (qp[x][y] != ' ' && !(x == x1 && y == y1))
        {
            cout << "\tGame over!" << endl; break;
        }
        // 如果蛇吃到食物
        if (x == x1 && y == y1)
        {
            length++;
            score = score + 100;
            if (length >= 8)
            {
                length -= 8;
                grade++;
                if (gamespeed >= 200)
                    gamespeed = 550 - grade * 50;
            }
            qp[x][y] = '@';
            qp[zb[0][head]][zb[1][head]] = '*';
            head = (head + 1) % 100;
            zb[0][head] = x;
            zb[1][head] = y;
            food();
            prt(grade, score, gamespeed);
        }
        else
        {
            qp[zb[0][tail]][zb[1][tail]] = ' ';
            tail = (tail + 1) % 100;
            qp[zb[0][head]][zb[1][head]] = '*';
            head = (head + 1) % 100;
            zb[0][head] = x;
            zb[1][head] = y;
            qp[zb[0][head]][zb[1][head]] = '@';
            prt(grade, score, gamespeed);
        }
    }
}

// 主函数
int main()
{
    chessboard cb; // 创建棋盘对象
    snake s;       // 创建蛇对象
    s.move();      // 开始游戏
}