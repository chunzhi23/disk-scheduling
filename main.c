#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#define ESC 27

int header, cnt, dir, req[1001], prior[1001];

void homepage();
void title(int x, int y);
void via(int n);
void input(int x, int y, char txt[]);
void confirmRetry(int x, int y);
void reset();

void FCFS();
void SSTP();
void SCAN();
void C_SCAN();

void swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void sort(){
    for (int i = 0; i < cnt-1; i++){
		for (int j = i+1; j < cnt; j++){
			if (req[i] > req[j]) swap(&req[i], &req[j]);
		}
	}
}

void gotoxy(int x, int y){
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// reference: https://m.blog.naver.com/azure0777/220295388741
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
void setCursorType(CURSOR_TYPE e){
     CONSOLE_CURSOR_INFO CurInfo;

     switch (e) {
     case NOCURSOR:
          CurInfo.dwSize=1;
          CurInfo.bVisible=FALSE;
          break;
     case SOLIDCURSOR:
          CurInfo.dwSize=100;
          CurInfo.bVisible=TRUE;
          break;
     case NORMALCURSOR:
          CurInfo.dwSize=20;
          CurInfo.bVisible=TRUE;
          break;
     }
     SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);
}

int main(){
    system("chcp 65001  > nul");
    system("title disk scheduling");
    setCursorType(NOCURSOR);
    homepage();
}

void homepage(){
    int sch = -1;
    int x=5, y=3;
    title(x,y);

    while (true){
        int e = getch();
        if (e == ESC){
            system("cls");
            exit(0);
        }
        else if (e == 49){
            sch = 1;
            break;
        }
        else if (e == 50){
            sch = 2;
            break;
        }
        else if (e == 51){
            sch = 3;
            break;
        }
        else if (e == 52){
            sch = 4;
            break;
        }
    }

    via(sch);
}

void title(int x, int y){
    gotoxy(x, y); printf("◇ Disk Scheduling ◇");

    int cy = y+2;
    gotoxy(x, cy);  printf("┏━━━━━┳━━━━━━━━━━━━━┓");
    gotoxy(x,cy+1); printf("┃  1  ┃     FCFS    ┃");
    gotoxy(x,cy+2); printf("┣━━━━━╋━━━━━━━━━━━━━┫");
    gotoxy(x,cy+3); printf("┃  2  ┃     SSTP    ┃");
    gotoxy(x,cy+4); printf("┣━━━━━╋━━━━━━━━━━━━━┫");
    gotoxy(x,cy+5); printf("┃  3  ┃     SCAN    ┃");
    gotoxy(x,cy+6); printf("┣━━━━━╋━━━━━━━━━━━━━┫");
    gotoxy(x,cy+7); printf("┃  4  ┃    C-SCAN   ┃");
    gotoxy(x,cy+8); printf("┗━━━━━┻━━━━━━━━━━━━━┛");

    gotoxy(x,cy+10); printf("Press the number of keys to start the scheduling...");
}

void via(int n){
    system("cls");

    switch (n){
        case 1: FCFS(); break;
        case 2: SSTP(); break;
        case 3: SCAN(); break;
        case 4: C_SCAN(); break;
    }
}

void FCFS(){
    int x=5, y=3;
    int d=0;

    input(x,y,"FCFS");

    for (int i = 0; i < cnt; i++){
        d += fabs(header - req[i]);
        header = req[i];
    }

    gotoxy(x,y+8); printf("The total distance traveled by the header is: %d", d);

    Sleep(2000);
    confirmRetry(x, y+11);
}

void SSTP(){
    int x=5, y=3;
    int d=0, minKey, minVal, minPrior;

    input(x,y,"SSTP");

    for (int i = 0; i < cnt; i++){
        minVal = 200;
        minPrior = 999;

        for (int j = 0; j < cnt; j++){
            if (minVal > fabs(header - req[j]) && req[j] != -1){
                minKey = j;
                minVal = fabs(header - req[j]);
                minPrior = prior[j];
            }
            else if (minVal == fabs(header - req[j]) && req[j] != -1){
                if (minPrior > prior[j]){
                    minKey = j;
                    minVal = fabs(header - req[j]);
                    minPrior = prior[j];
                }
            }
        }
        d += minVal;
        header = req[minKey];
        req[minKey] = -1;
    }

    gotoxy(x,y+10); printf("The total distance traveled by the header is: %d", d);

    Sleep(2000);
    confirmRetry(x, y+13);
}

