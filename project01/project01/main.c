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


// 함수 전방 선언
bool isInside(int x, int y, int left, int top, int right, int bottom);

void gotoxy(int x, int y);
void trimAll(char* str);
void setColor(WORD color);
int getRandomColor();
void drawBox(int x, int y, int width, int height);
void getTextInput(int x, int y, char* buffer, int maxLen);

void handleLoginScreen(const char* userName, const char* userID, const char* userPW);
void handleSignupScreen(const char* userName, const char* userID, const char* userPW);

void handleMainMenuScreen(const char* userName, const char* userID, const char* userPW);

int getDayIndex(const char* day);
int getStartHour(const char* timeStr);

void handleTimetableScreen(const char* userID, const char* userName);
void handleSubjectList(const char* userID);

void handleFriendsListScreen(const char* userID);
void handleAddFriendScreen(const char* friendID);
void handleFriendTimetableScreen(const char* userName, const char* userID, const char* friendID, const char* friendName);

// 전역 변수
char userName[50] = { 0 };
char userID[50] = { 0 };
char userPW[50] = { 0 };

char friendID[20] = { 0 };
char friendName[20] = { 0 };

char subjectCode[20] = { 0 };

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

// 요일 함수
int getDayIndex(const char* day)
{
    if (strstr(day, "월")) return 0;
    if (strstr(day, "화")) return 1;
    if (strstr(day, "수")) return 2;
    if (strstr(day, "목")) return 3;
    if (strstr(day, "금")) return 4;
    return -1;
}

// 시간 함수
int getStartHour(const char* timeStr)
{
    int hour = 0;
    sscanf(timeStr, " %d", &hour); // 첫 숫자만 추출
    return hour;
}

