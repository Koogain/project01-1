#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include "struct.h"

HANDLE hConsole;

// 전처리 변수
#define BTN_WIDTH 20
#define BTN_HEIGHT 3
#define FRAME_WIDTH 40
#define FRAME_HEIGHT 20

#define LOGIN_X 10
#define LOGIN_Y 3

#define PASS_X 10
#define PASS_Y 8

#define SIGNUP_X 14
#define SIGNUP_Y 14

// 함수 전방 선언
bool isInside(int x, int y, int left, int top, int right, int bottom);
void gotoxy(int x, int y);
void trimAll(char* str);
void setColor(WORD color);
int getRandomColor();
void drawBox(int x, int y, int width, int height);
void getTextInput(int x, int y, char* buffer, int maxLen);

void handleLoginScreen(char* userName, char* userID, char* userPW);
void handleSignupScreen(char* userName, char* userID, char* userPW);

void handleMainMenuScreen(char* userName, char* userID);

int getDayIndexFlexible(const char* day);
int getStartHour(const char* timeStr);

void handleTimetableScreen(char* userName, char* userID);
void handleSubjectList(const char* userID);

void handleFriendsListScreen(char* userName, char* userID, char* userPW);
void handleAddFriendScreen(char* userName, char* userID);

void handleFriendPin();

// 15 : 하얀색, 13 : 밝은 자주색

// 전역 변수
char userName[50] = { 0 };
char userID[50] = { 0 };
char userPW[50] = { 0 };

// 메인함수
int main()
{
    system("MODE CON:COLS=100 LINES=50");      // 콘솔 창 크기 설정
    system("cls");

    drawBox(4, 1, 92, 47);      // 전체 틀

    handleLoginScreen(userName, userID, userPW);

    gotoxy(0, 22); // 커서 내리기
    _getch();
    return 0;
}

// --------------
// 기본 기능 함수
// --------------

// 클릭 좌표 참거짓 함수
bool isInside(int clickX, int clickY, int left, int top, int right, int bottom)
{
    return (clickX >= left && clickX <= right && clickY >= top && clickY <= bottom);
}  

// 좌표 지정 함수
void gotoxy(int x, int y) 
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 
static void trim(char* s) {
    if (!s) return;
    // left trim
    char* p = s;
    while (*p && isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);
    // right trim
    size_t n = strlen(s);
    while (n > 0 && isspace((unsigned char)s[n - 1])) s[--n] = '\0';
}

// 공백 텍스트 제거 함수
void trimAll(char* str) 
{
    char* p = str;
    char* q = str;
    while (*q) 
    {
        if (*q != '\n' && *q != '\r' && *q != ' ' && *q != '\t')
            *p++ = *q;
        q++;
    }
    *p = '\0';
}

