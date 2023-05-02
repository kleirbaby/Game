// firstgame.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <array>
#include <locale.h>
#include <stdio.h>
#include <time.h>

using namespace std;

typedef struct
{
    short x;
    short y;
}Pos;

static void refreshMapData(char map[], int maxRow, int maxCol, Pos obj[], int objSize);
static void createMap(char map[], int maxRow, int maxCol);
static void handleInput();
static bool isTankByXY(int x, int y);
static bool isTankByPos(Pos* pos);
static bool isDestByXY(int x, int y);
static bool isDestByPos(Pos* pos);
static bool isSuccess();
static void test();

static char map[5][8] = {
    {'#','#','#','#','#','#','#','#'},
    {'#',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ','#'},
    {'#','#','#','#','#','#','#','#'}
};

enum
{
    DEST1 = 0,
    DEST2,
    TANK1,
    TANK2,
    ACTOR,
};

enum Dir
{
    DIR_EMPTY = 0,
    POS_H,
    NEG_H,
    POS_V,
    NEG_V,
};

#define MAX_DEST_COUNT 2

static Pos objMap[5] = { 0 };
static Dir actorDir = DIR_EMPTY;
static int sucCount = 0;

int main()
{
    //////////////////////////////
    test();
    getchar();
    //////////////////////////////
    objMap[DEST1].x = 1;
    objMap[DEST1].y = 3;

    objMap[DEST2].x = 1;
    objMap[DEST2].y = 4;

    objMap[TANK1].x = 2;
    objMap[TANK1].y = 3;

    objMap[TANK2].x = 2;
    objMap[TANK2].y = 4;

    objMap[ACTOR].x = 1;
    objMap[ACTOR].y = 5;

    char initMap[5][8];
    memcpy(initMap, map, sizeof(initMap));
    refreshMapData(initMap[0], 5, 8, objMap, 5);

    createMap(initMap[0], 5, 8);

    bool bSuc = false;
    while (true) {
        handleInput();
        bSuc = isSuccess();
        if (bSuc) {
            break;
        }
    }

    if (bSuc) {
        cout << "Successful !" << endl;
    }
    else {
        cout << "game end" << endl;
    }
    getchar();

    return 0;
}

void refreshMapData(char map[], int maxRow, int maxCol, Pos obj[], int objSize)
{
    for (size_t idx = DEST1; idx <= ACTOR; idx++) {
        if (obj[idx].x > 0 && obj[idx].x < maxRow - 1 && obj[idx].y > 0 && obj[idx].y < maxCol - 1) {
            switch (idx) {
            case DEST1:
            case DEST2: {
                map[obj[idx].x * maxCol + obj[idx].y] = '.';
                break;
            }
            case TANK1:
            case TANK2: {
                if (isDestByXY(obj[idx].x, obj[idx].y)) {
                    map[obj[idx].x * maxCol + obj[idx].y] = 'O';
                }
                else {
                    map[obj[idx].x * maxCol + obj[idx].y] = 'o';
                }
                break;
            }
            case ACTOR: {
                if (isDestByXY(obj[idx].x, obj[idx].y)) {
                    map[obj[idx].x * maxCol + obj[idx].y] = 'P';
                }
                else {
                    map[obj[idx].x * maxCol + obj[idx].y] = 'p';
                }
                break;
            }
            }
        }
    }
}

void createMap(char map[], int maxRow, int maxCol)
{
    system("cls");
    for (size_t row = 0; row < maxRow; ++row) {
        for (size_t col = 0; col < maxCol; ++col) {
            cout << map[row * maxCol + col];
        }
        cout << endl;
    }
}

void handleInput()
{
    int ch = getchar();
    fflush(stdin);
    switch (ch)
    {
    case 'w': {
        actorDir = NEG_V;
        if (map[objMap[ACTOR].x - 1][objMap[ACTOR].y] != '#') {
            objMap[ACTOR].x--;
        }

        for (size_t idx = TANK1; idx <= TANK2; idx++) {
            if (objMap[ACTOR].x == objMap[idx].x && objMap[ACTOR].y == objMap[idx].y) {
                if (map[objMap[idx].x - 1][objMap[idx].y] == '#') {
                    objMap[ACTOR].x++;
                }
                else if (isTankByXY(objMap[idx].x - 1, objMap[idx].y)) {
                    objMap[ACTOR].x++;
                }
                else {
                    objMap[idx].x--;
                }
                break;
            }
        }
        break;
    }
    case 's': {
        actorDir = POS_V;
        if (map[objMap[ACTOR].x + 1][objMap[ACTOR].y] != '#') {
            objMap[ACTOR].x++;
        }

        for (size_t idx = TANK1; idx <= TANK2; idx++) {
            if (objMap[ACTOR].x == objMap[idx].x && objMap[ACTOR].y == objMap[idx].y) {
                if (map[objMap[idx].x + 1][objMap[idx].y] == '#') {
                    objMap[ACTOR].x--;
                }
                else if (isTankByXY(objMap[idx].x + 1, objMap[idx].y)) {
                    objMap[ACTOR].x--;
                }
                else {
                    objMap[idx].x++;
                }
                break;
            }
        }
        break;
    }
    case 'a': {
        actorDir = NEG_H;
        if (map[objMap[ACTOR].x][objMap[ACTOR].y - 1] != '#') {
            objMap[ACTOR].y--;
        }

        for (size_t idx = TANK1; idx <= TANK2; idx++) {
            if (objMap[ACTOR].x == objMap[idx].x && objMap[ACTOR].y == objMap[idx].y) {
                if (map[objMap[idx].x][objMap[idx].y - 1] == '#') {
                    objMap[ACTOR].y++;
                }
                else if (isTankByXY(objMap[idx].x, objMap[idx].y - 1)) {
                    objMap[ACTOR].y++;
                }
                else {
                    objMap[idx].y--;
                }
                break;
            }
        }
        break;
    }
    case 'd': {
        actorDir = POS_H;
        if (map[objMap[ACTOR].x][objMap[ACTOR].y + 1] != '#') {
            objMap[ACTOR].y++;
        }

        for (size_t idx = TANK1; idx <= TANK2; idx++) {
            if (objMap[ACTOR].x == objMap[idx].x && objMap[ACTOR].y == objMap[idx].y) {
                if (map[objMap[idx].x][objMap[idx].y + 1] == '#') {
                    objMap[ACTOR].y--;
                }
                else if (isTankByXY(objMap[idx].x, objMap[idx].y + 1)) {
                    objMap[ACTOR].y--;
                }
                else {
                    objMap[idx].y++;
                }
                break;
            }
        }
        break;
    }
    default:
        return;
    }

    char initMap[5][8];
    memcpy(initMap, map, sizeof(initMap));
    refreshMapData(initMap[0], 5, 8, objMap, 5);
    createMap(initMap[0], 5, 8);
}

bool isTankByXY(int x, int y)
{
    Pos pos;
    pos.x = x;
    pos.y = y;
    return isTankByPos(&pos);
}

bool isTankByPos(Pos* pos)
{
    for (size_t idx = TANK1; idx <= TANK2; idx++) {
        if (pos->x == objMap[idx].x && pos->y == objMap[idx].y) {
            return true;
        }
    }

    return false;
}

bool isDestByXY(int x, int y)
{
    Pos pos;
    pos.x = x;
    pos.y = y;
    return isDestByPos(&pos);
}

bool isDestByPos(Pos* pos)
{
    for (size_t idx = DEST1; idx <= DEST2; idx++) {
        if (pos->x == objMap[idx].x && pos->y == objMap[idx].y) {
            return true;
        }
    }

    return false;
}

bool isSuccess()
{
    int nSuc = 0;
    for (size_t idx = TANK1; idx <= TANK2; idx++) {
        if (isDestByPos(&objMap[idx])) {
            nSuc++;
        }
    }

    return nSuc == MAX_DEST_COUNT;
}

void test()
{
    time_t currtime;
    struct tm timer;
    char buffer[80];

    time(&currtime);
    localtime_s(&timer,&currtime);

    printf("Locale is: %s\n", setlocale(LC_ALL, "en_GB.UTF-8"));
    strftime(buffer, 80, "%c", &timer);
    printf("Date is: %s\n", buffer);


    printf("Locale is: %s\n", setlocale(LC_ALL, "de_DE.UTF-8"));
    strftime(buffer, 80, "%c", &timer);
    printf("Date is: %s\n", buffer);

    printf("Locale is: %s\n", setlocale(LC_ALL, "zh-CN.UTF-8"));
    strftime(buffer, 80, "%c", &timer);
    printf("Date is: %s\n", buffer);

    std::array<int, 5> a1 = {1,22,2,3,4};
    
    std::array<std::array<int, 4>, 2> a2 = { 1,34,2,3,44,55,22,55};

    std::cout << a2[0][0] << std::endl;
    std::cout << a2[0][1] << std::endl;
}