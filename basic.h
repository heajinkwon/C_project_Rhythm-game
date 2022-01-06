#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<Windows.h>
#include <fmod.h> //배경음을 위한 외부 프로그램
#include<time.h>

#define X_MAX 79
#define Y_MAX 24

void gotoxy(int x, int y) //x좌표 y좌표 위치설정
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
	/*
	gotoxy 함수를 정의 할때 매개변수 x, y를 받도록 합니다. 우리가 사용할떄는 1,1로 가고싶다 이럴때 gotoxy(1,1) 이런식으로 사용할수 있도록 말이죠.
	COORD 라는 구조체는 x, y 위치를 가지고 있는 구조체 입니다. 이 구조체 변수를 만들어서 x와 y를 채워 넣어 줍니다.
	2*x를 넣어주는 이유는 커서가 1씩 이동하는것보다 2씩 이동하는게 더 자연스러워 보이기 때문에 2*x를 넣어줍니다.
	SetConsoleCursorPosition 함수는 이름에서 알수 있듯이 콘솔의 커서 위치를 알려주는 함수 입니다.
	첫번째 매개변수로는 화면 출력 핸들러  GetStdHandle(STD_OUTPUT_HANDLE)를 넣어주고 두번째 매개변수로는 위치를 나타내는 COORD 타입 변수인 Pos를 넣어줍니다.
	첫번째 매개변수에는 을 넣습니다. STD_OUTPUT_HANDLE은 윈도우 베이스의 기본 출력을 뜻하는 매크로 이고,
	그 매크로에 해당하는 핸들러를 GetStdHandle()로 받아옵니다.
	이거는 그냥 "아 출력용 핸들러를 가지고 오는거구나" 하면 될것같습니다.
	이렇게 커서를 이동시키는 함수를 완성했습니다.
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