static void trimEnds(char* s) {
    if (!s) return;
    while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') memmove(s, s + 1, strlen(s));
    char* p = s + strlen(s) - 1;
    while (p >= s && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')) *p-- = '\0';
}

static void stripBOM(char* s) {
    if (!s) return;
    unsigned char* u = (unsigned char*)s;
    if (u[0] == 0xEF && u[1] == 0xBB && u[2] == 0xBF) memmove(s, s + 3, strlen(s + 3) + 1);
}

static int equalsToken(const char* a, const char* b) {
    char aa[128] = { 0 }, bb[128] = { 0 };
    snprintf(aa, sizeof(aa), "%s", a ? a : "");
    snprintf(bb, sizeof(bb), "%s", b ? b : "");
    stripBOM(aa); stripBOM(bb);
    trimEnds(aa); trimEnds(bb);
    return strcmp(aa, bb) == 0;
}

// 색깔 설정 함수
void setColor(WORD color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 랜덤 색깔 지정 함수
int getRandomColor() 
{
    
}

// 박스 그리기 함수
void drawBox(int x, int y, int width, int height)
{
    // 윗줄
    gotoxy(x, y);
    printf("┌");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┐");

    // 중간줄
    for (int i = 0; i < height - 2; i++)
    {
        gotoxy(x, y + 1 + i);
        printf("│");
        for (int j = 0; j < width - 2; j++) printf(" ");
        printf("│");
    }

    // 아랫줄
    gotoxy(x, y + height - 1);
    printf("└");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("┘");
}

// 텍스트 입력 함수
void getTextInput(int x, int y, char* buffer, int maxLen)
{
    int i = 0;
    char ch;

    gotoxy(x, y);  // 입력 시작 위치로 커서 이동

    while (1)
    {
        ch = _getch();

        if (ch == 13)  // Enter
        {
            buffer[i] = '\0';
            break;
        }
        else if (ch == 8 && i > 0)  // Backspace
        {
            i--;
            buffer[i] = '\0';
            gotoxy(x + i, y);
            printf(" ");
            gotoxy(x + i, y);
        }
        else if (i < maxLen && ch >= 32 && ch <= 126)  // 입력 가능한 문자
        {
            buffer[i++] = ch;
            gotoxy(x + i - 1, y);
            printf("%c", ch);
        }
    }
    buffer[i] = '\0';
}

// ------------------------------------------
// 메인 활용 기능 부가 함수 (시간표 부가 함수)
// ------------------------------------------


// ----------------------
// 활용 기능 함수 (중기능)
// ----------------------

// 로그인 함수
void handleLoginScreen(char* userName, char* userID, char* userPW)
{
    system("cls");
    drawBox(4, 1, 92, 47);

    gotoxy(38, 4);  printf("===== 로그인 창 =====");

    // 아이디 입력창
    gotoxy(31, 9);  printf("아이디 : ");
    drawBox(31, 11, 35, 3);  

    // 비밀번호 입력창
    gotoxy(31, 15); printf("비밀번호 : ");
    drawBox(31, 17, 35, 3); 

    // 버튼
    gotoxy(33, 24); printf("[  로그인  ]");
    gotoxy(50, 24); printf("[ 회원가입 ]");

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            // 커서가 위치할 때
            if (mouse.dwEventFlags == MOUSE_MOVED)
            {
                // 로그인 커서
                if (isInside(clickX, clickY, 33, 24, 44, 24))
                {
                    setColor(13);
                    gotoxy(33, 24); printf("[  로그인  ]");
                }
                else
                {
                    setColor(15); // 흰색
                    gotoxy(33, 24); printf("[  로그인  ]");
                }

                // 회원가입 커서
                if (isInside(clickX, clickY, 50, 24, 61, 24))
                {
                    setColor(13);
                    gotoxy(50, 24); printf("[ 회원가입 ]");
                }
                else
                {
                    setColor(15);
                    gotoxy(50, 24); printf("[ 회원가입 ]");
                }

                setColor(15); // 기본 색상 복원
            }

            // 클릭을 했을 때
            if ((mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && mouse.dwEventFlags == 0)
            {
                // [회원가입] 버튼 
                if (isInside(clickX, clickY, 50, 24, 61, 24))
                {
                    handleSignupScreen(userName, userID, userPW);
                    break;
                }

                // [로그인] 버튼 
                else if (isInside(clickX, clickY, 33, 24, 44, 24))
                {
                    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Member.txt", "r");

                    char line[200];
                    Member loginUser = { 0 };
                    int isMatch = 0;

                    if (!file)
                    {
                        gotoxy(31, 28);
                        printf("파일을 열 수 없습니다.");
                        continue;
                    }

                    while (fgets(line, sizeof(line), file))
                    {
                        if (sscanf(line, "userNAME: %49[^/]/ userID: %49[^/]/ userPW: %49[^\n]",
                            loginUser.userName, loginUser.userID, loginUser.userPW) == 3)
                        {
                            trimAll(loginUser.userID);
                            trimAll(loginUser.userPW);

                            if (strcmp(loginUser.userID, userID) == 0 && strcmp(loginUser.userPW, userPW) == 0)
                            {
                                strncpy(userName, loginUser.userName, sizeof(userName) - 1);
                                userName[sizeof(userName) - 1] = '\0';

                                strncpy(userID, loginUser.userID, sizeof(userID) - 1);
                                userID[sizeof(userID) - 1] = '\0';

                                strncpy(userPW, loginUser.userPW, sizeof(userPW) - 1);
                                userPW[sizeof(userPW) - 1] = '\0';

                                gotoxy(31, 28);
                                printf(" 【  회원가입 성공!, 환영합니다, %s님  】 ", userName);
                                Sleep(1000);
                                fclose(file);
                                system("cls");
                                handleMainMenuScreen(userName, userID);
                                return;
                            }
                        }
                    }
                    fclose(file);
                    gotoxy(31, 28);
                    printf(" 로그인 실패! 아이디 또는 비밀번호 확인");
                }

                // ID 입력 박스 (좌표: 31~65, 11~13)
                else if (isInside(clickX, clickY, 31, 11, 65, 13))
                {
                    gotoxy(32, 12);
                    printf("                                ");
                    gotoxy(32, 12);
                    getTextInput(32, 12, userID, 49);
                }

                // PW 입력 박스 (좌표: 31~65, 17~19)
                else if (isInside(clickX, clickY, 31, 17, 65, 19))
                {
                    gotoxy(32, 18);
                    printf("                                ");
                    gotoxy(32, 18);
                    getTextInput(32, 18, userPW, 49);
                }
            }
        }
    }
}

// 회원가입 함수
void handleSignupScreen()
{
    system("cls");

    Member user;  // 구조체 사용

    drawBox(4, 1, 92, 47);

    gotoxy(38, 4);  printf("===== 회원가입 창 =====");

    // 입력창
    gotoxy(31, 9);  printf("이름: ");
    drawBox(31, 11, 30, 3);

    gotoxy(31, 15); printf("아이디: ");
    drawBox(31, 17, 30, 3);

    gotoxy(31, 21); printf("비밀번호: ");
    drawBox(31, 23, 30, 3);

    gotoxy(33, 30); printf("[  확인  ]");
    gotoxy(50, 30); printf("[ 로그인 화면 ]");

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            if (mouse.dwEventFlags == MOUSE_MOVED)
            {
                // 확인 버튼 hover
                if (isInside(clickX, clickY, 33, 30, 41, 30))
                {
                    setColor(13);
                    gotoxy(33, 30); printf("[  확인  ]");
                }
                else
                {
                    setColor(15); 
                    gotoxy(33, 30); printf("[  확인  ]");
                }

                // 로그인 화면 버튼 hover
                if (isInside(clickX, clickY, 50, 30, 65, 30))
                {
                    setColor(13);
                    gotoxy(50, 30); printf("[ 로그인 화면 ]");
                }
                else
                {
                    setColor(15);
                    gotoxy(50, 30); printf("[ 로그인 화면 ]");
                }

                setColor(15); // 기본 색상 복원
            }

            if ((mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && mouse.dwEventFlags == 0)
            {
                // 이름 입력 박스
                if (isInside(clickX, clickY, 31, 11, 61, 13))
                {
                    gotoxy(32, 12);
                    printf("                         ");
                    gotoxy(32, 12);
                    getTextInput(32, 12, user.userName, sizeof(user.userName) - 1);
                }
                // 아이디 입력 박스
                else if (isInside(clickX, clickY, 31, 17, 61, 19))
                {
                    gotoxy(32, 18);
                    printf("                         ");
                    gotoxy(32, 18);
                    getTextInput(32, 18, user.userID, sizeof(user.userID) - 1);
                }
                // 비밀번호 입력 박스
                else if (isInside(clickX, clickY, 31, 23, 61, 25))
                {
                    gotoxy(32, 24);
                    printf("                         ");
                    gotoxy(32, 24);
                    getTextInput(32, 24, user.userPW, sizeof(user.userPW) - 1);
                }
                // 확인 버튼 클릭
                else if (isInside(clickX, clickY, 33, 30, 41, 30))
                {
                    // 입력값 검증
                    if (strlen(user.userName) == 0 || strlen(user.userID) == 0 || strlen(user.userPW) == 0)
                    {
                        gotoxy(33, 32);
                        printf("회원가입 실패! (모든 칸을 입력하세요)");
                        Sleep(1500);
                        gotoxy(33, 32);
                        printf("                                     "); // 메시지 지우기
                        continue; // 다시 입력 대기
                    }

                    // 중복 ID 검사
                    int isDuplicate = 0;
                    FILE* checkFile = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Member.txt", "r");
                    char line[150], existingID[50];

                    if (checkFile)
                    {
                        while (fgets(line, sizeof(line), checkFile))
                        {
                            if (sscanf(line, "userNAME: %*[^/] / userID: %49s", existingID) == 1)
                            {
                                if (strcmp(existingID, user.userID) == 0)
                                {
                                    isDuplicate = 1;
                                    break;
                                }
                            }
                        }
                        fclose(checkFile);
                    }

                    if (isDuplicate)
                    {
                        gotoxy(33, 32);
                        printf("회원가입 실패! (이미 존재하는 아이디)");
                        Sleep(1500);
                        gotoxy(33, 32);
                        printf("                                     ");
                        continue;
                    }

                    // 신규 계정 저장
                    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Member.txt", "a");
                    if (file)
                    {
                        fprintf(file, "userNAME: %s / userID: %s / userPW: %s\n",
                            user.userName, user.userID, user.userPW);
                        fclose(file);

                        gotoxy(33, 32);
                        printf(" 【  회원가입 성공!  】 ");
                        Sleep(1000);

                        SetConsoleMode(hInput, prevMode);
                        system("cls");
                        handleLoginScreen(user.userName, user.userID, user.userPW);
                        return;
                    }
                    else
                    {
                        gotoxy(33, 32);
                        printf("파일 오류! 회원가입 실패");
                        Sleep(1500);
                    }
                }
                // 로그인 화면 버튼 클릭
                else if (isInside(clickX, clickY, 50, 30, 65, 30))
                {
                    SetConsoleMode(hInput, prevMode);
                    system("cls");
                    handleLoginScreen(user.userName, user.userID, user.userPW);
                    return;
                }
            }
        }
    }


    // 기존 콘솔 모드 복원
    SetConsoleMode(hInput, prevMode);

    // 회원가입 후 로그인 화면으로 이동
    system("cls");
    handleLoginScreen(user.userName, user.userID, user.userPW);
}

