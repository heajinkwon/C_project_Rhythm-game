#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include<string.h>
#include<conio.h>
#include<Windows.h>
#include<time.h>
#include <fmod.h>

#define X_MAX 79
#define Y_MAX 24

#define arrMAXSIZE (50)
#define MAX_RANK (10)

//����ü ���� �κ� 
struct song
{
    char songname[20];      //���
    char artist[20];        //�۰��
};

typedef struct _info {
    char	name[24];       //�̸�
    int		score;          //����
    int		rank;           //����
}INFO;

int  draw_title(int x, int y);					                              //Ÿ��Ʋȭ��
void draw_explanation(void);					                              //���� ȭ��
void draw_equalizer(int* equalizer_count, char pic[][8]);                     // �� ���� ȭ��-�ִϸ��̼�
void draw_song(int* sel, char key, struct song*);                             //�� ���� ȭ��-�� ��� ���
void wirte_somnail(struct song* sn);			                              //���� �Է� �Լ�
void move_draw_song(char key, struct song*);	                              //�� ���� ȭ��-�� ��� �ű��
int draw_select_song(int x_m, int y_m, int x, int y);	                      //�� �ű�� �Լ� ����Ͽ� ��� + ���, �۰ �� ���߱�
int draw_game_screen(int song, int diff);                            //�ΰ��� ȭ��
void draw_key_effect(char* key);                                              //����ȭ��-Ű�� ���� �� ����
void delete_key(char* key);                                                   //����ȭ��-����Ʈ ���� �� �ʱ�ȭ
void note_to_note(int* num, int notelen, char f_note[][5], int note[4][24]);  //����ȭ��-�ؽ�Ʈ�� �ҷ��� ��Ʈ�� �������� �ű��
void draw_note(int vac[4][24]);                                               //����ȭ��-��Ʈ �׸���
int judge_acc(char* key, int note[4][24]);                                    //����ȭ��-��Ʈ ġ�� Ÿ�̹� �Ǵ� �Լ�
void draw_judge(int* jud);                                                    //����ȭ��-�޺���Ī(perfect/great/good/bad...) ����Լ�
void draw_score_combo_life_coin();                                            //����ȭ��-����,�޺� ��, ����, �������� ��
void erease_judge();                                                          //����ȭ��-����Ʈ�� ����
bool qus_use_coin();                                                          //���� ��� ���� �Ǻ�
void random_coin();                                                           //�������λ��� ���
void game_clear();                                                            //���� Ŭ����� ���â
void game_stop();                                                             //���� �߰��� ���� �� ���â
void game_over();                                                             //���� Ż�� �� ���â
void rank(INFO temp, int score);                                              //���� �ű������ �̸� �Է¹ޱ�, ���� ���Ͽ� ����
void print_rank(INFO p[arrMAXSIZE]);                                          //��ũ ���

