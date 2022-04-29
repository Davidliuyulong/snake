#include<stdio.h>
#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
//多媒体设备播放音乐
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")


#define SNAKE_NUM 500;//蛇的最大节数
enum DIR//表示蛇的方向
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
using namespace std;
/*
  课程内容：贪吃蛇
  课程讲师：顽石老师
  知识点：结构体、循环、函数、easyx、数组
*/
//做界面1，创建一个窗口，图形窗口，
//蛇的结构
struct Snake {
	int size;//蛇的节数
	int dir=RIGHT;//蛇的方向
	int speed;//移动速度
	POINT coor[500];//坐标
}snake;
//食物结构
struct Food
{
	int x;
	int y;
	int r;     //食物大小
	bool flag;//食物是否被吃
	DWORD color;//食物颜色
}food;
void GameInit()
{
	//播放背景音乐
	mciSendString("open ./snake_bgm.mp3  alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);
	//init 初始化graph图形窗口,SHOWCONSOLE显示控制台
	initgraph(640, 480/*, SHOWCONSOLE*/);
	//设置随机数种子  GetTickCount();获取系统开机，到现在所经过的毫秒数 
	srand(GetTickCount());
	//初始化蛇
	snake.size = 3;
	snake.speed = 10;
	snake.dir;
	for (int i = 0; i < snake.size;i++) {
		snake.coor[i].x = 40-10*i;
		snake.coor[i].y = 10;
	}
	//初始化食物 rand()随机产生一个整数，如果没有设置随机数种子，每次产生的都是固定的整数
	//5%2==1
	//设置随机化种子，需要头文件stdlib.h 一般把时间作为种子，因为时间是在不断变化的
	food.x = rand()%640;
	food.y = rand()%480;
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	food.r = rand() % 10 + 5;
	food.flag = true;
	 
}
void GameDraw()
{
	//双缓冲绘图
	BeginBatchDraw();
	//设置背景颜色
	setbkcolor(RGB(20, 115, 119));
	cleardevice();
	//绘制蛇
	setfillcolor(GREEN);
	for (int i = 0; i < snake.size; i++) {
		solidcircle(snake.coor[i].x, snake.coor[i].y, 5);
	}
	EndBatchDraw();
	//绘制食物
	if (food.flag) {
		solidcircle(food.x, food.y, food.r);
	}
}
//移动蛇
void snakeMove()
{
	//让身体跟着头移动
	for (int i = snake.size - 1;i>0; i--) {
		snake.coor[i] = snake.coor[i - 1];
	}
	//移动是什么发生改变
	switch (snake.dir) {
	case UP:
		snake.coor[0].y-=snake.speed;
		if (snake.coor[0].y <= 0)//超出了上面的边界
		{
			snake.coor[0].y = 480;
		}
		break;
	case DOWN:
		snake.coor[0].y+= snake.speed;
		if (snake.coor[0].y >=480)//超出了下面的边界
		{
			snake.coor[0].y = 0;
		}
		break;
	case LEFT:
		snake.coor[0].x-= snake.speed;
		if (snake.coor[0].x <=0)//超出了左边的边界
		{
			snake.coor[0].x =640;
		}
		break;
	case RIGHT:
		snake.coor[0].x+= snake.speed;
		if (snake.coor[0].x >= 640)//超出了右边的边界
		{
			snake.coor[0].x = 0;
		}
		break;
	case ' '://游戏暂停逻辑
		while (1)
		{
			if (_getch() == ' ') 
				return ;
		}
		break;
	}

}
//通过按键来改变蛇的移动方向
void keyControl() {
	//判断有没有按键,如果有按键，就返回真
	if (_kbhit())
	{
		//72 80 75 77 上下左右按键
		switch (_getch())
		{
		case 'w':
		case 'W':
		case 72:
			//改变方向
			if(snake.dir!= DOWN){
			snake.dir = UP;
			}
			break;
		case 's':
		case 'S':
		case 80:
			if (snake.dir != UP) {
				snake.dir = DOWN;
			}
			break;
		case 'A':
		case 'a':
		case 75:
			if (snake.dir != RIGHT) {
				snake.dir = LEFT;
			}
			break;
		case 'D':
		case 'd':
		case 77:
			if (snake.dir != LEFT) {
				snake.dir = RIGHT;
			}
			break;
		}
	}
}
//吃食物
void EatFood() {
	if (food.flag&&snake.coor[0].x >= food.x-food.r&&snake.coor[0].x<=food.x+food.r
		&&snake.coor[0].y>=food.y-food.r&&snake.coor[0].y<=food.y+food.r) {
		food.flag = false;
		snake.size++;
	}
	//如果食物消失，则重新生成一个食物
	if (!food.flag) {
		food.x = rand() % 640;
		food.y = rand() % 480;
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.r = rand() % 10 + 5;
		food.flag = true;
	}
	/*
	可以加一个分数，出一个食物，加n分。  80%
	只要能够有收获就好
	*/
}
//游戏暂停(用死循环作暂停)
void stop() {
	if (_kbhit()) {
		if (_getch() == ' ')
		{
			while (1) {
				if (_getch() != ' ') {
					break;
				}
			}
		}
	}
}
int main() 
{
	GameInit();
	GameDraw();
	while (1) {
		GameDraw();
		snakeMove();
		keyControl();
		EatFood();
		Sleep(200);

	}


	return 0;
}