// 메인메뉴 함수
void handleMainMenuScreen(char* userName, char* userID)
{
    system("cls");

    drawBox(4, 1, 92, 47);
    gotoxy(32, 4);
    printf("===== 메인 메뉴 =====");

    gotoxy(36, 6);
    printf("환영합니다!");

    drawBox(27, 9, 30, 3);
    gotoxy(29, 10);  printf("1. 나의 시간표 보기 ");

    drawBox(27, 13, 30, 3);
    gotoxy(29, 14);  printf("2. 친구 목록 보기 ");

    drawBox(27, 17, 30, 3);
    gotoxy(29, 18);  printf("3. 로그아웃 ");

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            if (mouse.dwEventFlags == MOUSE_MOVED)
            {
                // 나의 시간표 보기
                if (isInside(clickX, clickY, 27, 9, 56, 11))
                {
                    setColor(13);
                    drawBox(27, 9, 30, 3);
                    gotoxy(29, 10);  printf("1. 나의 시간표 보기 ");
                }
                else
                {
                    setColor(15);
                    drawBox(27, 9, 30, 3);
                    gotoxy(29, 10);  printf("1. 나의 시간표 보기 ");
                }

                // 친구 목록 보기
                if (isInside(clickX, clickY, 27, 13, 56, 15))
                {
                    setColor(13);
                    drawBox(27, 13, 30, 3);
                    gotoxy(29, 14);  printf("2. 친구 목록 보기 ");
                }
                else
                {
                    setColor(15);
                    drawBox(27, 13, 30, 3);
                    gotoxy(29, 14);  printf("2. 친구 목록 보기 ");
                }

                // 로그아웃
                if (isInside(clickX, clickY, 27, 17, 56, 19))
                {
                    setColor(13);
                    drawBox(27, 17, 30, 3);
                    gotoxy(29, 18);  printf("3. 로그아웃 ");
                }
                else
                {
                    setColor(15);
                    drawBox(27, 17, 30, 3);
                    gotoxy(29, 18);  printf("3. 로그아웃 ");
                }

                setColor(15); // 기본 색상 복원
            }

            if (mouse.dwEventFlags == 0 && (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
            {
                if (isInside(clickX, clickY, 27, 9, 56, 11))  
                {
                    handleTimetableScreen(userID, userName);
                }
                else if (isInside(clickX, clickY, 27, 13, 56, 15))  
                {
                    handleFriendsListScreen(userName, userID, userPW);
                }
                else if (isInside(clickX, clickY, 27, 17, 56, 19)) 
                {
                    handleLoginScreen(userName, userID, userPW);
                    break;
                }
            }
        }
    }
}