void gotoxy(int x, int y)         //x�� y�� ����� ��ǥ�� Ŀ���� �Ű��ִ� �Լ�
{
    COORD Pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void move_cursor(char key, int* x, int* y, int x_m, int y_m)   //ȭ��ǥ ����(key) ������� Ŀ�� ��ǥ �� ����
{
    switch (key)
    {
    case 72:        //������
        *y = *y - 1;
        if (*y < 1) *y += 1;
        break;
    case 75:        //���ʹ���
        *x = *x - 1;
        if (*x < 1) *x += 1;
    case 77:        //�����ʹ���
        *x += 1;
        if (*x > x_m) *x -= 1;
    case 80:        //�Ʒ�����
        *y += 1;
        if (*y > y_m) *y -= 1;
        break;
    }
}

//�������� ���� �κ� 
int score;              //���� �ջ��� ���� ����
int combo, max_combo;   //���� �޺��� �ִ� �޺� ���� ����
int offset = 90;        //��Ʈ �������� �ӵ��� ���õ� ������
char g_key[4] = { 'a', 's', 'd', 'f' }; //����ڰ� �Է��� Ű ��
int life = 5;           //��� ���� ����
int coin = 0;           //���� ���� ����
INFO temp;              //��ũ ���� ����
INFO p[arrMAXSIZE];     //��ũ ���� ����

//�����Լ�
int main(void)
{
    //goto�� ������� ���� �������� ���ƿ� �� �ְ� ����.
START:
    //���� ���� �� �ʱ�ȭ
    int x, y, select, page;
    x = 21; y = 12;

    select = draw_title(x, y);      //����ڰ� ������ �޴� �� ����

    for (;;)
    {
        if (select == 0)            //���� ����
        {
            draw_explanation();     //���� ���� ������

            page = draw_select_song(X_MAX, Y_MAX, 67, 4);   //�Լ� ȣ���ϸ鼭 �� ���� �������� �̵�
            if (page == 2) {        //���� ���� 2��� ����ȭ������ ���ư���
                goto START;
            }
            break;
        }
        else if (select == 1)       //���α׷� ����
        {
            return 0;               //�����Լ� ����
            break;
        }
        else if (select == 2)   //��ŷ������
        {
            print_rank(p);
            system("cls");
            goto START;
        }
    }


}

int draw_title(int x, int y)        //�޴� ���� â.
{
    char key;                       //Ű�� ������ ���� ����
    for (;;)
    {
        //������ ���
        printf("��������������������������������������������������������������������������������������������������\n");
        printf("��      ######################################   ��\n");
        printf("��      ##                                  ##   ��\n");
        printf("��      ##     ##  ###                      ##   ��\n");
        printf("��      ##     ## ###    ##   ##    ##      ##   ��\n");
        printf("��      ##     ####       ## ## ## ##       ##   ��\n");
        printf("��      ##     ## ###      ###   ###        ##   ��\n");
        printf("��      ##     ##  ###      #     #         ##   ��\n");
        printf("��    ####     ##   ###                   ####   ��\n");
        printf("��   #####                              ######   ��\n");
        printf("��  #####                              ######    ��\n");
        printf("��������������������������������������������������������������������������������������������������\n");
        printf("                      ����                                           \n");
        printf("                      ����                                           \n");
        printf("                      ��ŷȭ��                                       \n");
        printf("                                                                       \n");
        printf("         �̵� : ��,��,��,��   Ȯ�� : space               \n");
        printf("                                                                       \n");
        printf("                                     ������ : �����C                    \n");
        printf("                                \n");

        for (;;)        //ȭ��ǥ�� ���� ��� �Է¹ޱ� ���� ���� �ݺ���.
        {
            //gotoxy - Ŀ�� �̵�. //\b�� �齺���̽��� ����� ����. //�̵� �� ����� > ���
            gotoxy(x, y); printf("\b\b"); printf(">");

            key = _getch();                     //Ű �Է� ����.
            if (y >= 12 && y <= 14)             //y���� ������ ������ ����.
            {
                if (key == 72 || key == 80)     //�� �Ʒ� ȭ��ǥ���̸�
                {
                    printf("\b\b"); printf("  ");   //����� ���� ���

                    move_cursor(key, &x, &y, X_MAX, Y_MAX);   //key�� ���� y�� ��������

                    if (y > 14) y -= 3;      //���� ����� �ִ� �ּҿ� ���� �� ����.
                    if (y < 12) y += 3;
                }
                else if (key == 32)              //�����̽��� ����
                {
                    if (y == 12)                 //���� ����
                    {
                        system("cls");           //â �ʱ�ȭ
                        return 0;                //�޴� ���� : ���� ����
                    }
                    else if (y == 13)             //����
                    {
                        system("cls");            //â �ʱ�ȭ
                        return 1;                //�޴� ���� : ����
                    }
                    else if (y == 14)   //��ŷ������
                    {
                        system("cls");
                        return 2;                //�޴� ���� : ��ŷ������
                    }
                }
                else
                {
                    continue;
                }
            }
        }
    }
}

void draw_explanation(void) {
    //����ȭ�� ���
    printf("\n\n");
    printf("          [���� ����]                                                  [Ȯ�� : Enter any key]\n\n");
    printf("          ��������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
    printf("          1. Ű������ A,S,D,F�� �������� ��Ʈ�� ���߾� ����Ű�� ������.                           \n");
    printf("\n");
    printf("          2. ����ڰ� ���� Ÿ�ֿ̹� ���� �޺� ��Ī�� �߸� �� �޺����� ������ �����ȴ�.            \n");
    printf("\n");
    printf("      Perfect : 10��                                 \n");
    printf("      Great   : 6��                                 \n");
    printf("      Good    : 2��                                 \n");
    printf("      Bad     : 0��                                 \n");
    printf("\n");
    printf("          3. ü���� �� ������������ �־����� Bad�Ͻ� �ϳ��� ����! ���� �������� Game Over!!!                                   \n");
    printf("                                                                       \n");
    printf("          4. Game Clear��, ���ڿ��� ������ �������� ���� �� �ִ�.                     \n");
    printf("             �� ������ ���� ���ӿ��� Game Over�� �� �� �̾� �� �� �ְ� ���ش�.              \n");
    printf("                                                                       \n");
    printf("          5. Game Over�� ���� �ϳ��� ����Ͽ� �̾��� �� �ִ�!!                  \n");
    printf("             ������ ������ ��ŷ��� ����!!! ��Ŀ�� �Ǿ��!!!\n");
    printf("          ��������������������������������������������������������������������������������������������������������������������������������������������������������������������\n\n");
    _getch();
    system("cls");  //â �ʱ�ȭ
    //������ �����Լ��� ���ư���, ���� ������ ȣ��
}

int draw_select_song(int x_m, int y_m, int x, int y)
{
SELECT_RESTART:     //goto������ ���̵� ���� ��� �� ��� ���ƿ��� ����.
    FMOD_SYSTEM* g_System;                  //���� �ý��� ������ ���ؼ� ���� ����
    FMOD_SOUND* sound[3], * move_sel;       //���� ��ü ����, ����� ���� ������ ������ŭ ����
    FMOD_CHANNEL* g_channel = NULL;         //ä���� ���� ���� ���
    unsigned int      version;

    FMOD_System_Create(&g_System);          //�ý��� ��ü ����
    FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL, NULL);     //�ʱ�ȭ

    FMOD_System_GetVersion(g_System, &version);                 //fmod �����ϱ����ؼ� ������ �´��� Ȯ��
    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 1;
    }
    //���� �ε��Լ��� ���� ������ �о�ͼ� ���� ��ü�� ������Ŵ
    FMOD_System_CreateSound(g_System, "sound/select_sound/D.mp3", FMOD_LOOP_NORMAL, 0, &sound[0]);
    FMOD_System_CreateSound(g_System, "sound/select_sound/H.mp3", FMOD_LOOP_NORMAL, 0, &sound[1]);
    FMOD_System_CreateSound(g_System, "sound/select_sound/R.mp3", FMOD_LOOP_NORMAL, 0, &sound[2]);
    FMOD_System_CreateSound(g_System, "sound/l_r_sound/1.mp3", FMOD_HARDWARE, 0, &move_sel);      // ȭ��ǥ ������ �� ���� ȿ����

    //���� ���� ����.
    char key = 0;                   //Ű�� ����
    char pic[8][8] = { 0, };        //�ִϸ��̼� ���� �迭
    int song = 1, difficulty = 1;   //�뷡�� ���̵� ���� ���� ���� �� �ʱ�ȭ.
    int selected_music = 2, playing_music = 2, sw = 0, first = 1, l_r = 0;  //�뷡 ��� ���� ����
    struct song music[3];           //����ü �迭�� �� 3�� ����.
    int page;                       //������ ��.
    int equalizer_count = 0;        //�ִϸ��̼� ���� ����


    wirte_somnail(music);           //������ �Է�
    srand((unsigned int)time(NULL));//���� ����
    system("cls");                  //ȭ�� �ʱ�ȭ
    //���
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("                                                          ���̵� : "); printf("\n");
    printf("                                                          - 0. ���"); printf("\n");
    printf("                                                          - 1. easy"); printf("\n");
    printf("                                                          - 2. normal"); printf("\n");
    printf("                                                          - 3. hard"); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("                           ���������������������������������������������� "); printf("\n");
    printf("    ��� :                 ��  ��� :             ��      ��� :                   "); printf("\n");//��� : 15ĭ
    printf("    ��Ƽ��Ʈ :             ��  ��Ƽ��Ʈ :         ��      ��Ƽ��Ʈ :               "); printf("\n");//��Ƽ��Ʈ : 11ĭ
    printf("                           ���������������������������������������������� "); printf("\n");
    printf("     "); printf("\n");
    printf("   ----------------------------------------------------------------------\n");
    printf("   �� �̵� : ��,��    ���� : space    ������ : q\n");
    printf("   ----------------------------------------------------------------------\n");
    printf("   TIP! : �� ���� ���� ��, ���̵� ����. ");

    Sleep(800);         //0.8�ʰ� ���α׷� ����

    for (;;)            //ȭ��ǥ�� ��� �ޱ� ���� ���ѹݺ���.
    {
        if (first == 1) //ù �����̶��, ���� ���
        {
            FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, sound[1], 0, &g_channel);
            first = 0;
        }

        if (sw == 1)     //�⺻���� 0�̹Ƿ� ó������ ����ȵǰ� ȭ��ǥ �¿� ������ ���� ����Ǹ鼭 ó���� ����� ���� Ǯ����.
        {
            if (l_r == 1)                       //l_r �⺻�� 0��, ȭ��ǥ �¿쿡 ���� �� �����. �� ���� ȭ��ǥ ������ ������ ��
            {
                switch (playing_music)          //���� ����ǰ� �ִ� �뷡 ���� �°� �뷡 ����
                {
                case 1:
                    FMOD_Sound_Release(sound[0]);
                    break;
                case 2:
                    FMOD_Sound_Release(sound[1]);
                    break;
                case 3:
                    FMOD_Sound_Release(sound[2]);
                    break;
                }
                playing_music--;                //���� �� ȭ��ǥ �¿쿡 ���� �뷡 �� �����.
                if (playing_music < 1)          //���� �Ѿ�� �ʰ� ����
                    playing_music = 3;
            }
            else if (l_r == 2)                  //�� ���� ȭ��ǥ ���� ������ ��
            {
                switch (playing_music)          //���� ����ǰ� �ִ� �뷡 ���� �°� �뷡 ����
                {
                case 1:
                    FMOD_Sound_Release(sound[0]);
                    break;
                case 2:
                    FMOD_Sound_Release(sound[1]);
                    break;
                case 3:
                    FMOD_Sound_Release(sound[2]);
                    break;

                }
                playing_music++;                //���� �� ȭ��ǥ �¿쿡 ���� �뷡 �� �����.
                if (playing_music > 3)          //���� �Ѿ�� �ʰ� ����
                    playing_music = 1;
            }

        }
        if (sw == 1)//���� ��ȯ�ϱ����� ����ġ
        {
            if (selected_music == 1)
            {
                FMOD_System_CreateSound(g_System, "sound/select_sound/D.mp3", FMOD_LOOP_NORMAL, 0, &sound[0]);
                FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, sound[0], 0, &g_channel);        //���� ���

                sw = 0;
            }
            else if (selected_music == 2)
            {
                FMOD_System_CreateSound(g_System, "sound/select_sound/H.mp3", FMOD_LOOP_NORMAL, 0, &sound[1]);
                FMOD_Channel_Stop(g_channel);
                FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, sound[1], 0, &g_channel);        //���� ���

                sw = 0;
            }
            else if (selected_music == 3)
            {
                FMOD_System_CreateSound(g_System, "sound/select_sound/R.mp3", FMOD_LOOP_NORMAL, 0, &sound[2]);
                FMOD_Channel_Stop(g_channel);
                FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, sound[2], 0, &g_channel);        //���� ���

                sw = 0;
            }
        }
        for (;;)
        {
            Sleep(100);             //0.1�ʰ� ����

            if (_kbhit() == 1)      //getch�� �Է¹����� Ű �Է±��� ���Ѵ���ϹǷ� kbhit�� ����Ͽ� �Ʒ� �ڵ� ����
            {                       //kbhit�� Ű�� �Է��� ������ 1 ��ȯ ������ �����ϰ� �������� ����.
                key = _getch();     //Ű �Է�
            }
            draw_equalizer(&equalizer_count, pic);       //�ִϸ��̼� ȣ��.
            draw_song(&selected_music, key, music);      //ȭ�鿡 �� ���� ���
            if (key == 'q')      //���θ޴��� ���ư���
            {
                switch (playing_music)      //���� ������ ����
                {
                case 1:
                    FMOD_Sound_Release(sound[0]);       //����
                    break;
                case 2:
                    FMOD_Sound_Release(sound[1]);       //����
                    break;
                case 3:
                    FMOD_Sound_Release(sound[2]);       //����
                    break;
                }
                system("cls");                          //â�ʱ�ȭ
                return 2;                               //2 ���� -> ����ȭ�� �̵�
            }

            else if (key == 32)      //32 - �����̽� Ȯ�� ��, ���̵� ����
            {
                gotoxy(x, y);   scanf_s("%d", &difficulty);   //�̵� �� �� �Է¹ޱ�.
                if (difficulty == 0)
                {
                    //� ���� �뷡 ����.
                    switch (playing_music)
                    {
                    case 1:
                        FMOD_Sound_Release(sound[0]);
                        break;
                    case 2:
                        FMOD_Sound_Release(sound[1]);
                        break;
                    case 3:
                        FMOD_Sound_Release(sound[2]);
                        break;
                    }
                    goto SELECT_RESTART;        //���ư��� �缱��
                }
                else if (difficulty >= 1 || difficulty <= 3)
                {
                    //� ���� �뷡 ����.
                    switch (playing_music)
                    {
                    case 1:
                        FMOD_Sound_Release(sound[0]);
                        break;
                    case 2:
                        FMOD_Sound_Release(sound[1]);
                        break;
                    case 3:
                        FMOD_Sound_Release(sound[2]);
                        break;
                    }
                    system("cls");          //â �ʱ�ȭ
                    page = draw_game_screen(playing_music, difficulty); //���� ��ũ�� ���� �� ���� �� ����
                    if (page == 2)          //���� �� 2 ��� : ���� Ŭ���� or ���� ���� ����
                    {
                        system("cls");      //â �ʱ�ȭ
                        return page;        //page : 2 ���� -> ����ȭ������ �̵�
                    }
                    game_over();            //�� ���� ��(���� Ż��)�̸� ���ӿ��� ���

                    system("cls");          //â �ʱ�ȭ
                    return 2;               //2 ���� -> ����ȭ������ �̵�
                }

            }
            else if (key == 75 || key == 77)   //ȭ��ǥ �¿�, �� �̵��ϸ鼭 �� ����.
            {
                move_draw_song(key, music);    //�� ����
                FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, move_sel, 0, &g_channel);
                Sleep(100);                    //0.1�� ����
                if (key == 77)                 //�������̸�
                {
                    //�� ���� �°� ����
                    selected_music--;
                    if (selected_music < 1) //���� �Ѿ�� �ʰ� ����
                        selected_music = 3;
                    l_r = 1;
                    sw = 1;
                    key = 0;
                    break;
                }
                else if (key == 75)            //�����̸�
                {
                    //�� ���� �°� ����
                    selected_music++;
                    if (selected_music > 3) //���� �Ѿ�� �ʰ� ����
                        selected_music = 1;
                    l_r = 2;
                    sw = 1;
                    key = 0;
                    break;
                }

            }
        }
    }
}

