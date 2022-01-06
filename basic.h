#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<Windows.h>
#include <fmod.h> //������� ���� �ܺ� ���α׷�
#include<time.h>

#define X_MAX 79
#define Y_MAX 24

void gotoxy(int x, int y) //x��ǥ y��ǥ ��ġ����
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
	/*
	gotoxy �Լ��� ���� �Ҷ� �Ű����� x, y�� �޵��� �մϴ�. �츮�� ����ҋ��� 1,1�� ����ʹ� �̷��� gotoxy(1,1) �̷������� ����Ҽ� �ֵ��� ������.
	COORD ��� ����ü�� x, y ��ġ�� ������ �ִ� ����ü �Դϴ�. �� ����ü ������ ���� x�� y�� ä�� �־� �ݴϴ�.
	2*x�� �־��ִ� ������ Ŀ���� 1�� �̵��ϴ°ͺ��� 2�� �̵��ϴ°� �� �ڿ������� ���̱� ������ 2*x�� �־��ݴϴ�.
	SetConsoleCursorPosition �Լ��� �̸����� �˼� �ֵ��� �ܼ��� Ŀ�� ��ġ�� �˷��ִ� �Լ� �Դϴ�.
	ù��° �Ű������δ� ȭ�� ��� �ڵ鷯  GetStdHandle(STD_OUTPUT_HANDLE)�� �־��ְ� �ι�° �Ű������δ� ��ġ�� ��Ÿ���� COORD Ÿ�� ������ Pos�� �־��ݴϴ�.
	ù��° �Ű��������� �� �ֽ��ϴ�. STD_OUTPUT_HANDLE�� ������ ���̽��� �⺻ ����� ���ϴ� ��ũ�� �̰�,
	�� ��ũ�ο� �ش��ϴ� �ڵ鷯�� GetStdHandle()�� �޾ƿɴϴ�.
	�̰Ŵ� �׳� "�� ��¿� �ڵ鷯�� ������ ���°ű���" �ϸ� �ɰͰ����ϴ�.
	�̷��� Ŀ���� �̵���Ű�� �Լ��� �ϼ��߽��ϴ�.
	*/
}
void setcolor(int color, int bgcolor)
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}
void move_cursor(char key, int* x, int* y, int x_m, int y_m)
{
	switch (key)
	{
	case 72:
		*y = *y - 1;
		if (*y < 1) *y += 1;
		break;
	case 75:
		*x = *x - 1;
		if (*x < 1) *x += 1;
	case 77:
		*x += 1;
		if (*x > x_m) *x -= 1;
	case 80:
		*y += 1;
		if (*y > y_m) *y -= 1;
		break;
	}
}