// 친구 목록 함수
void handleFriendsListScreen(char* userName, char* userID, char* userPW)
{
    system("cls");
    drawBox(4, 1, 92, 47);

    gotoxy(32, 4);
    printf("===== 친구 목록 =====");

    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Friends.txt", "r");
    if (!file)
    {
        gotoxy(30, 10);
        printf(" 친구 목록을 불러올 수 없습니다.");
        _getch();
        return;
    }

    Friends friendData;
    char line[256];
    int yPos = 8;
    int friendCount = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line,
            "userID: %49[^/] / friendName: %49[^/] / friendID: %49[^/] / friendPin: %9s",
            friendData.userID, friendData.friendName, friendData.friendID, friendData.friendPin) == 4)
        {
            trimAll(friendData.userID);
            trimAll(friendData.friendName);
            trimAll(friendData.friendID);

            if (strcmp(friendData.userID, userID) == 0)
            {
                gotoxy(20, yPos);
                printf("친구 이름 : %-12s ( ID : %-12s)", friendData.friendName, friendData.friendID);
                yPos += 2;
                friendCount++;
            }
        }
    }
    fclose(file);

    if (friendCount == 0)
    {
        gotoxy(30, 10);
        printf("친구가 없습니다.");
    }

    // 버튼 표시
    gotoxy(27, 30);
    printf("[ 친구 추가 ]");

    gotoxy(40, 30);
    printf("[ 즐겨찾기 ]");

    gotoxy(53, 30);
    printf("[ 뒤로가기 ]");

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD m = record.Event.MouseEvent;

            if (m.dwEventFlags == 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
            {
                MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
                int clickX = mouse.dwMousePosition.X;
                int clickY = mouse.dwMousePosition.Y;

                // [ 친구 추가 ] 클릭
                if (isInside(clickX, clickY, 27, 30, 38, 30))
                {
                    handleAddFriendScreen(userName, userID);
                    system("cls");
                    handleFriendsListScreen(userID, userName, userPW);
                    break;
                }

                /*
                // [ 즐겨찾기 ] 클릭
                else if (isInside(clickX, clickY, 40, 30, 50, 30))
                {
                    handleFriendPin(userID); // 즐겨찾기 설정 화면 진입
                    system("cls");
                    handleFriendsListScreen(userID, userName, userPW); // 목록 다시 출력
                    break;
                }
                */

                // [ 뒤로가기 ] 클릭
                else if (isInside(clickX, clickY, 53, 30, 63, 30))
                {
                    handleMainMenuScreen(userName, userID);
                    break;
                }
            }
        }
    }
}