void draw_equalizer(int* equalizer_count, char pic[][8])      //���� ���� �迭�� ����(3�Ǹ� �ʱ�ȭ) ��, �� ���� ���� ���.
{
    //���� ���� �� �ʱ�ȭ
    int x = 30, y = 5, i, j, k;

    //ī��Ʈ�� 3�̸�, -> ī��Ʈ�� ���� ������������ �����ϴ� ���� �ڿ������� �ϱ� ����.
    if (*equalizer_count == 3)
    {
        //3�̸� ������ �ʱ�ȭ
        for (i = 0; i < 8; i++)
        {
            k = rand() % 5;
            pic[i][k] = 1;
        }
        //�������� ���� ������ ä���
        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 7; j++)
            {
                if (pic[i][j] == 1)
                {
                    pic[i][j + 1] = 1;
                }
            }
        }
        //�ʱ�ȭ
        *equalizer_count = 0;
    }
    //���� ��ĭ�� �̵���, �ڿ������� ���̱�.
    for (i = 0; i < 8; i++)
    {
        for (j = 7; j > 0; j--)
        {
            pic[i][j] = pic[i][j - 1];
            pic[i][j - 1] = 0;
        }
    }

    //��� : ������ �Ųٷ� �߱⿡ pic[j][i]�� ������ ���
    for (i = 0; i < 8; i++)
    {
        gotoxy(x, y);
        for (j = 0; j < 8; j++)
        {
            if (pic[j][i] == 1)
                printf("��");
            else if (pic[j][i] == 0)
                printf("  ");
        }
        y += 1;
    }
    //ī��Ʈ ����
    *equalizer_count = *equalizer_count + 1;

    return;


}