void SCAN(){
    int x=5, y=3;
    int d=0, minKey=200, minVal=999;

    input(x,y,"SCAN");

    sort();
    for (int i = 0; i < cnt; i++){
        if (dir == 0 && req[i] <= header){
            if (minVal > fabs(header - req[i])){
                minKey = i;
                minVal = fabs(header - req[i]);
            }
        }
        else if (dir == 199 && req[i] >= header){
            if (minVal > fabs(header - req[i])){
                minKey = i;
                minVal = fabs(header - req[i]);
            }
        }
    }

    if (dir == 0){
        for (int i = minKey; i >= 0; i--){
            d += fabs(header - req[i]);
            header = req[i];
        }

        d += header;
        header = 0;
        for (int i = minKey+1; i < cnt; i++){
            d += fabs(header - req[i]);
            header = req[i];
        }
    }
    else if (dir == 199){
        for (int i = minKey; i < cnt; i++){
            d += fabs(header - req[i]);
            header = req[i];
        }

        d += 199 - header;
        header = 199;
        for (int i = minKey-1; i >= 0; i--){
            d += fabs(header - req[i]);
            header = req[i];
        }
    }
    else return;

    gotoxy(x,y+10); printf("The total distance traveled by the header is: %d", d);

    Sleep(2000);
    confirmRetry(x, y+13);
}

void C_SCAN(){
    int x=5, y=3;
    int d=0, minKey=200, minVal=999;

    input(x,y,"C-SCAN");

    sort();
    for (int i = 0; i < cnt; i++){
        if (dir == 0 && req[i] <= header){
            if (minVal > fabs(header - req[i])){
                minKey = i;
                minVal = fabs(header - req[i]);
            }
        }
        else if (dir == 199 && req[i] >= header){
            if (minVal > fabs(header - req[i])){
                minKey = i;
                minVal = fabs(header - req[i]);
            }
        }
    }

    if (dir == 0){
        for (int i = minKey; i >= 0; i--){
            d += fabs(header - req[i]);
            header = req[i];
        }

        d += header + 199;
        header = 199;
        for (int i = cnt-1; i > minKey; i--){
            d += fabs(header - req[i]);
            header = req[i];
        }
    }
    else if (dir == 199){
        for (int i = minKey; i < cnt; i++){
            d += fabs(header - req[i]);
            header = req[i];
        }

        d += (199 - header) + 199;
        header = 0;
        for (int i = 0; i < minKey; i++){
            d += fabs(header - req[i]);
            header = req[i];
        }
    }
    else return;

    gotoxy(x,y+10); printf("The total distance traveled by the header is: %d", d);

    Sleep(2000);
    confirmRetry(x, y+13);
}

void input(int x, int y, char txt[]){
    setCursorType(NORMALCURSOR);

    gotoxy(x, y);  printf("◇ %s ◇", txt);
    gotoxy(x,y+2); printf("Input the current location of the header > ");
    scanf("%d", &header);

    gotoxy(x,y+4); printf("Input the number of requests > ");
    scanf("%d", &cnt);

    gotoxy(x,y+6); printf("Input %d requests > ", cnt);
    for (int i = 0; i < cnt; i++){
        scanf("%d", &req[i]);
    }

    if (txt == "SSTP"){
        gotoxy(x,y+8); printf("Input %d requests' priority > ", cnt);
        for (int i = 0; i < cnt; i++){
            scanf("%d", &prior[i]);
        }
    }

    if (txt == "SCAN" || txt == "C-SCAN"){
        gotoxy(x,y+8); printf("Input direction of the header(0 or 199) > ");
        scanf("%d", &dir);

        if (dir != 0 && dir != 199){
            system("cls");
            gotoxy(x, y); printf("Invaild direction key...");
            confirmRetry(x,y+3);
        }
    }

    setCursorType(NOCURSOR);
}

void confirmRetry(int x, int y){
    gotoxy(x,y); printf("Press 'y' to try again, 'n' to exit.");

    while (kbhit()) getch();
    char e = getch();
    if (e == 'y') reset();
    else system("exit");
}

void reset(){
    system("cls");
    homepage();
}
//전희준 다녀감
