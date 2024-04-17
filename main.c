#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

int lock = 0;
int gameover = 0;
int level = 0; 
int score = 0;
int width, height;
int m,n;
HANDLE consoleHandle;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

void hidecursor(){
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void gotoxy(int x, int y){
    COORD coordinates;
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(consoleHandle, coordinates);
}

int checkRegion(int x){
    if(x>=2&&x<=9 || x<=width-3&&x>=width-12){
        return 1;
    }
    return 0;
}

void scorecard(int a){
    if (a == 1) {
        gotoxy(2, 2);
        printf("Level %2d", level);
    }
    gotoxy(width - 12, 2);
    printf("Score %3d", score);
}

void move(int x){    
    for(int i=0;i<height - 5;i++){
        if(i==2 && checkRegion(x)){
            continue;
        }
        if(!lock){
            lock = 1;
            gotoxy(x,i);
            printf("0");
            Sleep(200-20*level);
            gotoxy(x,i);
            printf(" ");
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
        gotoxy(i,height-5);
        printf("|");
    }
    printf(" ");
}


void *input_thread(void *arg) {
    char input;
    while (!gameover){
        if(lock){
            continue;
        }
        input = getch();   
        if(input=='a'){
            if(m>0){
                m--;n--;
                lock=1;
                createPlatform();
                lock =0;
            }
            else{
                continue;
            }
        }
        else if(input=='d'){
            if (n<width){
                m++;n++;
                lock=1;
                createPlatform();
                lock=0;
            }
            else{
                continue;
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
    while(1){
        if(getch()==13){
            break;
        }
    }
}

int main() {
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    hidecursor();
    GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
    width = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
    height = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;

    time_t t1;
    srand ( (unsigned) time (&t1));

    m = width/2 - 4;
    n = width/2 + 6;
    createPlatform();
    scorecard(1);

    pthread_t tid;
    pthread_create(&tid, NULL, input_thread, NULL);

    while(!gameover){
        move(rand()%width);
        if(score/10 != level){
            level = score/10;
            scorecard(1);            
        }
        else{
            scorecard(0);
        }
    }
    
    pthread_join(tid, NULL);
    gameend();
    return 0;
}