void wirte_somnail(struct song* sn)      //�� ���� ����
{

    strcpy_s(sn->songname, 20, "Ref");
    strcpy_s(sn->artist, 20, "Vk");

    strcpy_s((sn + 1)->songname, 20, "HSD");
    strcpy_s((sn + 1)->artist, 20, "ND");

    strcpy_s((sn + 2)->songname, 20, "Dgg");
    strcpy_s((sn + 2)->artist, 20, "Mm");

}

void draw_song(int* sel, char key, struct song* sn)      //������ ���
{
    int  y = 16;
    gotoxy(11, y);                  //gotoxy�� �̵� �� ���� ���
    printf("%s", sn->songname);
    gotoxy(15, y + 1);
    printf("%s", sn->artist);
    gotoxy(37, y);
    printf("%s", (sn + 1)->songname);
    gotoxy(41, y + 1);
    printf("%s", (sn + 1)->artist);
    gotoxy(63, y);
    printf("%s", (sn + 2)->songname);
    gotoxy(67, y + 1);
    printf("%s", (sn + 2)->artist);
}

void move_draw_song(char key, struct song* sn)
{
    //����ü �� ���� ����
    struct song temp;
    int i;
    switch (key)//Ű(ȭ��ǥ)�� ����
    {
        //temp����ü�� �� ��ȯ
        //temp = a
        //a = b
        //b = temp
    case 77:
        // _TRUNCATE�� ���� unsigned int�� �ִ밪 �� �� 42��
        // strcpy_s�� strcpyó�� �ٲ�(ù��° ���� ���۴� ��ȣ���� ����)
        strcpy_s(temp.songname, _TRUNCATE, (sn)->songname);
        strcpy_s(temp.artist, _TRUNCATE, (sn)->artist);
        for (i = 0; i < 2; i++)
        {
            strcpy_s((sn + i)->songname, _TRUNCATE, (sn + i + 1)->songname);
            strcpy_s((sn + i)->artist, _TRUNCATE, (sn + i + 1)->artist);
        }
        strcpy_s((sn + 2)->artist, _TRUNCATE, temp.artist);
        strcpy_s((sn + 2)->songname, _TRUNCATE, temp.songname);
        break;
    case 75:
        strcpy_s(temp.songname, _TRUNCATE, (sn + 2)->songname);
        strcpy_s(temp.artist, _TRUNCATE, (sn + 2)->artist);
        for (i = 2; i > 0; i--)
        {
            strcpy_s((sn + i)->songname, _TRUNCATE, (sn + i - 1)->songname);
            strcpy_s((sn + i)->artist, _TRUNCATE, (sn + i - 1)->artist);
        }
        strcpy_s((sn)->artist, _TRUNCATE, temp.artist);
        strcpy_s((sn)->songname, _TRUNCATE, temp.songname);
        break;
    }
}