// --------------
// 활용 기능 함수 (중기능)
// --------------

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

    DWORD lastBtnState = 0;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;

            if ((mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) && mouse.dwEventFlags == 0)
            {
                int clickX = mouse.dwMousePosition.X;
                int clickY = mouse.dwMousePosition.Y;

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
                    if (!file)
                    {
                        gotoxy(31, 28);
                        printf("파일을 열 수 없습니다.");
                        continue;
                    }

                    char line[200];
                    Member loginUser = { 0 };
                    int isMatch = 0;

                    while (fgets(line, sizeof(line), file))
                    {
                        if (sscanf(line, "userNAME: %49[^/]/ userID: %49[^/]/ userPW: %49[^\n]",
                            loginUser.userName, loginUser.userID, loginUser.userPW) == 3)
                        {
                            trimAll(loginUser.userID);
                            trimAll(loginUser.userPW);

                            if (strcmp(loginUser.userID, userID) == 0 && strcmp(loginUser.userPW, userPW) == 0)
                            {
                                strncpy(userName, loginUser.userName, sizeof(loginUser.userName) - 1);
                                userName[sizeof(loginUser.userName) - 1] = '\0';

                                gotoxy(31, 28);
                                printf(" 【  회원가입 성공!, 환영합니다, %s님  】 ", userName);
                                Sleep(1000);
                                fclose(file);
                                system("cls");
                                handleMainMenuScreen(userName, userID, userPW);
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
void handleSignupScreen(const char* userName, const char* userID, const char* userPW)
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
                    setColor(10); // 초록색
                    gotoxy(33, 30); printf("[  확인  ]");
                }
                else
                {
                    setColor(15); // 흰색
                    gotoxy(33, 30); printf("[  확인  ]");
                }

                // 로그인 화면 버튼 hover
                if (isInside(clickX, clickY, 50, 30, 65, 30))
                {
                    setColor(10);
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
void handleMainMenuScreen(const char* userName, const char* userID, const char* userPW)
{
    system("cls");

    drawBox(4, 1, 92, 47);
    gotoxy(32, 4);
    printf("===== 메인 메뉴 =====");

    gotoxy(36, 6);
    printf(" %s님환영합니다!", userName);

    drawBox(27, 9, 30, 3);
    drawBox(27, 13, 30, 3);
    drawBox(27, 17, 30, 3);

    gotoxy(29, 10);  printf("1. 나의 시간표 보기 ");
    gotoxy(29, 14);  printf("2. 친구 목록 보기 ");
    gotoxy(29, 18);  printf("3. 로그아웃 ");

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    INPUT_RECORD inputRecord;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);

        if (inputRecord.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD m = inputRecord.Event.MouseEvent;

            if (m.dwEventFlags == 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
            {
                int x = m.dwMousePosition.X;
                int y = m.dwMousePosition.Y;

                if (x >= 29 && x <= 57 && y >= 9 && y <= 11)
                {
                    handleTimetableScreen(userID, userName);
                }
                else if (x >= 29 && x <= 57 && y >= 13 && y <= 15)
                {
                    handleFriendsListScreen(userID);
                }
                else if (x >= 29 && x <= 57 && y >= 17 && y <= 19)
                {
                    handleLoginScreen(userName, userID, userPW);
                    break;
                }
            }
        }
    }
}

// 친구 목록 함수
void handleFriendsListScreen(const char* userID)
{
    system("cls");
    drawBox(4, 1, 92, 47);

    // 레이아웃 상수
    const int itemsPerPage = 8;
    const int boxLeft = 20;
    const int boxWidth = 70;
    const int firstTop = 8;
    const int vStep = 4; // 박스 높이(3) + 간격(1)

    Friends friendsData[300];
    int totalFriends = 0;

    // 파일에서 현재 사용자 친구만 수집
    FILE* friendsFile = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Friends.txt", "r");
    if (!friendsFile) {
        gotoxy(30, 10); printf(" 친구 목록을 불러올 수 없습니다.");
        _getch();
        return;
    }

    char line[256];
    Friends tmp;
    while (fgets(line, sizeof(line), friendsFile)) {
        if (sscanf(line,
            "userID: %49[^/] / friendName: %49[^/] / friendID: %49[^/] / friendPin: %9s",
            tmp.userID, tmp.friendName, tmp.friendID, tmp.friendPin) == 4)
        {
            trimAll(tmp.userID);
            trimAll(tmp.friendName);
            trimAll(tmp.friendID);
            trimAll(tmp.friendPin);

            if (strcmp(tmp.userID, userID) == 0) {
                friendsData[totalFriends++] = tmp;
            }
        }
    }
    fclose(friendsFile);

    int page = 1;
    int currentIndex = 0;

    while (1)
    {
        int totalPages = (totalFriends + itemsPerPage - 1) / itemsPerPage;
        if (totalPages == 0) totalPages = 1;

        gotoxy(28, 3);
        printf("===== 친구 목록 (페이지 %d/%d) =====", page, totalPages);

        // 항상 8칸 박스로 출력 (상세 팝업 없음)
        for (int slot = 0; slot < itemsPerPage; ++slot) {
            int i = currentIndex + slot;
            int top = firstTop + slot * vStep;

            drawBox(boxLeft, top, boxWidth, 3);
            gotoxy(boxLeft + 2, top + 1);

            if (i < totalFriends) {
                // 박스 안에 이름/ID 등 핵심 정보만
                printf("이름: %-12s | ID: %-12s", friendsData[i].friendName, friendsData[i].friendID);
            }
            else {
                printf(" - 빈 칸 - ");
            }
        }

        // 버튼 (페이지/추가/뒤로)
        gotoxy(20, 45); printf("[ 이전 페이지 ]");
        gotoxy(40, 45); printf("[ 다음 페이지 ]");
        gotoxy(60, 45); printf("[ 친구 추가 ]");
        gotoxy(75, 45); printf("[ 뒤로가기 ]");

        // 마우스 입력(루프 밖 1회)
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        DWORD prevMode = 0, mode = 0;
        GetConsoleMode(hInput, &prevMode);
        mode = prevMode;
        mode &= ~ENABLE_QUICK_EDIT_MODE;
        mode |= ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT;
        SetConsoleMode(hInput, mode);
        FlushConsoleInputBuffer(hInput);

        INPUT_RECORD rec;
        DWORD events;

        // 입력 대기
        if (!ReadConsoleInput(hInput, &rec, 1, &events)) continue;
        if (rec.EventType != MOUSE_EVENT) continue;

        MOUSE_EVENT_RECORD m = rec.Event.MouseEvent;
        if (m.dwEventFlags != 0) continue; // 이동/휠 등 무시

        if (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) 
        {
            int x = m.dwMousePosition.X;
            int y = m.dwMousePosition.Y;

            for (int slot = 0; slot < itemsPerPage; ++slot)
            {
                int i = currentIndex + slot;
                int top = firstTop + slot * vStep;
                int bottom = top + 2;
                int left = boxLeft;
                int right = boxLeft + boxWidth - 1;

                if (isInside(x, y, left, top, right, bottom) && i < totalFriends)
                {
                    // ★ 친구 시간표 화면으로 이동
                    SetConsoleMode(hInput, prevMode);
                    handleFriendTimetableScreen(userID, userName,   // 내 계정
                        friendsData[i].friendID, friendsData[i].friendName); // 친구
                    return;
                }
            }

            // 이전 페이지
            if (isInside(x, y, 20, 45, 35, 45)) 
            {
                if (currentIndex - itemsPerPage >= 0) 
                {
                    currentIndex -= itemsPerPage; page--;
                }
                continue;
            }
            // 다음 페이지
            if (isInside(x, y, 40, 45, 55, 45)) 
            {
                if (currentIndex + itemsPerPage < totalFriends) 
                {
                    currentIndex += itemsPerPage; page++;
                }
                continue;
            }
            // 친구 추가
            if (isInside(x, y, 60, 45, 72, 45)) 
            {
                SetConsoleMode(hInput, prevMode);
                handleAddFriendScreen(userID);
                return;
            }
            // 뒤로가기
            if (isInside(x, y, 75, 45, 86, 45)) 
            {
                SetConsoleMode(hInput, prevMode);
                handleMainMenuScreen(userName, userID, userPW);
                return;
            }
            // ※ 박스 클릭 시 별도 동작 없음(요청대로)
        }
    }
}

// 친구 추가 함수
void handleAddFriendScreen(const char* userID)
{
    system("cls");

    drawBox(4, 1, 92, 47);

    Friends friendData;

    gotoxy(32, 4);
    printf("===== 친구 추가 =====");

    // 입력창
    gotoxy(31, 9);  printf("친구 이름 : ");
    drawBox(31, 11, 30, 3);

    gotoxy(31, 15); printf("친구 아이디 : ");
    drawBox(31, 17, 30, 3);

    gotoxy(33, 30); printf("[ 친구 추가 ]");
    gotoxy(50, 30); printf("[ 친구 목록 ]");

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
                    getTextInput(32, 12, friendData.friendName, sizeof(friendData.friendName) - 1);
                }
                // 친구 아이디 입력 박스
                else if (isInside(clickX, clickY, 31, 17, 61, 19))
                {
                    gotoxy(32, 18);
                    printf("                         ");
                    gotoxy(32, 18);
                    getTextInput(32, 18, friendData.friendID, sizeof(friendData.friendID) - 1);
                }
                // 확인 버튼 클릭
                else if (isInside(clickX, clickY, 33, 30, 42, 30))
                {
                    // 입력값 검증
                    if (strlen(friendData.friendName) == 0 || strlen(friendData.friendID) == 0)
                    {
                        gotoxy(33, 32);
                        printf("친구 추가 실패! (모든 칸을 입력하세요)");
                        Sleep(1500);
                        gotoxy(33, 32);
                        printf("                                     "); // 메시지 지우기
                        continue; // 다시 입력 대기
                    }

                    // 중복 친구 여부 확인
                    int isAlreadyFriend = 0;

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
                                if (strcmp(friendData.userID, userID) == 0 &&
                                    strcmp(friendData.friendID, friendData.friendID) == 0)
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
                                friendData.userID, friendData.friendName, friendData.friendID);
                            fclose(file);
                        }
                        gotoxy(28, 23);
                        printf(" 친구가 성공적으로 추가되었습니다!");
                    }
                }
                // 친구 목록 화면 이동 
                else if (isInside(clickX, clickY, 50, 30, 59, 30))
                {
                    SetConsoleMode(hInput, prevMode);
                    system("cls");
                    handleFriendsListScreen(userID);
                    return;
                }
            }
        }
    }
    // 기존 콘솔 모드 복원
    SetConsoleMode(hInput, prevMode);
}