// 친구 추가 함수
void handleAddFriendScreen(const char* userName, const char* userID)
{
    system("cls");
    drawBox(4, 1, 92, 47);

    Friends newFriend;
    int isAlreadyFriend = 0;

    gotoxy(32, 4);
    printf("===== 친구 추가 =====");

    // 입력창
    gotoxy(31, 9);  printf("친구 이름 : ");
    drawBox(31, 11, 30, 3);

    gotoxy(31, 15); printf("친구 아이디 : ");
    drawBox(31, 17, 30, 3);

    gotoxy(33, 30); printf("[ 친구 추가 ]");
    gotoxy(50, 30); printf("[ 친구 목록 ]");

    trimAll(newFriend.friendName);
    trimAll(newFriend.friendID);

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            if (mouse.dwEventFlags == MOUSE_MOVED)
            {
                // 친구 추가 버튼 hover
                if (isInside(clickX, clickY, 33, 30, 41, 30))
                {
                    setColor(10); // 초록색
                    gotoxy(33, 30); printf("[ 친구 추가 ]");
                }
                else
                {
                    setColor(15); // 흰색
                    gotoxy(33, 30); printf("[ 친구 추가 ]");
                }

                // 친구 목록 화면 이동 버튼 hover
                if (isInside(clickX, clickY, 50, 30, 65, 30))
                {
                    setColor(10);
                    gotoxy(50, 30); printf("[ 친구 목록 ]");
                    return;
                }
                else
                {
                    setColor(15);
                    gotoxy(50, 30); printf("[ 친구 목록 ]");
                }

                setColor(15); // 기본 색상 복원
            }

            if ((mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && mouse.dwEventFlags == 0)
            {
                // 친구 이름 입력 박스
                if (isInside(clickX, clickY, 31, 11, 61, 13))
                {
                    gotoxy(32, 12);
                    printf("                         ");
                    gotoxy(32, 12);
                    getTextInput(32, 12, newFriend.friendName, sizeof(newFriend.friendName) - 1);
                }
                // 친구 아이디 입력 박스
                else if (isInside(clickX, clickY, 31, 17, 61, 19))
                {
                    gotoxy(32, 18);
                    printf("                         ");
                    gotoxy(32, 18);
                    getTextInput(32, 18, newFriend.friendID, sizeof(newFriend.friendID) - 1);
                }
                // 확인 버튼 클릭
                else if (isInside(clickX, clickY, 33, 30, 42, 30))
                {
                    // 입력값 검증
                    if (strlen(newFriend.friendName) == 0 || strlen(newFriend.friendID) == 0)
                    {
                        gotoxy(33, 32);
                        printf("친구 추가 실패! (모든 칸을 입력하세요)");
                        Sleep(1500);
                        gotoxy(33, 32);
                        printf("                                     "); // 메시지 지우기
                        continue; // 다시 입력 대기
                    }

                    // 중복 친구 여부 확인
                    FILE* checkFile = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Friends.txt", "r");
                    char line[256];
                    Friends friendData;

                    if (checkFile)
                    {
                        while (fgets(line, sizeof(line), checkFile))
                        {
                            if (sscanf(line, "userID: %49[^/] / friendName: %49[^/] / friendID: %49[^/] / friendPin: %9s",
                                friendData.userID, friendData.friendName, friendData.friendID, friendData.friendPin) == 4)
                            {
                                trimAll(friendData.userID);
                                trimAll(friendData.friendID);

                                if (strcmp(friendData.userID, userID) == 0 &&
                                    strcmp(friendData.friendID, newFriend.friendID) == 0)
                                {
                                    isAlreadyFriend = 1;
                                    break;
                                }
                            }
                        }
                        fclose(checkFile);
                    }

                    // 결과 처리
                    if (isAlreadyFriend)
                    {
                        gotoxy(28, 23);
                        printf(" 이미 등록된 친구입니다.");
                    }
                    else
                    {
                        FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Friends.txt", "a");
                        if (file)
                        {
                            fprintf(file, "userID: %s / friendName: %s / friendID: %s / friendPin: 0\n",
                                userID, newFriend.friendName, newFriend.friendID);
                            fclose(file);
                        }
                        gotoxy(28, 23);
                        printf(" 친구가 성공적으로 추가되었습니다!");
                        handleFriendsListScreen(userID, userName, userPW);
                    }
                }
                // 친구 목록 화면 이동 
                else if (isInside(clickX, clickY, 50, 30, 59, 30))
                {
                    SetConsoleMode(hInput, prevMode);
                    system("cls");
                    handleFriendsListScreen(userID, userName, userPW);
                    return;
                }
            }
        }
    }
    // 기존 콘솔 모드 복원
    SetConsoleMode(hInput, prevMode);
}