int draw_game_screen(int song, int diff)
{
    //���� ��� ���� ���� ����
    bool coin_used = 0;
    //������ �ִٸ�
    if (coin > 0)
    {
        //��� ���� ���� �� â �ʱ�ȭ
        coin_used = qus_use_coin();
        system("cls");
    }
    FMOD_SYSTEM* g_System;              //���� �ý��� ������ ���ؼ� ���� ����
    FMOD_SOUND* sound[3];               //���� ��ü ����, ����� ���� ������ ������ŭ �迭�� ����
    FMOD_CHANNEL* g_channel = NULL;     //ä���� ���� ���� ���
    unsigned int      version;

    FMOD_System_Create(&g_System);      //�ý��� ��ü ����
    FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL, NULL);     //�ʱ�ȭ

    FMOD_System_GetVersion(g_System, &version);         //fmod �����ϱ����ؼ� ������ �´��� Ȯ��
    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        exit(1);
    }

    //���� �ε� �Լ��� ���� ������ �о�ͼ� ���� ��ü�� ������Ŵ
    FMOD_System_CreateSound(g_System, "sound/game_sound/Dingga.mp3", FMOD_DEFAULT, 0, &sound[0]);
    FMOD_System_CreateSound(g_System, "sound/game_sound/High_School_Dropout.mp3", FMOD_DEFAULT, 0, &sound[1]);
    FMOD_System_CreateSound(g_System, "sound/game_sound/Reflection.mp3", FMOD_DEFAULT, 0, &sound[2]);

    //���� ����
    //---------------------------------------------------------------
    int x = 10, y = 0;
    int i, j;
    int playlen = 0;
    char key = 0;
    char file_note[20000][5];
    int note[4][24] = { 0, 0, };
    int n_of_file = 0, num = 0;
    int judge = 0;
    int play_count = 0;
    //----------------------------------------------------------------

    //�ʱⰪ ����
    //----------------------------------------------------------------
    score = 0;      //���� �ʱ�ȭ
    combo = 0;      //�޺� �ʱ�ȭ
    life = 5;       //��� �ʱ�ȭ
    max_combo = 0;  //�ִ� �޺� �ʱ�ȭ
    //-------------------------------------------------------------------

    //�ؽ�Ʈ ���� �ҷ�����
    //------------------------------------------------------------------
    FILE* notefile = NULL;
    if (song == 1)
    {
        switch (diff) {
        case 1:fopen_s(&notefile, "sound/songs/D_easy.txt", "r");   break;
        case 2:fopen_s(&notefile, "sound/songs/D_normal.txt", "r");   break;
        case 3:fopen_s(&notefile, "sound/songs/D_hard.txt", "r");   break;
        }
    }
    else if (song == 2)
    {
        switch (diff) {
        case 1:fopen_s(&notefile, "sound/songs/H_easy.txt", "r");   break;
        case 2:fopen_s(&notefile, "sound/songs/H_normal.txt", "r");   break;
        case 3:fopen_s(&notefile, "sound/songs/H_hard.txt", "r");   break;
        }
    }
    else if (song == 3)
    {
        switch (diff) {
        case 1:fopen_s(&notefile, "sound/songs/R_easy.txt", "r");   break;
        case 2:fopen_s(&notefile, "sound/songs/R_normal.txt", "r");   break;
        case 3:fopen_s(&notefile, "sound/songs/R_hard.txt", "r");   break;
        }
    }
    if (notefile == NULL)
        printf("���� �ҷ����� ����\n");
    //���� ���� ����
    //**********************************
    fseek(notefile, 0L, SEEK_END);
    n_of_file = ftell(notefile);
    n_of_file = (n_of_file / 6) + 1;

    //********************************
    //������ ������ ��, �м��Ͽ� �迭�� ����
    //**********************************************

    fseek(notefile, 0, SEEK_SET);

    for (i = 0; i < n_of_file; i++)
    {
        for (j = 0; j < 5; j++)
        {
            file_note[i][j] = fgetc(notefile);
            fseek(notefile, 0L, SEEK_CUR);
        }
    }
    fclose(notefile);
    //**********************************************
    //---------------------------------------------------------------------

    //���� â ���
    for (i = 0; i < 21; i++)
    {
        gotoxy(x, y);
        for (j = 0; j < 4; j++)
        {
            printf("��       ");
        }
        printf("��");
        y += 1;
    }
    gotoxy(x, y);
    printf("������������������������������������������������������������������");
    y += 1;
    gotoxy(x, y);
    for (j = 0; j < 5; j++)
    {
        printf("��       ");
    }
    y += 1;
    gotoxy(x, y);
    printf("������������������������������������������������������������������");
    gotoxy(60, 2);
    printf("q:�����޴�");

    for (playlen = 0; playlen < n_of_file + 22; playlen++)
    {
        if (note[3][20] == 1 || note[2][20] == 1 || note[1][20] == 1 || note[0][20] == 1 || score != 0)
        {//�� Ű���� ��Ʈ�� y��ǥ 20�� ���� �������� �����ϸ� �뷡����
            if (play_count == 0)        //����� Ű �Է��� ���� �ݺ��� �ʿ��ѵ� ���嵵 �ݺ� ��µǴ°��� ���� ���� play_count ���� ���.
            {
                if (song == 1)          //���޹��� �� ���� ���� �˸��� ���� ���
                {
                    FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, sound[0], 0, &g_channel);
                    play_count++;
                }
                else if (song == 2)
                {
                    FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, sound[1], 0, &g_channel);
                    play_count++;
                }
                else if (song == 3)
                {
                    FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, sound[2], 0, &g_channel);
                    play_count++;
                }
            }
        }
        //Ű �Է� �ޱ�
        if (_kbhit())
            key = _getch();
        //q�̸�
        if (key == 'q')
        {
            //�뷡 ���� ��
            if (song == 1)
            {
                FMOD_Sound_Release(sound[0]);
            }
            else if (song == 2)
            {
                FMOD_Sound_Release(sound[1]);
            }
            else if (song == 3)
            {
                FMOD_Sound_Release(sound[2]);
            }
            //���� Ż��
            game_stop();
            //2 �������� ����ȭ������ �̵�
            return 2;
        }
        if (key == 'a' || key == 's' || key == 'd' || key == 'f')   //asdf�Է½�
        {
            draw_key_effect(&key); //����ȭ�� - Ű�� ���� ��(asdf) : ���ᰡ ������ �ϴ� �Լ�
            judge = judge_acc(&key, note); //����ȭ�� - ��Ʈ ���� �Լ� : great, perfect...
        }
        //judge_acc���� ���������� ó���ϱ⿡ bad�� ó������ �ʾ� ���� ����
        if (note[0][23] == 1 || note[1][23] == 1 || note[2][23] == 1 || note[3][23] == 1)
            judge = 4; //bad

        if (judge == 2)     //perfect
        {
            if (coin_used)  //���� ���� ���� �ι�
                score += 20;
            else
                score += 10;
            combo++;
            if (max_combo < combo)//�޺��� �ƽ��޺��� ������ ����
                max_combo = combo;
        }
        else if (judge == 1) //great
        {
            if (coin_used)   //���� ���� ���� �ι�
                score += 12;
            else
                score += 6;
            combo++;
            if (max_combo < combo)//�޺��� �ƽ��޺��� ������ ����
                max_combo = combo;
        }
        else if (judge == 3) //good
        {
            if (coin_used)   //���� ���� ���� �ι�
                score += 4;
            else
                score += 2;
            combo++;
            if (max_combo < combo)//�޺��� �ƽ��޺��� ������ ����
                max_combo = combo;
        }
        else if (judge == 4)//bad
        {
            combo = 0; //bad �� �� combo�� �ʱ�ȭ.
            life -= 1; //bad�� �� ü�°���.
        }
        if (life == 0)      //������ ��� ������
        {   //�뷡 ���� ��,
            if (song == 1)
            {
                FMOD_Sound_Release(sound[0]);
            }
            else if (song == 2)
            {
                FMOD_Sound_Release(sound[1]);
            }
            else if (song == 3)
            {
                FMOD_Sound_Release(sound[2]);
            }
            //0 ����
            return 0;
        }
        draw_score_combo_life_coin();   //�޺�, ����, ���� �� ���
        draw_judge(&judge);             //�Ǵܰ� ���

        note_to_note(&num, n_of_file, file_note, note); //����ȭ��-�ؽ�Ʈ�� �ҷ��� ��Ʈ�� �������� �̽�
        draw_note(note);                //����ȭ��-��Ʈ �׸���
        Sleep(offset);                  //��� ���� -> ��Ʈ �������� �ӵ� ����

        for (i = 0; i < 4; i++)         //�迭�� �ִ� ��Ʈ �� ������
        {
            for (j = 23; j > 0; j--)
            {
                note[i][j] = note[i][j - 1];
                note[i][j - 1] = 0;
            }
        }

        note[i][20] = 0;                //note[4][20] = 0 
        delete_key(&key);               //����ȭ�� - ������Է� ����ǥ�� �� �ʱ�ȭ
        erease_judge();                 //����ȭ�� - ����Ʈ(�Ǵܰ�)�� ����
    }
    //��Ʈ ������ �뷡 ��
    if (song == 1)
    {
        FMOD_Sound_Release(sound[0]);
    }
    else if (song == 2)
    {
        FMOD_Sound_Release(sound[1]);
    }
    else if (song == 3)
    {
        FMOD_Sound_Release(sound[2]);
    }
    //���� Ŭ����
    game_clear();   //Ŭ���� â ���
    system("cls");  //â �ʱ�ȭ
    random_coin();  //�������λ����Լ�
    rank(temp, score);  //�̸� �Է¹޾Ƽ� �������Ͽ� ����
    system("cls");  //â �ʱ�ȭ
    print_rank(p);  //��ŷ ���

    return 2;       //2 �����Ͽ� ����ȭ������ �̵�
}

