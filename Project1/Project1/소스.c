#include <stdio.h>
#include <windows.h>

void SetRealConsoleSize(int cols, int rows);

int main() {

    //SetRealConsoleSize(142, 50);
    system("mode con");
}



void SetRealConsoleSize(int cols, int rows) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { (SHORT)cols, (SHORT)rows };
    SMALL_RECT windowSize = { 0, 0, (SHORT)(cols - 1), (SHORT)(rows - 1) };

    // 1단계: 창을 최소로 줄여야 SetConsoleScreenBufferSize 성공함
    SMALL_RECT tmp = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(hConsole, TRUE, &tmp);

    // 2단계: 버퍼 크기 변경
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // 3단계: 실제 창 크기 변경
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}
