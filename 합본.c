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

//구조체 선언 부분 
struct song
{
    char songname[20];      //곡명
    char artist[20];        //작곡가명
};

typedef struct _info {
    char	name[24];       //이름
    int		score;          //점수
    int		rank;           //순위
}INFO;

int  draw_title(int x, int y);					                              //타이틀화면
void draw_explanation(void);					                              //설명 화면
void draw_equalizer(int* equalizer_count, char pic[][8]);                     // 곡 선택 화면-애니메이션
void draw_song(int* sel, char key, struct song*);                             //곡 선택 화면-곡 목록 출력
void wirte_somnail(struct song* sn);			                              //정보 입력 함수
void move_draw_song(char key, struct song*);	                              //곡 선택 화면-곡 목록 옮기기
int draw_select_song(int x_m, int y_m, int x, int y);	                      //곡 옮기는 함수 사용하여 출력 + 곡명, 작곡가 값 맞추기
int draw_game_screen(int song, int diff);                            //인게임 화면
void draw_key_effect(char* key);                                              //게임화면-키를 누를 시 반응
void delete_key(char* key);                                                   //게임화면-이펙트 반응 후 초기화
void note_to_note(int* num, int notelen, char f_note[][5], int note[4][24]);  //게임화면-텍스트로 불러온 노트를 게임으로 옮기기
void draw_note(int vac[4][24]);                                               //게임화면-노트 그리기
int judge_acc(char* key, int note[4][24]);                                    //게임화면-노트 치는 타이밍 판단 함수
void draw_judge(int* jud);                                                    //게임화면-콤보명칭(perfect/great/good/bad...) 출력함수
void draw_score_combo_life_coin();                                            //게임화면-점수,콤보 수, 생명, 보유코인 수
void erease_judge();                                                          //게임화면-이펙트를 삭제
bool qus_use_coin();                                                          //코인 사용 유무 판별
void random_coin();                                                           //랜덤코인상자 출력
void game_clear();                                                            //게임 클리어시 출력창
void game_stop();                                                             //게임 중간에 나갈 시 출력창
void game_over();                                                             //게임 탈락 시 출력창
void rank(INFO temp, int score);                                              //순위 매기기위한 이름 입력받기, 이진 파일에 저장
void print_rank(INFO p[arrMAXSIZE]);                                          //랭크 출력