void note_to_note(int* num, int notelen, char f_note[][5], int note[4][24])
//����ȭ��-�ؽ�Ʈ�� �ҷ��� ��Ʈ�� �������� �̽�
{
    int j;
    for (j = 0; j < 4; j++)
    {
        if (f_note[(*num)][j] == '0')
            note[j][0] = 0;
        else if (f_note[(*num)][j] == '1')
            note[j][0] = 1;
    }
    *num += 1;
}

void draw_note(int vac[4][24])  //��Ʈ ���
{
    //���� ���� ���� �� �ʱ�ȭ
    int x = 12, y = 0;
    int i, j;
    //x, y�� �°� �̵�
    gotoxy(x, y);
    for (i = 0; i < 21; i++)    //���� �ݺ� - ��ĭ�� ��������
    {

        for (j = 0; j < 4; j++) //���� �ݺ� - ��ĭ�� ���������� ����
        {
            if (i == 20 || i == 19 || i == 18 || i == 17)
            {
                gotoxy(x, i);
                printf("	  ");          //��ĭ ������� �����
            }
            gotoxy(x, y);
            if (vac[j][i] == 1)            //1�̸� ��Ʈ ���
            {
                printf("����");

                if (y > 0)                 //y ����� 
                {
                    gotoxy(x, y - 1);      //��ĭ �ö󰡼�(���� ��µ� ��)
                    printf("	  ");      //�����
                }
            }
            else if (vac[j][i] == 0)       //0�̸� ��ĭ ���
            {
                printf("      ");
            }
            x += 8;     //��ǥ ���� ���������� �̵�
        }
        x = 12;         //�ٽ� asdf���� aŰ�� �°� �ʱ�ȭ
        y += 1;         //���� ������
    }
}




void draw_key_effect(char* key) // ����� Ű���� �Է� ��, �ǹؿ� ���� ���.(�Է��ߴٴ� ǥ��)
{
    if (*key == g_key[0])       //g_key[0] = Ű���� a
    {
        gotoxy(12, 22);
        printf("����");
    }
    else if (*key == g_key[1])  //g_key[1] = Ű���� s
    {
        gotoxy(20, 22);
        printf("����");
    }
    else if (*key == g_key[2])  //g_key[2] = Ű���� d
    {
        gotoxy(28, 22);
        printf("����");
    }
    else if (*key == g_key[3])  //g_key[3] = Ű���� f
    {
        gotoxy(36, 22);
        printf("����");
    }
}

void delete_key(char* key) //����ȭ�� - ������Է� ����ǥ�� �� �ʱ�ȭ
{
    if (*key == g_key[0])       //g_key[1] = Ű���� a
    {
        gotoxy(12, 22);
        printf("      ");       //��ĭ���� ���ֱ�
    }
    else if (*key == g_key[1])  //g_key[1] = Ű���� s
    {
        gotoxy(20, 22);
        printf("      ");       //��ĭ���� ���ֱ�
    }
    else if (*key == g_key[2])  //g_key[1] = Ű���� d
    {
        gotoxy(28, 22);
        printf("      ");       //��ĭ���� ���ֱ�
    }
    else if (*key == g_key[3])  //g_key[1] = Ű���� f
    {
        gotoxy(36, 22);
        printf("      ");       //��ĭ���� ���ֱ�
    }
    *key = 0;                   //Ű �ʱ�ȭ
}