// 시간표 함수
void handleTimetableScreen(const char* userID, const char* userName)
{
    system("cls");
    drawBox(4, 1, 92, 47);

    int cellWidth = 15;
    int cellHeight = 3;
    int dayCellHeight = 3;
    int timeCellWidth = 7;

    int startX = 16;
    int startY = 7;

    int deleteMode = 0;

    const char* days[5] = { "월", "화", "수", "목", "금" };

    // 화면 제목
    gotoxy(39, 1);
    printf(" %s님의 시간표", userName);

    gotoxy(28, 45); printf("[ 수업추가 ]");
    gotoxy(42, 45); printf("[ 수업삭제 ]");
    gotoxy(56, 45); printf("[ 메인메뉴 ]");

    // 요일 칸
    for (int d = 0; d < 5; d++)
    {
        int x = startX + d * cellWidth;
        int y = startY - dayCellHeight;
        drawBox(x, y, cellWidth, dayCellHeight);
        gotoxy(x + (cellWidth / 2) - 1, y + 1);
        printf("%s", days[d]);
    }

    // 시간 칸
    for (int h = 0; h < 11; h++)
    {
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

    // ─────────────────────────────────────────────────────────────
    // 수업 코드 읽기
    FILE* TimetableFile = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Timetable.txt", "r");
    if (!TimetableFile)
    {
        gotoxy(28, 45);
        printf("시간표 데이터(Timetable.txt)를 열 수 없습니다.");
        getchar();
        return;
    }

    char line[256], codes[200] = { 0 };
    int found = 0;
    while (fgets(line, sizeof(line), TimetableFile))
    {
        char fileUser[50], tmpCodes[200] = { 0 };
        if (sscanf(line, "userID: %49[^/] / subjectCODE: %[^\n]", fileUser, tmpCodes) == 2) 
        {
            trimAll(fileUser);
            if (strcmp(fileUser, userID) == 0) 
            {
                strncpy(codes, tmpCodes, sizeof(codes) - 1);
                codes[sizeof(codes) - 1] = '\0';
                found = 1;
                break;
            }
        }
    }
    fclose(TimetableFile);

    if (!found || strlen(codes) == 0) {
        gotoxy(28, 45);
        printf("시간표 데이터가 없습니다.");
        getchar();
        return;
    }

    int codesArr[50], codeCount = 0;
    char* token = strtok(codes, ",");
    while (token)
    {
        codesArr[codeCount++] = atoi(token);
        token = strtok(NULL, ",");
    }

    // 칸 사용 여부
    int occupied[5][13] = { 0 };

    // ─────────────────────────────────────────────────────────────
    // 히트박스 구조체/배열 (시간표에 실제로 그린 수업 박스 기록)
    typedef struct 
    {
        int left, top, right, bottom;
        SubjectList sub;
    } SubHitBox;

    SubHitBox subBoxes[128];
    int subBoxCount = 0;

    // 과목 파일 읽고 시간표에 배치 + 히트박스 저장
    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Subject.txt", "r");
    if (!file)
    {
        gotoxy(28, 45);
        printf("과목 데이터(Subject.txt)를 열 수 없습니다.");
        getchar();
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        SubjectList sub;
        if (sscanf(line,
            "subjectCODE: %9[^/] / subjectNAME: %49[^/] / professorNAME: %49[^/] / subjectCATE: %19[^/] / WEEK: %19[^/] / TIME: %[^\n]",
            sub.subjectCode, sub.subjectName, sub.professorName, sub.subCategory, sub.week, sub.time) == 6)
        {
            // ★★★ 트리밍 매우 중요 ★★★
            trimAll(sub.subjectCode);
            trimAll(sub.subjectName);
            trimAll(sub.professorName);
            trimAll(sub.subCategory);
            trimAll(sub.week);
            trimAll(sub.time);

            int subjectCodeInt = atoi(sub.subjectCode);

            // 이 유저의 코드인지 확인
            int hasThisCode = 0;
            for (int i = 0; i < codeCount; i++) 
            {
                if (codesArr[i] == subjectCodeInt) { hasThisCode = 1; break; }
            }
            if (!hasThisCode) continue;

            // ★ 시간 파싱 보강: "9:00~10:00", "9 : 00 ~ 10 : 00" 모두 허용
            int sh = 0, sm = 0, eh = 0, em = 0;
            if (sscanf(sub.time, "%d:%d~%d:%d", &sh, &sm, &eh, &em) != 4) 
            {
                sscanf(sub.time, "%d : %d ~ %d : %d", &sh, &sm, &eh, &em);
            }
            if (eh <= sh) eh = sh + 1; // 최소 1시간

            // ★ 요일 다중 지원: "월,수" 형태 처리
            char weekBuf[32];
            strncpy(weekBuf, sub.week, sizeof(weekBuf) - 1);
            weekBuf[sizeof(weekBuf) - 1] = '\0';

            char* dayTok = strtok(weekBuf, ",");
            while (dayTok) 
            {
                trimAll(dayTok);
                int dayIdx = getDayIndex(dayTok);   // "월"/"화"/... 만 전달되도록

                int hourIdx = sh - 9;
                int duration = eh - sh;
                if (duration <= 0) duration = 1;

                // 중복 체크
                int canPlace = (dayIdx >= 0 && hourIdx >= 0 && hourIdx < 13);
                for (int h = hourIdx; canPlace && h < hourIdx + duration && h < 13; h++) 
                {
                    if (occupied[dayIdx][h]) canPlace = 0;
                }

                if (canPlace) 
                {
                    int boxX = startX + dayIdx * cellWidth;
                    int boxY = startY + hourIdx * cellHeight;
                    int boxHeight = cellHeight * duration;

                    int randColor = getRandomColor();
                    setColor(randColor);
                    drawBox(boxX, boxY, cellWidth, boxHeight);
                    gotoxy(boxX + 1, boxY + 1);
                    printf("%s", sub.subjectName);
                    setColor(7);

                    for (int h = hourIdx; h < hourIdx + duration && h < 13; h++)
                        occupied[dayIdx][h] = 1;

                    // 히트박스 저장
                    if (subBoxCount < (int)(sizeof(subBoxes) / sizeof(subBoxes[0]))) 
                    {
                        subBoxes[subBoxCount].left = boxX;
                        subBoxes[subBoxCount].top = boxY;
                        subBoxes[subBoxCount].right = boxX + cellWidth - 1;
                        subBoxes[subBoxCount].bottom = boxY + boxHeight - 1;
                        subBoxes[subBoxCount].sub = sub;
                        subBoxCount++;
                    }
                }

                dayTok = strtok(NULL, ",");
            }
        }
    }
    // 그리기 후 즉시 화면 반영
    fflush(stdout);

    // ─────────────────────────────────────────────────────────────
    // 마우스 입력 설정(루프 밖 1회)
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode = 0, mode = 0;
    GetConsoleMode(hInput, &prevMode);
    mode = prevMode;
    mode &= ~ENABLE_QUICK_EDIT_MODE;
    mode |= ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT;
    SetConsoleMode(hInput, mode);
    FlushConsoleInputBuffer(hInput);

    INPUT_RECORD record;
    DWORD events;

    while (1)
    {
        ReadConsoleInput(hInput, &record, 1, &events);
        if (record.EventType != MOUSE_EVENT) continue;

        MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
        if (mouse.dwEventFlags != 0) continue; // 이동/휠 등 무시

        if (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            // ★ 수업 박스 클릭 검사
            for (int k = 0; k < subBoxCount; ++k)
            {
                if (isInside(clickX, clickY,
                    subBoxes[k].left, subBoxes[k].top,
                    subBoxes[k].right, subBoxes[k].bottom))
                {
                    // 상세 팝업
                    system("cls");
                    drawBox(10, 8, 70, 20);
                    gotoxy(12, 10); printf("선택한 과목 상세");
                    gotoxy(12, 12); printf("과목코드 : %s", subBoxes[k].sub.subjectCode);
                    gotoxy(12, 13); printf("과목명   : %s", subBoxes[k].sub.subjectName);
                    gotoxy(12, 14); printf("분류     : %s", subBoxes[k].sub.subCategory);
                    gotoxy(12, 15); printf("교수명   : %s", subBoxes[k].sub.professorName);
                    gotoxy(12, 16); printf("요일     : %s", subBoxes[k].sub.week);
                    gotoxy(12, 17); printf("시간     : %s", subBoxes[k].sub.time);
                    gotoxy(12, 19); printf("아무 키나 누르면 돌아갑니다...");
                    _getch();

                    // 돌아오면 시간표 전체를 다시 그리기 위해 재귀 호출 또는 함수 재호출
                    // (간단히 재호출)
                    SetConsoleMode(hInput, prevMode);
                    handleTimetableScreen(userID, userName);
                    return;
                }
            }

            // 버튼: 폭을 넉넉히(라벨 길이 고려)
            if (isInside(clickX, clickY, 28, 45, 28 + 11, 45)) 
            {
                SetConsoleMode(hInput, prevMode);
                handleSubjectList(userID);
                return;
            }
            else if (isInside(clickX, clickY, 42, 45, 42 + 11, 45)) 
            {
                deleteMode = 1;
                // 하단 안내
                gotoxy(20, 43);
                printf("삭제 모드: 삭제할 과목 칸을 클릭하세요. (취소하려면 [메인메뉴] 또는 ESC)");
                fflush(stdout);
                continue;
            }
            else if (isInside(clickX, clickY, 56, 45, 56 + 11, 45)) 
            {
                SetConsoleMode(hInput, prevMode);
                handleMainMenuScreen(userID, userName, userPW);
                return;
            }
        }
    }
}

// 강의 목록 함수
void handleSubjectList(const char* userID)
{
    system("cls");
    drawBox(4, 1, 92, 47);

    SubjectList subjectdata[300];

    char line[256];
    int totalSubjects = 0;
    const int itemsPerPage = 8;
    int page = 1;
    int currentIndex = 0;

    FILE* file = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Subject.txt", "r");
    if (!file)
    {
        gotoxy(30, 10);
        printf(" 강의 목록이 열리지 않습니다. ");
        _getch();
        return;
    }

    // 강의 전체 불러오기
    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line,
            "subjectCODE: %49[^/] / subjectNAME: %49[^/] / professorNAME: %49[^/] / subjectCATE: %49[^/] / WEEK: %49[^/] / TIME: %[^\\n]",
            subjectdata[totalSubjects].subjectCode,
            subjectdata[totalSubjects].subjectName,
            subjectdata[totalSubjects].professorName,
            subjectdata[totalSubjects].subCategory,
            subjectdata[totalSubjects].week,
            subjectdata[totalSubjects].time) == 6)
        {
            trimAll(subjectdata[totalSubjects].subjectCode);
            trimAll(subjectdata[totalSubjects].subjectName);
            trimAll(subjectdata[totalSubjects].subCategory);
            trimAll(subjectdata[totalSubjects].professorName);
            trimAll(subjectdata[totalSubjects].week);
            trimAll(subjectdata[totalSubjects].time);

            totalSubjects++;
        }
    }
    fclose(file); 

    // 화면/좌표 고정값
    const int boxLeft = 20;
    const int boxWidth = 70;
    const int firstTop = 8;
    const int vStep = 4;   

    while (1)
    {
        int totalPages = (totalSubjects + itemsPerPage - 1) / itemsPerPage;
        if (totalPages == 0) totalPages = 1;

        gotoxy(30, 5);
        printf(" ▶-----   강의 목록 (페이지 %d/%d)   -----◀", page, totalPages);

        for (int slot = 0; slot < itemsPerPage; ++slot)
        {
            int i = currentIndex + slot;
            int top = firstTop + slot * vStep;

            drawBox(boxLeft, top, boxWidth, 3);
            gotoxy(boxLeft + 2, top + 1);

            if (i < totalSubjects)
            {
                printf("%-8s | %-10s | %-8s | %-10s | %-4s | %s",
                    subjectdata[i].subjectCode,
                    subjectdata[i].subjectName,
                    subjectdata[i].subCategory,
                    subjectdata[i].professorName,
                    subjectdata[i].week,
                    subjectdata[i].time);
            }
            else
            {
                printf(" - 빈 칸 - ");
            }
        }

        // 페이지/뒤로 버튼
        gotoxy(20, 45); printf("[ 이전 페이지 ]");
        gotoxy(40, 45); printf("[ 다음 페이지 ]");
        gotoxy(60, 45); printf("[ 시간표 ]");

        // 입력 모드 설정
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        DWORD prevMode;
        GetConsoleMode(hInput, &prevMode);
        SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

        INPUT_RECORD record;
        DWORD events;

        // 입력 대기
        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType == MOUSE_EVENT)
        {
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            int clickX = mouse.dwMousePosition.X;
            int clickY = mouse.dwMousePosition.Y;

            if (mouse.dwEventFlags == 0 && (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
            {
                // ▼▼▼ 강의 박스 클릭 → Timetable.txt 업데이트 ▼▼▼
                for (int slot = 0; slot < itemsPerPage; ++slot)
                {
                    int i = currentIndex + slot;
                    int top = firstTop + slot * vStep;
                    int bottom = top + 2;                 // drawBox 높이 3 → top..top+2
                    int left = boxLeft;
                    int right = boxLeft + boxWidth - 1;

                    if (isInside(clickX, clickY, left, top, right, bottom))
                    {
                        if (i < totalSubjects)
                        {
                            // 코드 문자열(예: "001") 그대로 사용
                            const char* code = subjectdata[i].subjectCode;
                            int rc = appendSubjectToTimetable(userID, code, "C:\\Users\\Koomy\\source\\repos\\project01\\Timetable.txt");

                            // 피드백 팝업
                            system("cls");
                            drawBox(20, 12, 50, 7);
                            gotoxy(22, 14);
                            if (rc == 0) {
                                printf("시간표에 과목이 추가되었습니다.");
                            }
                            else if (rc == 1) {
                                printf("이미 등록된 과목입니다.");
                            }
                            else {
                                printf("추가 중 오류가 발생했습니다.(%d)", rc);
                            }
                            gotoxy(22, 16); printf("아무 키나 누르면 돌아갑니다...");
                            _getch();

                            // 목록화면 다시 그리기
                            system("cls");
                            drawBox(4, 1, 92, 47);
                        }
                        break; // 박스 루프 탈출
                    }
                }

                // 페이지 +1 버튼
                if (isInside(clickX, clickY, 20, 45, 35, 45)) 
                {
                    if (currentIndex - itemsPerPage >= 0) 
                    { currentIndex -= itemsPerPage; page--; }
                }
                // 페이지 -1 버튼
                else if (isInside(clickX, clickY, 40, 45, 55, 45)) 
                {
                    if (currentIndex + itemsPerPage < totalSubjects) 
                    { currentIndex += itemsPerPage; page++; }
                }
                // 시간표 목록 되돌아가기 버튼
                else if (isInside(clickX, clickY, 60, 45, 75, 45)) 
                {
                    handleTimetableScreen(userID, userName);
                    break;
                }
            }
        }
        SetConsoleMode(hInput, prevMode);
    }
}