void gotoxy(int x, int y)         //x와 y에 저장된 좌표로 커서를 옮겨주는 함수
{
    COORD Pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void move_cursor(char key, int* x, int* y, int x_m, int y_m)   //화살표 방향(key) 기반으로 커서 좌표 값 변경
{
    switch (key)
    {
    case 72:        //위방향
        *y = *y - 1;
        if (*y < 1) *y += 1;
        break;
    case 75:        //왼쪽방향
        *x = *x - 1;
        if (*x < 1) *x += 1;
    case 77:        //오른쪽방향
        *x += 1;
        if (*x > x_m) *x -= 1;
    case 80:        //아래방향
        *y += 1;
        if (*y > y_m) *y -= 1;
        break;
    }
}

//전역변수 선언 부분 
int score;              //점수 합산을 위한 변수
int combo, max_combo;   //현재 콤보와 최대 콤보 저장 변수
int offset = 90;        //노트 내려오는 속도와 관련된 고정값
char g_key[4] = { 'a', 's', 'd', 'f' }; //사용자가 입력할 키 값
int life = 5;           //목숨 관련 변수
int coin = 0;           //코인 관련 변수
INFO temp;              //랭크 관련 변수
INFO p[arrMAXSIZE];     //랭크 관련 변수

//메인함수
int main(void)
{
    //goto문 사용으로 시작 지점으로 돌아올 수 있게 만듦.
START:
    //변수 선언 및 초기화
    int x, y, select, page;
    x = 21; y = 12;

    select = draw_title(x, y);      //사용자가 선택한 메뉴 값 저장

    for (;;)
    {
        if (select == 0)            //게임 시작
        {
            draw_explanation();     //게임 설명 페이지

            page = draw_select_song(X_MAX, Y_MAX, 67, 4);   //함수 호출하면서 곡 선택 페이지로 이동
            if (page == 2) {        //리턴 값이 2라면 시작화면으로 돌아가기
                goto START;
            }
            break;
        }
        else if (select == 1)       //프로그램 종료
        {
            return 0;               //메인함수 종료
            break;
        }
        else if (select == 2)   //랭킹페이지
        {
            print_rank(p);
            system("cls");
            goto START;
        }
    }


}

int draw_title(int x, int y)        //메뉴 선택 창.
{
    char key;                       //키값 저장을 위한 변수
    for (;;)
    {
        //페이지 출력
        printf("┌───────────────────────────────────────────────┐\n");
        printf("│      ######################################   │\n");
        printf("│      ##                                  ##   │\n");
        printf("│      ##     ##  ###                      ##   │\n");
        printf("│      ##     ## ###    ##   ##    ##      ##   │\n");
        printf("│      ##     ####       ## ## ## ##       ##   │\n");
        printf("│      ##     ## ###      ###   ###        ##   │\n");
        printf("│      ##     ##  ###      #     #         ##   │\n");
        printf("│    ####     ##   ###                   ####   │\n");
        printf("│   #####                              ######   │\n");
        printf("│  #####                              ######    │\n");
        printf("└───────────────────────────────────────────────┘\n");
        printf("                      시작                                           \n");
        printf("                      종료                                           \n");
        printf("                      랭킹화면                                       \n");
        printf("                                                                       \n");
        printf("         이동 : ←,→,↑,↓   확인 : space               \n");
        printf("                                                                       \n");
        printf("                                     개발팀 : 언더더C                    \n");
        printf("                                \n");

        for (;;)        //화살표로 값을 계속 입력받기 위한 무한 반복문.
        {
            //gotoxy - 커서 이동. //\b는 백스페이스로 지우는 역할. //이동 후 지우고 > 출력
            gotoxy(x, y); printf("\b\b"); printf(">");

            key = _getch();                     //키 입력 받음.
            if (y >= 12 && y <= 14)             //y값이 범위에 맞으면 실행.
            {
                if (key == 72 || key == 80)     //위 아래 화살표값이면
                {
                    printf("\b\b"); printf("  ");   //지우고 공백 출력

                    move_cursor(key, &x, &y, X_MAX, Y_MAX);   //key에 따른 y값 증감연산

                    if (y > 14) y -= 3;      //범위 벗어나면 최대 최소에 따라 값 맞춤.
                    if (y < 12) y += 3;
                }
                else if (key == 32)              //스페이스로 선택
                {
                    if (y == 12)                 //게임 시작
                    {
                        system("cls");           //창 초기화
                        return 0;                //메뉴 선택 : 게임 시작
                    }
                    else if (y == 13)             //종료
                    {
                        system("cls");            //창 초기화
                        return 1;                //메뉴 선택 : 종료
                    }
                    else if (y == 14)   //랭킹페이지
                    {
                        system("cls");
                        return 2;                //메뉴 선택 : 랭킹페이지
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
    //설명화면 출력
    printf("\n\n");
    printf("          [게임 설명]                                                  [확인 : Enter any key]\n\n");
    printf("          ──────────────────────────────────────────────────────────────────────────────────\n");
    printf("          1. 키보드의 A,S,D,F로 내려오는 노트에 맞추어 조작키를 누른다.                           \n");
    printf("\n");
    printf("          2. 사용자가 누른 타이밍에 따라 콤보 명칭이 뜨며 각 콤보별로 점수가 누적된다.            \n");
    printf("\n");
    printf("      Perfect : 10점                                 \n");
    printf("      Great   : 6점                                 \n");
    printf("      Good    : 2점                                 \n");
    printf("      Bad     : 0점                                 \n");
    printf("\n");
    printf("          3. 체력은 총 ♥♥♥♥♥로 주어지며 Bad일시 하나씩 차감! 전부 떨어지면 Game Over!!!                                   \n");
    printf("                                                                       \n");
    printf("          4. Game Clear시, 상자에서 코인을 랜덤으로 얻을 수 있다.                     \n");
    printf("             이 코인은 다음 게임에서 Game Over시 한 번 이어 할 수 있게 해준다.              \n");
    printf("                                                                       \n");
    printf("          5. Game Over시 코인 하나를 사용하여 이어할 수 있다!!                  \n");
    printf("             누적된 점수로 랭킹등록 가능!!! 랭커가 되어보자!!!\n");
    printf("          ──────────────────────────────────────────────────────────────────────────────────\n\n");
    _getch();
    system("cls");  //창 초기화
    //끝나면 메인함수로 돌아가며, 다음 페이지 호출
}

int draw_select_song(int x_m, int y_m, int x, int y)
{
SELECT_RESTART:     //goto문으로 난이도 선택 취소 할 경우 돌아오게 만듦.
    FMOD_SYSTEM* g_System;                  //사운드 시스템 생성을 위해서 변수 선언
    FMOD_SOUND* sound[3], * move_sel;       //사운드 객체 생성, 출력할 음원 파일의 개수만큼 선언
    FMOD_CHANNEL* g_channel = NULL;         //채널을 통해 사운드 출력
    unsigned int      version;

    FMOD_System_Create(&g_System);          //시스템 객체 생성
    FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL, NULL);     //초기화

    FMOD_System_GetVersion(g_System, &version);                 //fmod 실행하기위해서 버전이 맞는지 확인
    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 1;
    }
    //사운드 로딩함수가 사운드 파일을 읽어와서 사운드 객체에 대응시킴
    FMOD_System_CreateSound(g_System, "sound/select_sound/D.mp3", FMOD_LOOP_NORMAL, 0, &sound[0]);
    FMOD_System_CreateSound(g_System, "sound/select_sound/H.mp3", FMOD_LOOP_NORMAL, 0, &sound[1]);
    FMOD_System_CreateSound(g_System, "sound/select_sound/R.mp3", FMOD_LOOP_NORMAL, 0, &sound[2]);
    FMOD_System_CreateSound(g_System, "sound/l_r_sound/1.mp3", FMOD_HARDWARE, 0, &move_sel);      // 화살표 움질일 때 나는 효과음

    //지역 변수 선언.
    char key = 0;                   //키갑 저장
    char pic[8][8] = { 0, };        //애니메이션 관련 배열
    int song = 1, difficulty = 1;   //노래와 난이도 관련 변수 선언 및 초기화.
    int selected_music = 2, playing_music = 2, sw = 0, first = 1, l_r = 0;  //노래 재생 관련 변수
    struct song music[3];           //구조체 배열로 곡 3개 저장.
    int page;                       //페이지 값.
    int equalizer_count = 0;        //애니메이션 관련 변수


    wirte_somnail(music);           //곡정보 입력
    srand((unsigned int)time(NULL));//난수 생성
    system("cls");                  //화면 초기화
    //출력
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("                                                          난이도 : "); printf("\n");
    printf("                                                          - 0. 곡선택"); printf("\n");
    printf("                                                          - 1. easy"); printf("\n");
    printf("                                                          - 2. normal"); printf("\n");
    printf("                                                          - 3. hard"); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("  "); printf("\n");
    printf("                           ┌─────────────────────┐ "); printf("\n");
    printf("    곡명 :                 │  곡명 :             │      곡명 :                   "); printf("\n");//곡명 : 15칸
    printf("    아티스트 :             │  아티스트 :         │      아티스트 :               "); printf("\n");//아티스트 : 11칸
    printf("                           └─────────────────────┘ "); printf("\n");
    printf("     "); printf("\n");
    printf("   ----------------------------------------------------------------------\n");
    printf("   곡 이동 : ←,→    선택 : space    나가기 : q\n");
    printf("   ----------------------------------------------------------------------\n");
    printf("   TIP! : 곡 먼저 선택 후, 난이도 조절. ");

    Sleep(800);         //0.8초간 프로그램 지연

    for (;;)            //화살표값 계속 받기 위한 무한반복문.
    {
        if (first == 1) //첫 실행이라면, 사운드 재생
        {
            FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, sound[1], 0, &g_channel);
            first = 0;
        }

        if (sw == 1)     //기본값이 0이므로 처음에는 실행안되고 화살표 좌우 누르면 곡이 변경되면서 처음에 실행된 사운드 풀어줌.
        {
            if (l_r == 1)                       //l_r 기본값 0임, 화살표 좌우에 따라 값 변경됨. 이 경우는 화살표 오른쪽 눌렀을 때
            {
                switch (playing_music)          //현재 실행되고 있는 노래 값에 맞게 노래 종료
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
                playing_music--;                //종료 후 화살표 좌우에 따라 노래 값 변경됨.
                if (playing_music < 1)          //범위 넘어가지 않게 조정
                    playing_music = 3;
            }
            else if (l_r == 2)                  //이 경우는 화살표 왼쪽 눌렀을 때
            {
                switch (playing_music)          //현재 실행되고 있는 노래 값에 맞게 노래 종료
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
                playing_music++;                //종료 후 화살표 좌우에 따라 노래 값 변경됨.
                if (playing_music > 3)          //범위 넘어가지 않게 조정
                    playing_music = 1;
            }

        }
        if (sw == 1)//곡을 변환하기위한 스위치
        {
            if (selected_music == 1)
            {
                FMOD_System_CreateSound(g_System, "sound/select_sound/D.mp3", FMOD_LOOP_NORMAL, 0, &sound[0]);
                FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, sound[0], 0, &g_channel);        //사운드 출력

                sw = 0;
            }
            else if (selected_music == 2)
            {
                FMOD_System_CreateSound(g_System, "sound/select_sound/H.mp3", FMOD_LOOP_NORMAL, 0, &sound[1]);
                FMOD_Channel_Stop(g_channel);
                FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, sound[1], 0, &g_channel);        //사운드 출력

                sw = 0;
            }
            else if (selected_music == 3)
            {
                FMOD_System_CreateSound(g_System, "sound/select_sound/R.mp3", FMOD_LOOP_NORMAL, 0, &sound[2]);
                FMOD_Channel_Stop(g_channel);
                FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, sound[2], 0, &g_channel);        //사운드 출력

                sw = 0;
            }
        }
        for (;;)
        {
            Sleep(100);             //0.1초간 지연

            if (_kbhit() == 1)      //getch로 입력받으면 키 입력까지 무한대기하므로 kbhit을 사용하여 아래 코드 진행
            {                       //kbhit은 키의 입력이 있으면 1 반환 없으면 무시하고 지나가기 때문.
                key = _getch();     //키 입력
            }
            draw_equalizer(&equalizer_count, pic);       //애니메이션 호출.
            draw_song(&selected_music, key, music);      //화면에 곡 정보 출력
            if (key == 'q')      //메인메뉴로 돌아가기
            {
                switch (playing_music)      //음악 종류에 따라
                {
                case 1:
                    FMOD_Sound_Release(sound[0]);       //종료
                    break;
                case 2:
                    FMOD_Sound_Release(sound[1]);       //종료
                    break;
                case 3:
                    FMOD_Sound_Release(sound[2]);       //종료
                    break;
                }
                system("cls");                          //창초기화
                return 2;                               //2 리턴 -> 시작화면 이동
            }

            else if (key == 32)      //32 - 스페이스 확인 후, 난이도 선택
            {
                gotoxy(x, y);   scanf_s("%d", &difficulty);   //이동 후 값 입력받기.
                if (difficulty == 0)
                {
                    //곡에 따라 노래 종료.
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
                    goto SELECT_RESTART;        //돌아가서 재선택
                }
                else if (difficulty >= 1 || difficulty <= 3)
                {
                    //곡에 따라 노래 종료.
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
                    system("cls");          //창 초기화
                    page = draw_game_screen(playing_music, difficulty); //게임 스크린 실행 및 리턴 값 저장
                    if (page == 2)          //리턴 값 2 라면 : 게임 클리어 or 게임 도중 퇴장
                    {
                        system("cls");      //창 초기화
                        return page;        //page : 2 리턴 -> 시작화면으로 이동
                    }
                    game_over();            //그 외의 값(게임 탈락)이면 게임오버 출력

                    system("cls");          //창 초기화
                    return 2;               //2 리턴 -> 시작화면으로 이동
                }

            }
            else if (key == 75 || key == 77)   //화살표 좌우, 값 이동하면서 곡 변경.
            {
                move_draw_song(key, music);    //곡 변경
                FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, move_sel, 0, &g_channel);
                Sleep(100);                    //0.1초 지연
                if (key == 77)                 //오른쪽이면
                {
                    //곡 값에 맞게 변경
                    selected_music--;
                    if (selected_music < 1) //범위 넘어가지 않게 조정
                        selected_music = 3;
                    l_r = 1;
                    sw = 1;
                    key = 0;
                    break;
                }
                else if (key == 75)            //왼쪽이면
                {
                    //곡 값에 맞게 변경
                    selected_music++;
                    if (selected_music > 3) //범위 넘어가지 않게 조정
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

void draw_equalizer(int* equalizer_count, char pic[][8])      //랜덤 값을 배열에 저장(3되면 초기화) 후, 그 값에 따라 출력.
{
    //변수 선언 및 초기화
    int x = 30, y = 5, i, j, k;

    //카운트가 3이면, -> 카운트에 따라 이퀄라이져가 증감하는 것을 자연스럽게 하기 위함.
    if (*equalizer_count == 3)
    {
        //3이면 난수로 초기화
        for (i = 0; i < 8; i++)
        {
            k = rand() % 5;
            pic[i][k] = 1;
        }
        //랜덤으로 정한 값까지 채우기
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
        //초기화
        *equalizer_count = 0;
    }
    //값을 한칸씩 이동해, 자연스럽게 보이기.
    for (i = 0; i < 8; i++)
    {
        for (j = 7; j > 0; j--)
        {
            pic[i][j] = pic[i][j - 1];
            pic[i][j - 1] = 0;
        }
    }

    //출력 : 저장을 거꾸로 했기에 pic[j][i]로 접근해 출력
    for (i = 0; i < 8; i++)
    {
        gotoxy(x, y);
        for (j = 0; j < 8; j++)
        {
            if (pic[j][i] == 1)
                printf("■");
            else if (pic[j][i] == 0)
                printf("  ");
        }
        y += 1;
    }
    //카운트 증가
    *equalizer_count = *equalizer_count + 1;

    return;


}

void wirte_somnail(struct song* sn)      //곡 정보 저장
{

    strcpy_s(sn->songname, 20, "Ref");
    strcpy_s(sn->artist, 20, "Vk");

    strcpy_s((sn + 1)->songname, 20, "HSD");
    strcpy_s((sn + 1)->artist, 20, "ND");

    strcpy_s((sn + 2)->songname, 20, "Dgg");
    strcpy_s((sn + 2)->artist, 20, "Mm");

}

void draw_song(int* sel, char key, struct song* sn)      //곡정보 출력
{
    int  y = 16;
    gotoxy(11, y);                  //gotoxy로 이동 후 정보 출력
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
    //구조체 및 변수 생성
    struct song temp;
    int i;
    switch (key)//키(화살표)에 따라
    {
        //temp구조체로 값 교환
        //temp = a
        //a = b
        //b = temp
    case 77:
        // _TRUNCATE의 값은 unsigned int의 최대값 즉 약 42억
        // strcpy_s를 strcpy처럼 바꿈(첫번째 인자 버퍼는 보호하지 않음)
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
    //코인 사용 여부 저장 변수
    bool coin_used = 0;
    //코인이 있다면
    if (coin > 0)
    {
        //사용 여부 조사 후 창 초기화
        coin_used = qus_use_coin();
        system("cls");
    }
    FMOD_SYSTEM* g_System;              //사운드 시스템 생성을 위해서 변수 선언
    FMOD_SOUND* sound[3];               //사운드 객체 생성, 출력할 음원 파일의 개수만큼 배열로 선언
    FMOD_CHANNEL* g_channel = NULL;     //채널을 통해 사운드 출력
    unsigned int      version;

    FMOD_System_Create(&g_System);      //시스템 객체 생성
    FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL, NULL);     //초기화

    FMOD_System_GetVersion(g_System, &version);         //fmod 실행하기위해서 버전이 맞는지 확인
    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        exit(1);
    }

    //사운드 로딩 함수가 사운드 파일을 읽어와서 사운드 객체에 대응시킴
    FMOD_System_CreateSound(g_System, "sound/game_sound/Dingga.mp3", FMOD_DEFAULT, 0, &sound[0]);
    FMOD_System_CreateSound(g_System, "sound/game_sound/High_School_Dropout.mp3", FMOD_DEFAULT, 0, &sound[1]);
    FMOD_System_CreateSound(g_System, "sound/game_sound/Reflection.mp3", FMOD_DEFAULT, 0, &sound[2]);

    //변수 선언
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

    //초기값 설정
    //----------------------------------------------------------------
    score = 0;      //점수 초기화
    combo = 0;      //콤보 초기화
    life = 5;       //목숨 초기화
    max_combo = 0;  //최대 콤보 초기화
    //-------------------------------------------------------------------

    //텍스트 파일 불러오기
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
        printf("파일 불러오기 실패\n");
    //파일 길이 측정
    //**********************************
    fseek(notefile, 0L, SEEK_END);
    n_of_file = ftell(notefile);
    n_of_file = (n_of_file / 6) + 1;

    //********************************
    //파일을 가져온 후, 분석하여 배열에 저장
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

    //게임 창 출력
    for (i = 0; i < 21; i++)
    {
        gotoxy(x, y);
        for (j = 0; j < 4; j++)
        {
            printf("│       ");
        }
        printf("│");
        y += 1;
    }
    gotoxy(x, y);
    printf("├───────┼───────┼───────┼───────┤");
    y += 1;
    gotoxy(x, y);
    for (j = 0; j < 5; j++)
    {
        printf("│       ");
    }
    y += 1;
    gotoxy(x, y);
    printf("└───────┴───────┴───────┴───────┘");
    gotoxy(60, 2);
    printf("q:정지메뉴");

    for (playlen = 0; playlen < n_of_file + 22; playlen++)
    {
        if (note[3][20] == 1 || note[2][20] == 1 || note[1][20] == 1 || note[0][20] == 1 || score != 0)
        {//각 키값의 노트가 y좌표 20을 지나 내려오기 시작하면 노래시작
            if (play_count == 0)        //사용자 키 입력을 위해 반복문 필요한데 사운드도 반복 출력되는것을 막기 위해 play_count 변수 사용.
            {
                if (song == 1)          //전달받은 곡 값에 따라 알맞은 사운드 출력
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
        //키 입력 받기
        if (_kbhit())
            key = _getch();
        //q이면
        if (key == 'q')
        {
            //노래 종료 후
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
            //게임 탈출
            game_stop();
            //2 리턴으로 시작화면으로 이동
            return 2;
        }
        if (key == 'a' || key == 's' || key == 'd' || key == 'f')   //asdf입력시
        {
            draw_key_effect(&key); //게임화면 - 키를 누를 시(asdf) : ■■■가 나오게 하는 함수
            judge = judge_acc(&key, note); //게임화면 - 노트 판정 함수 : great, perfect...
        }
        //judge_acc에서 오차범위로 처리하기에 bad는 처리하지 않아 따로 판정
        if (note[0][23] == 1 || note[1][23] == 1 || note[2][23] == 1 || note[3][23] == 1)
            judge = 4; //bad

        if (judge == 2)     //perfect
        {
            if (coin_used)  //코인 사용시 점수 두배
                score += 20;
            else
                score += 10;
            combo++;
            if (max_combo < combo)//콤보가 맥스콤보를 넘으면 저장
                max_combo = combo;
        }
        else if (judge == 1) //great
        {
            if (coin_used)   //코인 사용시 점수 두배
                score += 12;
            else
                score += 6;
            combo++;
            if (max_combo < combo)//콤보가 맥스콤보를 넘으면 저장
                max_combo = combo;
        }
        else if (judge == 3) //good
        {
            if (coin_used)   //코인 사용시 점수 두배
                score += 4;
            else
                score += 2;
            combo++;
            if (max_combo < combo)//콤보가 맥스콤보를 넘으면 저장
                max_combo = combo;
        }
        else if (judge == 4)//bad
        {
            combo = 0; //bad 일 때 combo수 초기화.
            life -= 1; //bad일 때 체력감소.
        }
        if (life == 0)      //라이프 모두 소진시
        {   //노래 종료 후,
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
            //0 리턴
            return 0;
        }
        draw_score_combo_life_coin();   //콤보, 생명, 점수 등 출력
        draw_judge(&judge);             //판단값 출력

        note_to_note(&num, n_of_file, file_note, note); //게임화면-텍스트로 불러온 노트를 게임으로 이식
        draw_note(note);                //게임화면-노트 그리기
        Sleep(offset);                  //잠깐 지연 -> 노트 내려오는 속도 조절

        for (i = 0; i < 4; i++)         //배열에 있는 노트 값 내리기
        {
            for (j = 23; j > 0; j--)
            {
                note[i][j] = note[i][j - 1];
                note[i][j - 1] = 0;
            }
        }

        note[i][20] = 0;                //note[4][20] = 0 
        delete_key(&key);               //게임화면 - 사용자입력 ■■■표시 후 초기화
        erease_judge();                 //게임화면 - 이펙트(판단값)를 삭제
    }
    //노트 끝나면 노래 끔
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
    //게임 클리어
    game_clear();   //클리어 창 출력
    system("cls");  //창 초기화
    random_coin();  //랜덤코인상자함수
    rank(temp, score);  //이름 입력받아서 이진파일에 저장
    system("cls");  //창 초기화
    print_rank(p);  //랭킹 출력

    return 2;       //2 리턴하여 시작화면으로 이동
}

void note_to_note(int* num, int notelen, char f_note[][5], int note[4][24])
//게임화면-텍스트로 불러온 노트를 게임으로 이식
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

void draw_note(int vac[4][24])  //노트 출력
{
    //지역 변수 선언 및 초기화
    int x = 12, y = 0;
    int i, j;
    //x, y에 맞게 이동
    gotoxy(x, y);
    for (i = 0; i < 21; i++)    //세로 반복 - 한칸씩 내려가며
    {

        for (j = 0; j < 4; j++) //가로 반복 - 한칸씩 오른쪽으로 가며
        {
            if (i == 20 || i == 19 || i == 18 || i == 17)
            {
                gotoxy(x, i);
                printf("	  ");          //빈칸 출력으로 지우기
            }
            gotoxy(x, y);
            if (vac[j][i] == 1)            //1이면 노트 출력
            {
                printf("■■■");

                if (y > 0)                 //y 양수면 
                {
                    gotoxy(x, y - 1);      //한칸 올라가서(전에 출력된 값)
                    printf("	  ");      //지우기
                }
            }
            else if (vac[j][i] == 0)       //0이면 빈칸 출력
            {
                printf("      ");
            }
            x += 8;     //좌표 값도 오른쪽으로 이동
        }
        x = 12;         //다시 asdf에서 a키에 맞게 초기화
        y += 1;         //한줄 내리기
    }
}




void draw_key_effect(char* key) // 사용자 키보드 입력 시, 맨밑에 ■■■ 출력.(입력했다는 표시)
{
    if (*key == g_key[0])       //g_key[0] = 키보드 a
    {
        gotoxy(12, 22);
        printf("■■■");
    }
    else if (*key == g_key[1])  //g_key[1] = 키보드 s
    {
        gotoxy(20, 22);
        printf("■■■");
    }
    else if (*key == g_key[2])  //g_key[2] = 키보드 d
    {
        gotoxy(28, 22);
        printf("■■■");
    }
    else if (*key == g_key[3])  //g_key[3] = 키보드 f
    {
        gotoxy(36, 22);
        printf("■■■");
    }
}

void delete_key(char* key) //게임화면 - 사용자입력 ■■■표시 후 초기화
{
    if (*key == g_key[0])       //g_key[1] = 키보드 a
    {
        gotoxy(12, 22);
        printf("      ");       //빈칸으로 없애기
    }
    else if (*key == g_key[1])  //g_key[1] = 키보드 s
    {
        gotoxy(20, 22);
        printf("      ");       //빈칸으로 없애기
    }
    else if (*key == g_key[2])  //g_key[1] = 키보드 d
    {
        gotoxy(28, 22);
        printf("      ");       //빈칸으로 없애기
    }
    else if (*key == g_key[3])  //g_key[1] = 키보드 f
    {
        gotoxy(36, 22);
        printf("      ");       //빈칸으로 없애기
    }
    *key = 0;                   //키 초기화
}


int judge_acc(char* key, int note[4][24]) //게임화면-정확도 판단 함수 이 함수의 반환값을 judge에 저장
{

    if (*key == g_key[0]) //a 입력시 
    {
        if (note[0][20] == 1 || note[0][21] == 1) // 노트타이밍과 오차범위 +-1 
        {
            if (note[0][20] == 1)      note[0][20] = 0;     //0으로 바꾸는 이유는 키를 더이상 인게임에서 보이면 안되기 때문.
            else if (note[0][21] == 1) note[0][21] = 0;
            return 2; //"perfect"
        }

        else if (note[0][18] == 1 || note[0][19] == 1)  // 노트타이밍과 오차범위 +-2
        {
            if (note[0][18] == 1)       note[0][18] = 0;
            else if (note[0][19] == 1)	note[0][19] = 0;
            return 1; //"great"
        }

        else if (note[0][16] == 1 || note[0][17] == 1)  // 노트타이밍과 오차범위 +-3
        {
            if (note[0][16] == 1)        note[0][16] = 0;
            else if (note[0][17] == 1) 	 note[0][17] = 0;
            return 3; //"good"
        }

    }

    if (*key == g_key[1])                               //s 입력시
    {
        if (note[1][20] == 1 || note[1][21] == 1)       // 노트타이밍과 오차범위 +-1 
        {
            if (note[1][20] == 1)      note[1][20] = 0;
            else if (note[1][21] == 1) note[1][21] = 0;
            return 2;                                   //"perfect"
        }

        else if (note[1][18] == 1 || note[1][19] == 1)  // 노트타이밍과 오차범위 +-2
        {
            if (note[1][18] == 1)       note[1][18] = 0;
            else if (note[1][19] == 1)	note[1][19] = 0;
            return 1;                                   //"great"
        }

        else if (note[1][16] == 1 || note[1][17] == 1)  // 노트타이밍과 오차범위 +-3
        {
            if (note[1][16] == 1)        note[1][16] = 0;
            else if (note[1][17] == 1) 	 note[1][17] = 0;
            return 3;                                   //"good"
        }

    }

    if (*key == g_key[2])                               //d 입력시
    {
        if (note[2][20] == 1 || note[2][21] == 1)       // 노트타이밍과 오차범위 +-1 
        {
            if (note[2][20] == 1)      note[2][20] = 0;
            else if (note[2][21] == 1) note[2][21] = 0;
            return 2;                                   //"perfect"
        }

        else if (note[2][18] == 1 || note[2][19] == 1)  // 노트타이밍과 오차범위 +-2
        {
            if (note[2][18] == 1)       note[2][18] = 0;
            else if (note[2][19] == 1)	note[2][19] = 0;
            return 1;                                   //"great"
        }

        else if (note[2][16] == 1 || note[2][17] == 1)  // 노트타이밍과 오차범위 +-3
        {
            if (note[2][16] == 1)        note[2][16] = 0;
            else if (note[2][17] == 1) 	 note[2][17] = 0;
            return 3;                                   //"good"
        }

    }


    if (*key == g_key[3])                               //f 입력시
    {
        if (note[3][20] == 1 || note[3][21] == 1)       // 노트타이밍과 오차범위 +-1 
        {
            if (note[3][20] == 1)      note[3][20] = 0;
            else if (note[3][21] == 1) note[3][21] = 0;
            return 2;                                   //"perfect"
        }

        else if (note[3][18] == 1 || note[3][19] == 1)  // 노트타이밍과 오차범위 +-2
        {
            if (note[3][18] == 1)       note[3][18] = 0;
            else if (note[3][19] == 1)	note[3][19] = 0;
            return 1;                                   //"great"
        }

        else if (note[3][16] == 1 || note[3][17] == 1)  // 노트타이밍과 오차범위 +-3
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
    //이동해서
    gotoxy(x, y);
    //판단값에 따라 출력
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

void erease_judge() // 게임화면 - 이펙트를 삭제
{
    int x = 45, y = 15;
    //이동해서
    gotoxy(x, y);
    //빈칸으로 지우기
    printf("              ");
}

void draw_score_combo_life_coin() //점수와 콤보 출력함수
{
    //이동해서 출력
    gotoxy(45, 9);
    printf("점수 : ");
    gotoxy(52, 9);
    printf("%d", score);
    gotoxy(45, 10);
    printf("COMBO : ");
    gotoxy(52, 10);
    printf("%3d", combo);
    gotoxy(45, 11);
    printf("체력 : ");
    gotoxy(52, 11);
    if (life == 5) printf("♥♥♥♥♥");
    if (life == 4) printf("♥♥♥♥♡");
    if (life == 3) printf("♥♥♥♡♡");
    if (life == 2) printf("♥♥♡♡♡");
    if (life == 1) printf("♥♡♡♡♡");
    if (life == 0) printf("♡♡♡♡♡");
    gotoxy(45, 12);
    printf("보유 코인");
    gotoxy(55, 12);
    printf("%d개", coin);
    gotoxy(45, 13);
    printf("나가기 : q");

}

bool qus_use_coin()     //코인 사용 여부 조사
{

re:
    //출력
    system("cls");
    printf("┌───────────────────────────────────────────────┐\n");
    printf("│                                               │\n");
    printf("│                코인을 사용하여                │\n");
    printf("│                                               │\n");
    printf("│           점수를 2배 하시겠습니까 ? (Y/N)     │\n");
    printf("│                    :                          │\n");
    printf("│                 보유 코인 : %d 개              │\n", coin);
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("└───────────────────────────────────────────────┘\n");
    char answer;
    gotoxy(23, 5);
    //이동해서
    scanf_s(" %c", &answer, sizeof(answer));    //값 입력받기, " %c"로 버퍼 비우고 입력

    if (answer == 'Y' || answer == 'y')     //사용시
    {
        coin -= 1;          //사용시 코인 차감
        return 1;           //1리턴
    }
    else if (answer == 'N' || answer == 'n')//비사용시
    {
        return 0;           //0리턴
    }

    else
    {
        //잘못된 키 입력시 goto로 이동
        gotoxy(15, 7);
        printf("잘못입력하셨습니다.");
        gotoxy(15, 8);
        printf("아무키나 눌러주세요");
        //아무 키 입력 대기
        _getch();
        goto re;
    }
}

void random_coin()      //코인 박스
{
    system("cls");
    printf("┌───────────────────────────────────────────────┐\n");
    printf("│                                               │\n");
    printf("│          랜덤 코인을 골라주세요 :             │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("└───────────────────────────────────────────────┘\n");

    gotoxy(15, 5);
    printf("1\t2\t3\n");

    int box[3] = { 0,0,1 }; //임의로 지정. 배열값이 1인 상자에 코인 1개 들어있는 것으로 설정, 
                            //나머지는 0개의 코인.

    //shuffle - 배열의 인덱스 값들을 랜덤으로 돌려서 0 ,0, 1 값을 랜덤으로 지정.
    srand((unsigned int)time(NULL));            //난수 생성
    for (int i = 2; i > 0; i--)
    {
        int j = rand() % (i + 1);               //난수 생성
        int temp = 0;
        temp = box[i];                          //박스 교환
        box[i] = box[j];
        box[j] = temp;
    }

    int choice;
    gotoxy(23, 7);              //이동 후, 상자 선택 값 입력
    scanf_s("%d%*c", &choice);

    if (box[choice] == 1)       //값이 1이면
    {
        gotoxy(15, 9);
        printf("코인 1개를 뽑으셨습니다.\n");
        coin += 1;              //코인 증가
    }
    else
    {
        gotoxy(23, 9);
        printf("꽝");
    }
    //이동 후 출력
    gotoxy(15, 11);
    printf("현재 보유 코인 : %d개\n", coin);
    gotoxy(0, 2);
}

void game_clear()
{
    system("cls");          //창 초기화
    //출력
    printf("┌───────────────────────────────────────────────┐\n");
    printf("│                                               │\n");
    printf("│               GAME      CLEAR!!!              │\n");
    printf("│                                               │\n");
    printf("│               최종 점수  : %4d               │\n", score);
    printf("│                                               │\n");
    printf("│             최대 콤보 수 : %3d                │\n", max_combo);
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│     아무 키나 누르면 랜덤 코인 박스로 이동    │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("└───────────────────────────────────────────────┘\n");

    _getch();       //아무키 입력 대기
    return;
}



void game_stop()
{
    system("cls");          //창 초기화
    //출력
    printf("┌───────────────────────────────────────────────┐\n");
    printf("│                                               │\n");
    printf("│               GAME      STOP!!!               │\n");
    printf("│                                               │\n");
    printf("│               최종 점수  : %4d               │\n", score);
    printf("│                                               │\n");
    printf("│             최대 콤보 수 : %3d                │\n", max_combo);
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│     아무 키나 누르면 시작 화면으로 이동       │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("└───────────────────────────────────────────────┘\n");

    _getch();           //아무키 입력 대기
    return;
}

void game_over()
{
    system("cls");      //창 초기화
    //출력
    printf("┌───────────────────────────────────────────────┐\n");
    printf("│                                               │\n");
    printf("│               GAME      OVER!!!               │\n");
    printf("│                                               │\n");
    printf("│               최종 점수  : %4d               │\n", score);
    printf("│                                               │\n");
    printf("│             최대 콤보 수 : %3d                │\n", max_combo);
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│     아무 키나 누르면 시작 화면으로 이동       │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("│                                               │\n");
    printf("└───────────────────────────────────────────────┘\n");

    _getch();       //아무 키 입력 대기
    return;
}


void rank(INFO temp, int score) {

    FILE* fp = NULL;
    fopen_s(&fp, "rank.bin", "ab");
    if (fp == NULL)		exit(1);

    // 게임 클리어 했을때 이름 입력받고 점수 전달받아서 이진파일에 추가하며 저장
    gotoxy(13, 13);
    printf("이름을 입력하세요 : ");
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

    //프로그램 종료했다가 다시 랭킹화면 실행해도 이전의 순위가 남아있게 설계함
    //이진파일에 저장된 내용들 구조체 배열에 저장
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

    //순위 1위부터 정렬하고 이진파일에 저장
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

    //이진파일의 처음으로 돌아감.
    fseek(fpout, 0, SEEK_SET);

    //이진파일 읽어서 1위부터 10위까지 순서대로 출력
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
    printf("아무 키나 누르면 메뉴 화면으로 이동");
    _getch();               //아무 키 입력 대기
}