int judge_acc(char* key, int note[4][24]) //����ȭ��-��Ȯ�� �Ǵ� �Լ� �� �Լ��� ��ȯ���� judge�� ����
{

    if (*key == g_key[0]) //a �Է½� 
    {
        if (note[0][20] == 1 || note[0][21] == 1) // ��ƮŸ�ְ̹� �������� +-1 
        {
            if (note[0][20] == 1)      note[0][20] = 0;     //0���� �ٲٴ� ������ Ű�� ���̻� �ΰ��ӿ��� ���̸� �ȵǱ� ����.
            else if (note[0][21] == 1) note[0][21] = 0;
            return 2; //"perfect"
        }

        else if (note[0][18] == 1 || note[0][19] == 1)  // ��ƮŸ�ְ̹� �������� +-2
        {
            if (note[0][18] == 1)       note[0][18] = 0;
            else if (note[0][19] == 1)	note[0][19] = 0;
            return 1; //"great"
        }

        else if (note[0][16] == 1 || note[0][17] == 1)  // ��ƮŸ�ְ̹� �������� +-3
        {
            if (note[0][16] == 1)        note[0][16] = 0;
            else if (note[0][17] == 1) 	 note[0][17] = 0;
            return 3; //"good"
        }

    }

    if (*key == g_key[1])                               //s �Է½�
    {
        if (note[1][20] == 1 || note[1][21] == 1)       // ��ƮŸ�ְ̹� �������� +-1 
        {
            if (note[1][20] == 1)      note[1][20] = 0;
            else if (note[1][21] == 1) note[1][21] = 0;
            return 2;                                   //"perfect"
        }

        else if (note[1][18] == 1 || note[1][19] == 1)  // ��ƮŸ�ְ̹� �������� +-2
        {
            if (note[1][18] == 1)       note[1][18] = 0;
            else if (note[1][19] == 1)	note[1][19] = 0;
            return 1;                                   //"great"
        }

        else if (note[1][16] == 1 || note[1][17] == 1)  // ��ƮŸ�ְ̹� �������� +-3
        {
            if (note[1][16] == 1)        note[1][16] = 0;
            else if (note[1][17] == 1) 	 note[1][17] = 0;
            return 3;                                   //"good"
        }

    }

    if (*key == g_key[2])                               //d �Է½�
    {
        if (note[2][20] == 1 || note[2][21] == 1)       // ��ƮŸ�ְ̹� �������� +-1 
        {
            if (note[2][20] == 1)      note[2][20] = 0;
            else if (note[2][21] == 1) note[2][21] = 0;
            return 2;                                   //"perfect"
        }

        else if (note[2][18] == 1 || note[2][19] == 1)  // ��ƮŸ�ְ̹� �������� +-2
        {
            if (note[2][18] == 1)       note[2][18] = 0;
            else if (note[2][19] == 1)	note[2][19] = 0;
            return 1;                                   //"great"
        }

        else if (note[2][16] == 1 || note[2][17] == 1)  // ��ƮŸ�ְ̹� �������� +-3
        {
            if (note[2][16] == 1)        note[2][16] = 0;
            else if (note[2][17] == 1) 	 note[2][17] = 0;
            return 3;                                   //"good"
        }

    }


    if (*key == g_key[3])                               //f �Է½�
    {
        if (note[3][20] == 1 || note[3][21] == 1)       // ��ƮŸ�ְ̹� �������� +-1 
        {
            if (note[3][20] == 1)      note[3][20] = 0;
            else if (note[3][21] == 1) note[3][21] = 0;
            return 2;                                   //"perfect"
        }

        else if (note[3][18] == 1 || note[3][19] == 1)  // ��ƮŸ�ְ̹� �������� +-2
        {
            if (note[3][18] == 1)       note[3][18] = 0;
            else if (note[3][19] == 1)	note[3][19] = 0;
            return 1;                                   //"great"
        }

        else if (note[3][16] == 1 || note[3][17] == 1)  // ��ƮŸ�ְ̹� �������� +-3
        {
            if (note[3][16] == 1)        note[3][16] = 0;
            else if (note[3][17] == 1) 	 note[3][17] = 0;
            return 3;                                   //"good"
        }

    }

    return 0;
}

void draw_judge(int* jud)
{
    int x = 45, y = 15;
    //�̵��ؼ�
    gotoxy(x, y);
    //�Ǵܰ��� ���� ���
    if (*jud == 1)
        printf("GOOD!");
    else if (*jud == 2)
        printf("PERFECT!");
    else if (*jud == 3)
        printf("GREAT!");
    else if (*jud == 4)
        printf("Bad");
    else
        return;

    *jud = 0;
}

void erease_judge() // ����ȭ�� - ����Ʈ�� ����
{
    int x = 45, y = 15;
    //�̵��ؼ�
    gotoxy(x, y);
    //��ĭ���� �����
    printf("              ");
}

void draw_score_combo_life_coin() //������ �޺� ����Լ�
{
    //�̵��ؼ� ���
    gotoxy(45, 9);
    printf("���� : ");
    gotoxy(52, 9);
    printf("%d", score);
    gotoxy(45, 10);
    printf("COMBO : ");
    gotoxy(52, 10);
    printf("%3d", combo);
    gotoxy(45, 11);
    printf("ü�� : ");
    gotoxy(52, 11);
    if (life == 5) printf("����������");
    if (life == 4) printf("����������");
    if (life == 3) printf("����������");
    if (life == 2) printf("����������");
    if (life == 1) printf("����������");
    if (life == 0) printf("����������");
    gotoxy(45, 12);
    printf("���� ����");
    gotoxy(55, 12);
    printf("%d��", coin);
    gotoxy(45, 13);
    printf("������ : q");

}

bool qus_use_coin()     //���� ��� ���� ����
{

re:
    //���
    system("cls");
    printf("��������������������������������������������������������������������������������������������������\n");
    printf("��                                               ��\n");
    printf("��                ������ ����Ͽ�                ��\n");
    printf("��                                               ��\n");
    printf("��           ������ 2�� �Ͻðڽ��ϱ� ? (Y/N)     ��\n");
    printf("��                    :                          ��\n");
    printf("��                 ���� ���� : %d ��              ��\n", coin);
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��������������������������������������������������������������������������������������������������\n");
    char answer;
    gotoxy(23, 5);
    //�̵��ؼ�
    scanf_s(" %c", &answer, sizeof(answer));    //�� �Է¹ޱ�, " %c"�� ���� ���� �Է�

    if (answer == 'Y' || answer == 'y')     //����
    {
        coin -= 1;          //���� ���� ����
        return 1;           //1����
    }
    else if (answer == 'N' || answer == 'n')//�����
    {
        return 0;           //0����
    }

    else
    {
        //�߸��� Ű �Է½� goto�� �̵�
        gotoxy(15, 7);
        printf("�߸��Է��ϼ̽��ϴ�.");
        gotoxy(15, 8);
        printf("�ƹ�Ű�� �����ּ���");
        //�ƹ� Ű �Է� ���
        _getch();
        goto re;
    }
}

