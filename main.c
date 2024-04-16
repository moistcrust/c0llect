#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>


int lock = 0;
int request = 0;
int gameover = 0;
int speed, score = 0;
int width, height;
int m,n;

void hidecursor(){
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void gotoxy(int x, int y){
    COORD coordinates;
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

void deletechar(int x, int y){
    gotoxy(x,y);
    printf(" ");
}

void move(int x){
    for(int i=0;i<height - 5;i++){
        speed = 200-score*10;
        if(!lock){
            lock = 1;
            gotoxy(x,i);
            printf("0");
            Sleep(speed);
            deletechar(x,i);
            lock = 0;
        }
        else{
            i--;
        }
    }
    if(x>=m && x<=n){
        score++;
    }
    else{
        gameover = 1;
    }
}

void createPlatform(){
    gotoxy(m-1,height-5);
    printf(" ");
    for(int i=m;i<n;i++){
        printf("|");
    }
    printf(" ");
}

void *input_thread(void *arg) {
    char input;
    while (1){
        input = getch();
        if(input=='a'){
            if(m>0){
                m--;n--;
                lock = 1;
                createPlatform();
                lock =0;
            }
            if(gameover){
                break;
            }
        }
        else if(input=='d'){
            if (n<width){
                m++;n++;
                lock=1;
                createPlatform();
                lock=0;
            }
            if(gameover){
                break;
            }
        }
    }
    return NULL;
}

void gameend(){
    system("cls");
    gotoxy(width/2 -5,height/2);
    printf("Game Over");
    gotoxy(width/2 - 8,height/2 +2);
    printf("You score was %d",score);
    while(getch()=='\n'){
        continue;
    }
}

int main() {
    hidecursor();
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
    width = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
    height = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;

    time_t t1;
    srand ( (unsigned) time (&t1));

    m = width/2 - 4;
    n = width/2 + 6;
    createPlatform();

    pthread_t tid;
    pthread_create(&tid, NULL, input_thread, NULL);

    while(!gameover){
        move(rand()%width);    
    }
    
    pthread_join(tid, NULL);
    gameend();
    getch();
    return 0;
}