// 강의 추가 함수
int appendSubjectToTimetable(const char* userID, const char* subjectCode, const char* path)
{
    // 안전 여유치 넉넉히
    char lines[600][512];
    int lineCount = 0;
    FILE* fp = fopen(path, "r");
    if (!fp) {
        // 파일이 없으면 새로 만듦
        FILE* fw = fopen(path, "w");
        if (!fw) return -1;
        fprintf(fw, "userID: %s / subjectCODE: %s\n", userID, subjectCode);
        fclose(fw);
        return 0;
    }

    // 읽기
    while (lineCount < 600 && fgets(lines[lineCount], sizeof(lines[0]), fp)) 
    {
        lineCount++;
    }
    fclose(fp);

    // userID 줄 찾기
    int foundIdx = -1;
    char fileUser[64], codes[256];
    for (int i = 0; i < lineCount; i++) 
    {
        fileUser[0] = '\0'; codes[0] = '\0';
        if (sscanf(lines[i], "userID: %63[^/] / subjectCODE: %255[^\r\n]", fileUser, codes) == 2) 
        {
            // 좌우 공백 제거
            trimAll(fileUser); trimAll(codes);
            if (strcmp(fileUser, userID) == 0) 
            {
                foundIdx = i;
                break;
            }
        }
    }

    // 이미 있으면 중복 체크 → 없으면 추가
    if (foundIdx >= 0) 
    {
        // codes 토큰 분해해서 중복인지 확인
        char codesCopy[256];
        strncpy(codesCopy, codes, sizeof(codesCopy));
        codesCopy[sizeof(codesCopy) - 1] = '\0';

        int isDup = 0;
        char* tok = strtok(codesCopy, ",");
        while (tok) 
        {
            trimAll(tok);
            if (strcmp(tok, subjectCode) == 0) { isDup = 1; break; }
            tok = strtok(NULL, ",");
        }
        if (isDup) return 1; // 이미 있음

        // 뒤에 ", subjectCode" 추가(콤마 뒤 공백은 취향대로)
        char newCodes[256];
        if ((int)strlen(codes) > 0)
            snprintf(newCodes, sizeof(newCodes), "%s,%s", codes, subjectCode);
        else
            snprintf(newCodes, sizeof(newCodes), "%s", subjectCode);

        // 새 라인으로 교체
        char newLine[512];
        snprintf(newLine, sizeof(newLine), "userID: %s / subjectCODE: %s\n", userID, newCodes);
        strncpy(lines[foundIdx], newLine, sizeof(lines[foundIdx]) - 1);
        lines[foundIdx][sizeof(lines[foundIdx]) - 1] = '\0';
    }
    else {
        // userID가 없으면 새 줄 추가
        if (lineCount >= 600) return -2; // 용량 초과
        snprintf(lines[lineCount], sizeof(lines[0]),
            "userID: %s / subjectCODE: %s\n", userID, subjectCode);
        lineCount++;
    }

    // 전체 다시 쓰기
    fp = fopen(path, "w");
    if (!fp) return -3;
    for (int i = 0; i < lineCount; i++) 
    {
        // 줄 끝에 개행이 없다면 보정
        size_t L = strlen(lines[i]);
        if (L == 0 || lines[i][L - 1] != '\n') 
        {
            if (L < sizeof(lines[i]) - 1) 
            {
                lines[i][L] = '\n';
                lines[i][L + 1] = '\0';
            }
        }
        fputs(lines[i], fp);
    }
    fclose(fp);
    return 0;
}

