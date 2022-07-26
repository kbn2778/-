#ifndef BASEHEADER_H_INCLUDED
#define BASEHEADER_H_INCLUDED

#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <string.h>

//define size of map(I is y-size and J is x-size)
//I should be odd number.
#define I 11
#define J 28
#define LAYER I/2
#define MOB_DELAY 150 //milliseconds

//define map and set 0
char lmap[I][J] = {0, };
//define level
int level=1;
//define score
int score=0;
//define the number of stars
int cnt_star=0;
//set direction
enum{
    left,
    right,
    up,
    down
};
//set state of one blocks in map
enum{
    empty,
    block,
    player,
    star,
    monster
};
//define object structure
typedef struct Obj {
    int x, y;
    int direction;
    //for monster
    struct Obj *next;
}Obj;
//print something on this position
void gotoxy(int x, int y){
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//initialize the console
void init(){
    //claer console
    system("cls");
    //delete cursor in console
    CONSOLE_CURSOR_INFO cursorInfo = {0,};
    cursorInfo.bVisible = 0;
    cursorInfo.dwSize=1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
//set color function
void setcolor(int color){
    //14=yellow, 15=white
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

#endif // BASEHEADER_H_INCLUDED