/*
// 즐겨찾기 함수
void handleFriendPin()
{

}
*/

// 요일 함수
static int getDayIndexFlexible(const char* w) {
    if (!w) return -1;
    // 소문자/공백 제거
    char buf[32]; int j = 0;
    for (int i = 0; w[i] && j < 31; i++) if (!isspace((unsigned char)w[i])) buf[j++] = tolower((unsigned char)w[i]);
    buf[j] = '\0';

    // 한글 전체/축약
    if (strstr(buf, "월")) return 0;
    if (strstr(buf, "화")) return 1;
    if (strstr(buf, "수")) return 2;
    if (strstr(buf, "목")) return 3;
    if (strstr(buf, "금")) return 4;

    // 영문 요일(축약 포함, 마침표 허용)
    if (strstr(buf, "mon")) return 0;
    if (strstr(buf, "tue")) return 1;
    if (strstr(buf, "wed")) return 2;
    if (strstr(buf, "thu")) return 3;
    if (strstr(buf, "fri")) return 4;

    return -1;
}

// 시간 함수
static int parseTimeRange(const char* s, int* outStartHour, int* outEndHour) {
    if (!s || !outStartHour || !outEndHour) return 0;

    // 숫자만 뽑아 H1, m1, H2, m2 순으로 잡기
    int nums[4] = { 0,0,0,0 }, k = 0, cur = 0, inNum = 0;
    for (const char* p = s; *p && k < 4; p++) {
        if (isdigit((unsigned char)*p)) {
            cur = cur * 10 + (*p - '0'); inNum = 1;
        }
        else {
            if (inNum) { nums[k++] = cur; cur = 0; inNum = 0; }
        }
    }
    if (inNum && k < 4) nums[k++] = cur;

    int sh = 0, sm = 0, eh = 0, em = 0;
    if (k == 2) { sh = nums[0]; eh = nums[1]; sm = 0; em = 0; }
    else if (k >= 3) { sh = nums[0]; sm = nums[1]; eh = nums[2]; em = (k >= 4 ? nums[3] : 0); }
    else return 0;

    // 24h 보정, 종료 올림
    if (sh < 0 || sh>24 || eh < 0 || eh>24) return 0;
    int startH = sh;
    int endH = (em > 0 ? eh + 1 : eh);
    if (endH <= startH) endH = startH + 1;

    *outStartHour = startH;
    *outEndHour = endH;
    return 1;
}

// 성공 시 outCodes에 "101,203,405" 같은 원본 문자열을 그대로 채워줌
static int readUserCodesFromTimetable(const char* timetablePath,
    const char* userID,
    char* outCodes, size_t outSize)
{
    FILE* fp = fopen(timetablePath, "r");
    if (!fp) {
        // 절대경로 실패 표시
        gotoxy(5, 43); printf("open fail: %s                                      ", timetablePath);

        // ① 상대경로로도 한번 더 시도 (exe 실행 폴더)
        fp = fopen("Timetable.txt", "r");
        if (fp) {
            gotoxy(5, 43); printf("fallback open OK: Timetable.txt                     ");
        }
        else {
            gotoxy(5, 43); printf("fallback open FAIL: Timetable.txt                   ");
            return 0;
        }
    }
    else {
        gotoxy(5, 43); printf("open OK: %s                                          ", timetablePath);
    }

    char line[512];
    int ok = 0;

    while (fgets(line, sizeof(line), fp)) {
        char fileUser[64] = { 0 };
        char codes[256] = { 0 };

        // 현재 읽은 줄(앞부분) 확인용
        gotoxy(5, 44); printf("LINE: %.60s                                         ", line);

        if (sscanf(line, "userID: %63[^/] / subjectCODE: %255[^\n]", fileUser, codes) == 2) {
            trim(fileUser); trim(codes);

            // 파싱 결과 및 비교 표시
            gotoxy(5, 45); printf("PARSED user=[%s] codes=[%s]                         ", fileUser, codes);
            gotoxy(5, 46); printf("COMPARE with userID=[%s] -> %s                      ",
                userID, (strcmp(fileUser, userID) == 0 ? "MATCH" : "NO"));

            if (strcmp(fileUser, userID) == 0) {
                strncpy(outCodes, codes, outSize - 1);
                outCodes[outSize - 1] = '\0';
                ok = 1;
                break;
            }
        }
    }
    fclose(fp);
    return ok;
}


