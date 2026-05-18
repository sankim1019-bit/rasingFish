#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define NUM 6
#define MAX_WATER 100
#define WATER_AMOUNT 3

typedef struct {
    int waterLevel;
    int isAlive;
    int fishSize; 
} FishTank;

int level;
FishTank arrayFishes[NUM];
long totalElapsedTime;
long startTime = 0;
long lastTickTime = 0;
FishTank* cursor;
int position = 0;

char systemMessage[100];

void initData();
void update();
void render();
void decreaseWater(FishTank* tanks, long elapsedTime);
int checkFishAlive();
int checkWin();
void gotoxy(int x, int y);
int nonBlockingGetch();

void processInput();
void moveCursorLeft();
void moveCursorRight();
void giveWater();
void growAllFishes(FishTank* tanks); 

int main(void) {
    int gameOver = 0;

    initData();
    startTime = time(NULL);
    lastTickTime = startTime;
    cursor = arrayFishes;

    while (!gameOver) {
        position = cursor - arrayFishes;

        processInput();
        update();
        position = cursor - arrayFishes;

        if (totalElapsedTime / 20 > level - 1) {
            level++;
            growAllFishes(arrayFishes); 
            sprintf_s(systemMessage, sizeof(systemMessage), "레벨 %d 달성. 물고기가 성장합니다.", level);
        }

        if (checkWin()) {
            gameOver = 1;
        }
        else if (checkFishAlive() == 0) {
            sprintf_s(systemMessage, sizeof(systemMessage), "모든 물고기가 죽었습니다. 게임을 종료합니다.");
            gameOver = 1;
        }

        render();
        Sleep(100);
    }

    while (_kbhit()) {
        _getch();
    }

    gotoxy(2, 17);
    printf("게임을 종료하려면 아무 키나 누르세요...");
    _getch();

    system("cls");
    return 0;
}

void initData() {
    level = 1;
    totalElapsedTime = 0;

    sprintf_s(systemMessage, sizeof(systemMessage), "게임을 시작합니다. 물고기에게 물을 주세요.");

    for (int i = 0; i < NUM; i++) {
        arrayFishes[i].waterLevel = 100;
        arrayFishes[i].isAlive = 1;
        arrayFishes[i].fishSize = 1; 
    }
}

void processInput() {
    int input = nonBlockingGetch();
    if (input == -1) {
        return;
    }

    switch (input) {
    case 'j':
        moveCursorLeft();
        break;
    case 'l':
        moveCursorRight();
        break;
    case 'k':
        giveWater();
        break;
    default:
        break;
    }
}

void moveCursorLeft() {
    if (cursor > arrayFishes) {
        cursor--;
    }
}

void moveCursorRight() {
    if (cursor < arrayFishes + NUM - 1) {
        cursor++;
    }
}

void giveWater() {
    if (cursor->isAlive) {
        cursor->waterLevel += WATER_AMOUNT;


        if (cursor->waterLevel > MAX_WATER) {
            cursor->waterLevel = MAX_WATER;
        }
    }
}

void update() {
    long currentTime = time(NULL);
    totalElapsedTime = currentTime - startTime;

    long elapsedTime = currentTime - lastTickTime;

    if (elapsedTime > 0) {
        decreaseWater(arrayFishes, elapsedTime);
        lastTickTime = currentTime;
    }
}

void render() {
    system("cls");

    gotoxy(2, 2);
    printf("물고기 키우기 게임");


    gotoxy(2, 4);
    printf(">> 안내: %s", systemMessage);

    gotoxy(2, 5);
    printf("-------------------------------------------------");

    gotoxy(2, 7);
    printf("게임 경과 시간: %ld초", totalElapsedTime);

    gotoxy(2, 9);
    printf("어항 번호: ");
    for (int i = 0; i < NUM; i++) {
        printf(" %3d", i + 1);
    }

    gotoxy(2, 10);
    printf("물 높이  : ");
    for (int i = 0; i < NUM; i++) {
        if (arrayFishes[i].isAlive) {
            printf(" %3d", arrayFishes[i].waterLevel);
        }
        else {
            printf("   X");
        }
    }

    gotoxy(2, 11);
    printf("물고기크기:");
    for (int i = 0; i < NUM; i++) {
        if (arrayFishes[i].isAlive) {
            printf(" %3d", arrayFishes[i].fishSize);
        }
        else {
            printf("   -");
        }
    }

    gotoxy(2, 12);
    printf("현재 레벨: ");
    for (int i = 0; i < NUM; i++) {
        if (arrayFishes[i].isAlive) {
            printf(" %3d", level);
        }
        else {
            printf("   -");
        }
    }

    gotoxy(2, 13);
    printf("커서 위치: ");
    for (int i = 0; i < position; i++) {
        printf("    ");
    }
    printf("   *");

    gotoxy(2, 15);
    printf("조작 방법: 왼쪽이동(j)  물주기(k)  오른쪽이동(l)");
}

void growAllFishes(FishTank* tanks) {
    for (int i = 0; i < NUM; i++) {
        if ((tanks + i)->isAlive) {
            (tanks + i)->fishSize++;
        }
    }
}

void decreaseWater(FishTank* tanks, long elapsedTime) {
    for (int i = 0; i < NUM; i++) {
        if ((tanks + i)->isAlive) {
            (tanks + i)->waterLevel -= (level * 2 + (tanks + i)->fishSize) * (int)elapsedTime;

            if ((tanks + i)->waterLevel <= 0) {
                (tanks + i)->waterLevel = 0;
                (tanks + i)->isAlive = 0;
            }
        }
    }
}

int checkWin() {
    if (level == 5) {
        sprintf_s(systemMessage, sizeof(systemMessage), "최고 레벨을 달성했습니다.");
        return 1;
    }
    return 0;
}

int checkFishAlive() {
    for (int i = 0; i < NUM; i++) {
        if (arrayFishes[i].isAlive == 1) {
            return 1;
        }
    }
    return 0;
}

void gotoxy(int x, int y) {
    COORD CursorPosition = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}

int nonBlockingGetch() {
    if (_kbhit()) {
        return _getch();
    }
    return -1;
}