// 강의 삭제 함수
int removeSubjectFromTimetable(const char* userID, const char* subjectCode, const char* path)
{
    // 넉넉한 버퍼
    char lines[600][512];
    int lineCount = 0;

    FILE* fp = fopen(path, "r");
    if (!fp) return -1;

    while (lineCount < 600 && fgets(lines[lineCount], sizeof(lines[0]), fp)) 
    {
        lineCount++;
    }
    fclose(fp);

    int foundIdx = -1;
    char fileUser[64], codes[256];

    for (int i = 0; i < lineCount; i++) 
    {
        fileUser[0] = '\0'; codes[0] = '\0';
        if (sscanf(lines[i], "userID: %63[^/] / subjectCODE: %255[^\r\n]", fileUser, codes) == 2) 
        {
            trimAll(fileUser); trimAll(codes);
            if (strcmp(fileUser, userID) == 0) 
            {
                foundIdx = i;
                break;
            }
        }
    }
    if (foundIdx < 0) return 1; // userID 줄 없음

    // 코드 리스트 파싱하여 target 제외
    char buf[256];
    strncpy(buf, codes, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    int targetI = atoi(subjectCode);
    char* tok = strtok(buf, ",");
    char rebuilt[256] = { 0 };
    int removed = 0;

    while (tok) 
    {
        char t[32];
        strncpy(t, tok, sizeof(t) - 1); t[sizeof(t) - 1] = '\0';
        trimAll(t);

        if (atoi(t) == targetI) 
        {
            removed = 1;              // 이번 토큰은 건너뜀(=삭제)
        }
        else 
        {
            if (rebuilt[0] != '\0') strncat(rebuilt, ",", sizeof(rebuilt) - 1);
            strncat(rebuilt, t, sizeof(rebuilt) - 1);
        }
        tok = strtok(NULL, ",");
    }
    if (!removed) return 1; // 해당 코드가 없음

    // 줄 교체(빈 목록도 허용)
    char newLine[512];
    snprintf(newLine, sizeof(newLine), "userID: %s / subjectCODE: %s\n", userID, rebuilt);
    strncpy(lines[foundIdx], newLine, sizeof(lines[foundIdx]) - 1);
    lines[foundIdx][sizeof(lines[foundIdx]) - 1] = '\0';

    // 파일 다시 쓰기
    fp = fopen(path, "w");
    if (!fp) return -2;
    for (int i = 0; i < lineCount; i++) 
    {
        // 라인 끝 개행 보정
        size_t L = strlen(lines[i]);
        if (L == 0 || lines[i][L - 1] != '\n') 
        {
            if (L < sizeof(lines[i]) - 1) 
            {
                lines[i][L] = '\n';
                lines[i][L + 1] = '\0';
            }
        }
        fputs(lines[i], fp);
    }
    fclose(fp);
    return 0;
}


// 친구 시간표 보기 (친구ID/친구이름을 표시, 뒤로가기는 내 친구목록으로)
void handleFriendTimetableScreen(const char* userName, const char* userID, const char* friendID, const char* friendName)
{
    system("cls");
    drawBox(4, 1, 92, 47);

    int cellWidth = 15, cellHeight = 3, dayCellHeight = 3, timeCellWidth = 7;
    int startX = 16, startY = 7;
    const char* days[5] = { "월","화","수","목","금" };

    // 제목
    gotoxy(33, 1); printf(" %s님의 시간표 (친구)", friendName);

    // 버튼
    gotoxy(30, 45); printf("[ 뒤로가기 ]");
    gotoxy(46, 45); printf("[ 메인메뉴 ]");

    // 요일 헤더
    for (int d = 0; d < 5; d++) 
    {
        int x = startX + d * cellWidth, y = startY - dayCellHeight;
        drawBox(x, y, cellWidth, dayCellHeight);
        gotoxy(x + (cellWidth / 2) - 1, y + 1); printf("%s", days[d]);
    }
    // 시간 열
    for (int h = 0; h < 11; h++) 
    {
        int posY = startY + h * cellHeight;
        drawBox(startX - timeCellWidth - 1, posY, timeCellWidth, cellHeight);
        gotoxy(startX - timeCellWidth + 1, posY + (cellHeight / 2));
        printf("%02d시", 9 + h);
    }
    // 격자
    for (int r = 0; r < 11; r++) 
    {
        int posY = startY + r * cellHeight;
        for (int c = 0; c < 5; c++) 
        {
            drawBox(startX + c * cellWidth, posY, cellWidth, cellHeight);
        }
    }

    // ── Timetable.txt 에서 friendID의 과목코드 읽기
    FILE* tfp = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Timetable.txt", "r");
    if (!tfp) 
    {
        gotoxy(25, 43); printf("시간표 데이터(Timetable.txt)를 열 수 없습니다.");
        _getch(); return;
    }
    char line[256], codes[200] = { 0 };
    int found = 0;
    while (fgets(line, sizeof(line), tfp)) 
    {
        char fileUser[50], tmp[200] = { 0 };
        if (sscanf(line, "userID: %49[^/] / subjectCODE: %[^\n]", fileUser, tmp) == 2) 
        {
            trimAll(fileUser);
            if (strcmp(fileUser, friendID) == 0) 
            {
                strncpy(codes, tmp, sizeof(codes) - 1);
                found = 1; break;
            }
        }
    }
    fclose(tfp);
    if (!found || strlen(codes) == 0) 
    {
        gotoxy(27, 43); printf("등록된 시간표 데이터가 없습니다.");
        _getch(); return;
    }
    // 코드 배열
    int codesArr[50], codeCount = 0;
    char* tok = strtok(codes, ",");
    while (tok && codeCount < 50) 
    {
        trimAll(tok);
        codesArr[codeCount++] = atoi(tok);
        tok = strtok(NULL, ",");
    }

    int occupied[5][13] = { 0 };

    // ── Subject.txt 읽어 배치
    FILE* sfp = fopen("C:\\Users\\Koomy\\source\\repos\\project01\\Subject.txt", "r");
    if (!sfp) {
        gotoxy(25, 43); printf("과목 데이터(Subject.txt)를 열 수 없습니다.");
        _getch(); return;
    }

    while (fgets(line, sizeof(line), sfp)) 
    {
        SubjectList sub;
        if (sscanf(line,
            "subjectCODE: %9[^/] / subjectNAME: %49[^/] / professorNAME: %49[^/] / subjectCATE: %19[^/] / WEEK: %19[^/] / TIME: %[^\n]",
            sub.subjectCode, sub.subjectName, sub.professorName, sub.subCategory, sub.week, sub.time) == 6)
        {
            trimAll(sub.subjectCode); trimAll(sub.subjectName);
            trimAll(sub.professorName); trimAll(sub.subCategory);
            trimAll(sub.week); trimAll(sub.time);

            int code = atoi(sub.subjectCode);
            int has = 0; for (int i = 0; i < codeCount; i++) { if (codesArr[i] == code) { has = 1; break; } }
            if (!has) continue;

            // 시간 파싱: "9:00~10:00" 와 "9 : 00 ~ 10 : 00" 둘 다 지원
            int sh = 0, sm = 0, eh = 0, em = 0;
            if (sscanf(sub.time, "%d:%d~%d:%d", &sh, &sm, &eh, &em) != 4)
                sscanf(sub.time, "%d : %d ~ %d : %d", &sh, &sm, &eh, &em);
            if (eh <= sh) eh = sh + 1;

            // 요일(복수 지원: "월,수")
            char weekBuf[32]; strncpy(weekBuf, sub.week, sizeof(weekBuf) - 1); weekBuf[31] = '\0';
            char* dTok = strtok(weekBuf, ",");
            while (dTok) {
                trimAll(dTok);
                int dayIdx = getDayIndex(dTok);
                int hourIdx = sh - 9;
                int duration = (eh - sh > 0) ? (eh - sh) : 1;

                int canPlace = (dayIdx >= 0 && hourIdx >= 0 && hourIdx < 13);
                for (int h = hourIdx; canPlace && h < hourIdx + duration && h < 13; h++) 
                {
                    if (occupied[dayIdx][h]) canPlace = 0;
                }
                if (canPlace) {
                    int boxX = startX + dayIdx * cellWidth;
                    int boxY = startY + hourIdx * cellHeight;
                    int boxH = cellHeight * duration;

                    int randColor = getRandomColor();
                    setColor(randColor);
                    drawBox(boxX, boxY, cellWidth, boxH);
                    gotoxy(boxX + 1, boxY + 1); printf("%s", sub.subjectName);
                    setColor(7);

                    for (int h = hourIdx; h < hourIdx + duration && h < 13; h++) occupied[dayIdx][h] = 1;
                }
                dTok = strtok(NULL, ",");
            }
        }
    }
    fclose(sfp);
    fflush(stdout);

    // 입력(뒤로가기/메인)
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode = 0, mode = 0; GetConsoleMode(hInput, &prevMode);
    mode = prevMode; mode &= ~ENABLE_QUICK_EDIT_MODE;
    mode |= ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT;
    SetConsoleMode(hInput, mode); FlushConsoleInputBuffer(hInput);

    INPUT_RECORD rec; DWORD ne;
    while (1) 
    {
        if (!ReadConsoleInput(hInput, &rec, 1, &ne)) continue;
        if (rec.EventType != MOUSE_EVENT) continue;
        MOUSE_EVENT_RECORD m = rec.Event.MouseEvent;
        if (m.dwEventFlags != 0) continue;
        if (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) 
        {
            int x = m.dwMousePosition.X, y = m.dwMousePosition.Y;

            // 뒤로가기
            if (isInside(x, y, 30, 45, 40, 45)) 
            {
                SetConsoleMode(hInput, prevMode);
                handleFriendsListScreen(userID);
                return;
            }
            // 메인메뉴
            if (isInside(x, y, 46, 45, 56, 45)) 
            {
                SetConsoleMode(hInput, prevMode);
                handleMainMenuScreen(userName, userID, userPW); // userPW 전역/외부 가정
                return;
            }
        }
    }
}