/* ========== 1.5단계: "101,203,405" → ["101","203","405"] ========== */
// 문자열 배열로 유지(앞의 0 보존). 반환: 코드 개수
static int splitCodes(const char* codesCsv, char codes[][16], int maxCodes) {
    if (!codesCsv) return 0;
    char buf[512]; strncpy(buf, codesCsv, sizeof(buf) - 1); buf[sizeof(buf) - 1] = '\0';

    int count = 0;
    char* tok = strtok(buf, ",");
    while (tok && count < maxCodes) {
        // 앞뒤 공백 + 제어문자 제거
        while (*tok && isspace((unsigned char)*tok)) tok++;
        char* e = tok + strlen(tok);
        while (e > tok && isspace((unsigned char)e[-1])) *--e = '\0';

        strncpy(codes[count], tok, 15);
        codes[count][15] = '\0';
        count++;
        tok = strtok(NULL, ",");
    }
    return count;
}

// 2단계: Subject.txt 순회하며 코드 매칭 후 화면에 배치 ==========
static void placeSubjectOnGrid(int startX, int startY, int cellWidth, int cellHeight, int dayIdx, int startHour, int endHour, const char* subjectName, int occupied[5][11])
{
    if (dayIdx < 0 || dayIdx > 4) return;
    int hourIdx = startHour - 9;
    int duration = endHour - startHour;
    if (hourIdx < 0 || hourIdx >= 11) return;
    if (duration <= 0) duration = 1;
    if (hourIdx + duration > 11) duration = 11 - hourIdx;

    // 중복 칸 체크
    for (int h = hourIdx; h < hourIdx + duration; h++) {
        if (occupied[dayIdx][h]) return; // 이미 차 있으면 그리지 않음 (정책 유지)
    }

    int boxY = startY + hourIdx * cellHeight;
    int boxHeight = cellHeight * duration;

    int randColor = getRandomColor();
    setColor(randColor);
    drawBox(startX + dayIdx * cellWidth, boxY, cellWidth, boxHeight);
    gotoxy(startX + dayIdx * cellWidth + 1, boxY + 1);
    printf("%s", subjectName);
    setColor(7);

    for (int h = hourIdx; h < hourIdx + duration; h++) {
        occupied[dayIdx][h] = 1;
    }
}

static int codeEq(const char* a, const char* b) {
    if (strcmp(a, b) == 0) return 1;
    // 숫자 비교(선행 0 무시)
    char* end1, * end2;
    long va = strtol(a, &end1, 10);
    long vb = strtol(b, &end2, 10);
    if (*end1 == '\0' && *end2 == '\0' && va == vb) return 1;
    return 0;
}

static int codeInUserList(const char* code, char codes[][16], int codeCount) {
    for (int i = 0; i < codeCount; i++) {
        if (codeEq(code, codes[i])) return 1;
    }
    return 0;
}