void random_coin()      //���� �ڽ�
{
    system("cls");
    printf("��������������������������������������������������������������������������������������������������\n");
    printf("��                                               ��\n");
    printf("��          ���� ������ ����ּ��� :             ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��������������������������������������������������������������������������������������������������\n");

    gotoxy(15, 5);
    printf("1\t2\t3\n");

    int box[3] = { 0,0,1 }; //���Ƿ� ����. �迭���� 1�� ���ڿ� ���� 1�� ����ִ� ������ ����, 
                            //�������� 0���� ����.

    //shuffle - �迭�� �ε��� ������ �������� ������ 0 ,0, 1 ���� �������� ����.
    srand((unsigned int)time(NULL));            //���� ����
    for (int i = 2; i > 0; i--)
    {
        int j = rand() % (i + 1);               //���� ����
        int temp = 0;
        temp = box[i];                          //�ڽ� ��ȯ
        box[i] = box[j];
        box[j] = temp;
    }

    int choice;
    gotoxy(23, 7);              //�̵� ��, ���� ���� �� �Է�
    scanf_s("%d%*c", &choice);

    if (box[choice] == 1)       //���� 1�̸�
    {
        gotoxy(15, 9);
        printf("���� 1���� �����̽��ϴ�.\n");
        coin += 1;              //���� ����
    }
    else
    {
        gotoxy(23, 9);
        printf("��");
    }
    //�̵� �� ���
    gotoxy(15, 11);
    printf("���� ���� ���� : %d��\n", coin);
    gotoxy(0, 2);
}

void game_clear()
{
    system("cls");          //â �ʱ�ȭ
    //���
    printf("��������������������������������������������������������������������������������������������������\n");
    printf("��                                               ��\n");
    printf("��               GAME      CLEAR!!!              ��\n");
    printf("��                                               ��\n");
    printf("��               ���� ����  : %4d               ��\n", score);
    printf("��                                               ��\n");
    printf("��             �ִ� �޺� �� : %3d                ��\n", max_combo);
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��     �ƹ� Ű�� ������ ���� ���� �ڽ��� �̵�    ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��������������������������������������������������������������������������������������������������\n");

    _getch();       //�ƹ�Ű �Է� ���
    return;
}



void game_stop()
{
    system("cls");          //â �ʱ�ȭ
    //���
    printf("��������������������������������������������������������������������������������������������������\n");
    printf("��                                               ��\n");
    printf("��               GAME      STOP!!!               ��\n");
    printf("��                                               ��\n");
    printf("��               ���� ����  : %4d               ��\n", score);
    printf("��                                               ��\n");
    printf("��             �ִ� �޺� �� : %3d                ��\n", max_combo);
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��     �ƹ� Ű�� ������ ���� ȭ������ �̵�       ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��������������������������������������������������������������������������������������������������\n");

    _getch();           //�ƹ�Ű �Է� ���
    return;
}

void game_over()
{
    system("cls");      //â �ʱ�ȭ
    //���
    printf("��������������������������������������������������������������������������������������������������\n");
    printf("��                                               ��\n");
    printf("��               GAME      OVER!!!               ��\n");
    printf("��                                               ��\n");
    printf("��               ���� ����  : %4d               ��\n", score);
    printf("��                                               ��\n");
    printf("��             �ִ� �޺� �� : %3d                ��\n", max_combo);
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��     �ƹ� Ű�� ������ ���� ȭ������ �̵�       ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��                                               ��\n");
    printf("��������������������������������������������������������������������������������������������������\n");

    _getch();       //�ƹ� Ű �Է� ���
    return;
}


void rank(INFO temp, int score) {

    FILE* fp = NULL;
    fopen_s(&fp, "rank.bin", "ab");
    if (fp == NULL)		exit(1);

    // ���� Ŭ���� ������ �̸� �Է¹ް� ���� ���޹޾Ƽ� �������Ͽ� �߰��ϸ� ����
    gotoxy(13, 13);
    printf("�̸��� �Է��ϼ��� : ");
    gets_s(temp.name, sizeof(temp.name));
    temp.score = score;
    temp.rank = 0;

    fwrite(&temp, sizeof(INFO), 1, fp);
    fclose(fp);
}

void print_rank(INFO p[arrMAXSIZE]) {
    int	state;
    int temp_rank, temp_score;
    char	temp_name[24];
    FILE* fpin = NULL;
    FILE* fpout = NULL;

    fopen_s(&fpin, "rank.bin", "rb");
    fopen_s(&fpout, "print_rank.bin", "wb+");

    //���α׷� �����ߴٰ� �ٽ� ��ŷȭ�� �����ص� ������ ������ �����ְ� ������
    //�������Ͽ� ����� ����� ����ü �迭�� ����
    state = fread(p, sizeof(INFO), arrMAXSIZE, fpin);
    if (state == 0)	exit(1);
    else {
        for (int i = 0; i < arrMAXSIZE; i++) {
            if (p[i].score != 0)
                p[i].rank = 1;
            for (int j = 0; j < arrMAXSIZE; j++) {
                if ((p[i].score != 0) && (p[i].score < p[j].score))
                    p[i].rank++;
            }
        }
    }

    //���� 1������ �����ϰ� �������Ͽ� ����
    for (int i = 0; p[i].rank != 0; i++) {
        for (int j = i + 1; p[j].rank != 0; j++) {
            if (p[i].rank > p[j].rank) {
                temp_rank = p[i].rank;
                p[i].rank = p[j].rank;
                p[j].rank = temp_rank;
                temp_score = p[i].score;
                p[i].score = p[j].score;
                p[j].score = temp_score;
                strcpy_s(temp_name, sizeof(temp_name), p[i].name);
                strcpy_s(p[i].name, sizeof(p[i].name), p[j].name);
                strcpy_s(p[j].name, sizeof(p[j].name), temp_name);
            }
        }
    }
    fwrite(p, sizeof(INFO), arrMAXSIZE, fpout);

    //���������� ó������ ���ư�.
    fseek(fpout, 0, SEEK_SET);

    //�������� �о 1������ 10������ ������� ���
    state = fread(p, sizeof(INFO), arrMAXSIZE, fpout);
    if (state == 0)	exit(1);
    else {
        for (int i = 0; i < MAX_RANK; i++) {
            if (p[i].rank != 0)
                printf("%2d : %s %d\n", i + 1, p[i].name, p[i].score);
            else
                printf("%2d : ______    0 \n", i + 1);
        }
    }

    fclose(fpin);
    fclose(fpout);
    printf("�ƹ� Ű�� ������ �޴� ȭ������ �̵�");
    _getch();               //�ƹ� Ű �Է� ���
}