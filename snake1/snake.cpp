#include<stdio.h>
#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
//��ý���豸��������
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")


#define SNAKE_NUM 500;//�ߵ�������
enum DIR//��ʾ�ߵķ���
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
using namespace std;
/*
  �γ����ݣ�̰����
  �γ̽�ʦ����ʯ��ʦ
  ֪ʶ�㣺�ṹ�塢ѭ����������easyx������
*/
//������1������һ�����ڣ�ͼ�δ��ڣ�
//�ߵĽṹ
struct Snake {
	int size;//�ߵĽ���
	int dir=RIGHT;//�ߵķ���
	int speed;//�ƶ��ٶ�
	POINT coor[500];//����
}snake;
//ʳ��ṹ
struct Food
{
	int x;
	int y;
	int r;     //ʳ���С
	bool flag;//ʳ���Ƿ񱻳�
	DWORD color;//ʳ����ɫ
}food;
void GameInit()
{
	//���ű�������
	mciSendString("open ./snake_bgm.mp3  alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);
	//init ��ʼ��graphͼ�δ���,SHOWCONSOLE��ʾ����̨
	initgraph(640, 480/*, SHOWCONSOLE*/);
	//�������������  GetTickCount();��ȡϵͳ�������������������ĺ����� 
	srand(GetTickCount());
	//��ʼ����
	snake.size = 3;
	snake.speed = 10;
	snake.dir;
	for (int i = 0; i < snake.size;i++) {
		snake.coor[i].x = 40-10*i;
		snake.coor[i].y = 10;
	}
	//��ʼ��ʳ�� rand()�������һ�����������û��������������ӣ�ÿ�β����Ķ��ǹ̶�������
	//5%2==1
	//������������ӣ���Ҫͷ�ļ�stdlib.h һ���ʱ����Ϊ���ӣ���Ϊʱ�����ڲ��ϱ仯��
	food.x = rand()%640;
	food.y = rand()%480;
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	food.r = rand() % 10 + 5;
	food.flag = true;
	 
}
void GameDraw()
{
	//˫�����ͼ
	BeginBatchDraw();
	//���ñ�����ɫ
	setbkcolor(RGB(20, 115, 119));
	cleardevice();
	//������
	setfillcolor(GREEN);
	for (int i = 0; i < snake.size; i++) {
		solidcircle(snake.coor[i].x, snake.coor[i].y, 5);
	}
	EndBatchDraw();
	//����ʳ��
	if (food.flag) {
		solidcircle(food.x, food.y, food.r);
	}
}
//�ƶ���
void snakeMove()
{
	//���������ͷ�ƶ�
	for (int i = snake.size - 1;i>0; i--) {
		snake.coor[i] = snake.coor[i - 1];
	}
	//�ƶ���ʲô�����ı�
	switch (snake.dir) {
	case UP:
		snake.coor[0].y-=snake.speed;
		if (snake.coor[0].y <= 0)//����������ı߽�
		{
			snake.coor[0].y = 480;
		}
		break;
	case DOWN:
		snake.coor[0].y+= snake.speed;
		if (snake.coor[0].y >=480)//����������ı߽�
		{
			snake.coor[0].y = 0;
		}
		break;
	case LEFT:
		snake.coor[0].x-= snake.speed;
		if (snake.coor[0].x <=0)//��������ߵı߽�
		{
			snake.coor[0].x =640;
		}
		break;
	case RIGHT:
		snake.coor[0].x+= snake.speed;
		if (snake.coor[0].x >= 640)//�������ұߵı߽�
		{
			snake.coor[0].x = 0;
		}
		break;
	case ' '://��Ϸ��ͣ�߼�
		while (1)
		{
			if (_getch() == ' ') 
				return ;
		}
		break;
	}

}
//ͨ���������ı��ߵ��ƶ�����
void keyControl() {
	//�ж���û�а���,����а������ͷ�����
	if (_kbhit())
	{
		//72 80 75 77 �������Ұ���
		switch (_getch())
		{
		case 'w':
		case 'W':
		case 72:
			//�ı䷽��
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
//��ʳ��
void EatFood() {
	if (food.flag&&snake.coor[0].x >= food.x-food.r&&snake.coor[0].x<=food.x+food.r
		&&snake.coor[0].y>=food.y-food.r&&snake.coor[0].y<=food.y+food.r) {
		food.flag = false;
		snake.size++;
	}
	//���ʳ����ʧ������������һ��ʳ��
	if (!food.flag) {
		food.x = rand() % 640;
		food.y = rand() % 480;
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.r = rand() % 10 + 5;
		food.flag = true;
	}
	/*
	���Լ�һ����������һ��ʳ���n�֡�  80%
	ֻҪ�ܹ����ջ�ͺ�
	*/
}
//��Ϸ��ͣ(����ѭ������ͣ)
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