/* ========== 메인 시간표 화면 로직 ========== */
void handleTimetableScreen(char* userName, char* userID)
{
    system("cls");
    drawBox(4, 1, 92, 47);

    int cellWidth = 15;
    int cellHeight = 3;
    int dayCellHeight = 3;
    int timeCellWidth = 7;

    int startX = 16;
    int startY = 7;

    const char* days[5] = { "월", "화", "수", "목", "금" };

    // 화면 제목
    gotoxy(39, 1);
    printf(" %s님의 시간표", userName);

    gotoxy(28, 45); printf("[ 수업추가 ]");
    gotoxy(42, 45); printf("[ 수업삭제 ]");
    gotoxy(56, 45); printf("[ 메인메뉴 ]");

    // 요일 헤더
    for (int d = 0; d < 5; d++) {
        int x = startX + d * cellWidth;
        int y = startY - dayCellHeight;
        drawBox(x, y, cellWidth, dayCellHeight);
        gotoxy(x + (cellWidth / 2) - 1, y + 1);
        printf("%s", days[d]);
    }

    // 시간 라벨
    for (int h = 0; h < 11; h++) {
        int posY = startY + h * cellHeight;
        drawBox(startX - timeCellWidth - 1, posY, timeCellWidth, cellHeight);
        gotoxy(startX - timeCellWidth + 1, posY + (cellHeight / 2));
        printf("%02d시", 9 + h);
    }

    // 그리드
    for (int row = 0; row < 11; row++) 
    {
        int posY = startY + row * cellHeight;
        for (int col = 0; col < 5; col++) 
        {
            drawBox(startX + col * cellWidth, posY, cellWidth, cellHeight);
        }
    }

    /* ===== 1단계: Timetable.txt에서 사용자 과목 코드 읽기 ===== */
    char codesCsv[256] = { 0 };
    if (!readUserCodesFromTimetable("C:\\Users\\Koomy\\source\\repos\\project01\\Timetable.txt",
        userID, codesCsv, sizeof(codesCsv))) 
    {
        gotoxy(28, 45); printf("시간표 데이터(Timetable.txt)를 열거나 찾을 수 없습니다.");
        getchar();
        return;
    }

    if (strlen(codesCsv) == 0) {
        gotoxy(28, 45); printf("시간표 데이터가 없습니다.");
        getchar();
        return;
    }

    char userCodes[64][16]; // 최대 64개 과목 코드 가정
    int  userCodeCount = splitCodes(codesCsv, userCodes, 64);

    gotoxy(5, 43);
    printf("codesCsv:[%s]  count=%d                                   ",
        codesCsv, userCodeCount);

    /* ===== 2단계: Subject.txt에서 매칭 → 시간표 출력 ===== */
    int occupied[5][11] = { 0 };
    int placedCount = 0;

    FILE* fp = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Subject.txt", "r");
    if (!fp) {
        gotoxy(28, 45); printf("과목 데이터(Subject.txt)를 열 수 없습니다.");
        getchar();
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        SubjectList sub = { 0 };

        // 예시 포맷:
        // subjectCODE: 101 / subjectNAME: 자료구조 / professorNAME: 홍길동 / subjectCATE: 전공 / WEEK: 월 / TIME: 09 : 00 ~ 11 : 00
        if (sscanf(line,
            "subjectCODE: %9[^/] / subjectNAME: %49[^/] / professorNAME: %49[^/] / subjectCATE: %19[^/] / WEEK: %19[^/] / TIME: %49[^\n]",
            sub.subjectCode, sub.subjectName, sub.professorName, sub.subCategory, sub.week, sub.time) == 6) {

            trim(sub.subjectCode); trim(sub.subjectName);
            trim(sub.professorName); trim(sub.subCategory);
            trim(sub.week); trim(sub.time);
            // ▼ [B] 읽힌 값 가시화
            gotoxy(5, 44);
            printf("READ subCODE=%s  WEEK=%s  TIME=%s                          ",
                sub.subjectCode, sub.week, sub.time);

            // [C] === 사용자 코드 매칭 결과 확인 ===
        // 위치: 위 trim(...) '바로 뒤' / 요일/시간 파싱 '하기 전'
            int matched = codeInUserList(sub.subjectCode, userCodes, userCodeCount);
            gotoxy(5, 45);
            printf("match=%d for subCODE=%s                                    ", matched, sub.subjectCode);
            if (!matched) continue;

            // [D] === 요일/시간 파싱 체크 ===
        // 위치: [C] 이후, placeSubjectOnGrid 호출 '전'
            int dayIdx = getDayIndexFlexible(sub.week);
            if (dayIdx < 0) {
                gotoxy(5, 46);
                printf("요일 해석 실패: [%s]                                       ", sub.week);
                continue;
            }

            int sh = 0, eh = 0;
            if (!parseTimeRange(sub.time, &sh, &eh)) {
                gotoxy(5, 46);
                printf("시간 해석 실패: [%s]                                       ", sub.time);
                continue;
            }

            // [E] === 실제 배치 직전 정보 찍기 ===
        // 위치: placeSubjectOnGrid(...) 호출 '직전'
            int hourIdx = (sh < 9) ? 0 : (sh - 9);   // 9시 이전은 임시로 0칸부터
            int duration = (eh - sh);
            gotoxy(5, 46);
            printf("place: day=%d hourIdx=%d dur=%d name=%s                    ",
                dayIdx, hourIdx, duration, sub.subjectName);

            placeSubjectOnGrid(startX, startY, cellWidth, cellHeight,
                dayIdx, sh, eh, sub.subjectName, occupied);
            placedCount++;

            // 사용자 과목 코드 목록에 포함되는지 (문자열 비교! 001/002도 정확히 매칭)
            if (!codeInUserList(sub.subjectCode, userCodes, userCodeCount)) continue;

            if (!parseTimeRange(sub.time, &sh, &eh)) continue;

            // 실제 그리기
            placeSubjectOnGrid(startX, startY, cellWidth, cellHeight,
                dayIdx, sh, eh, sub.subjectName, occupied);
        }
    }
    fclose(fp);

    // 위치: Subject.txt 닫은 '직후', 마우스 입력 루프 시작 '이전'
    if (placedCount == 0) {
        gotoxy(5, 43);
        printf("⚠ 표시된 과목이 없습니다. (코드/요일/시간/구간 불일치 의심)     ");
    }

    /* ===== 마우스 입력 처리 ===== */
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD record;
    DWORD events;

    while (1) {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            if ((mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && mouse.dwEventFlags == 0) {
                // 수업추가
                /*
                if (isInside(clickX, clickY, 28, 45, 28 + 7, 45)) 
                {
                    handleSubjectList(userID);
                }
                */
                // 수업삭제 (구현 예정 자리)
                if (isInside(clickX, clickY, 42, 45, 42 + 7, 45)) 
                {
                    // TODO: 수업삭제 UI/로직
                }
                // 메인메뉴
                else if (isInside(clickX, clickY, 56, 45, 56 + 7, 45)) {
                    handleMainMenuScreen(userName, userID);
                    break;
                }
            }
        }
    }
}