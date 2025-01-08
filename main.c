#include <stdio.h>
#include <unistd.h>
#include <time.h>

// ekran
#define WIDTH 30
#define HEIGHT 25

#define HOLE 5 // razmak izmedju donjeg i gornjeg bloka
#define birdX 5
#define BLOCK_WIDTH 5
#define BLOCK_SPACE 12
#define JUMP 2

int birdY = HEIGHT/2;
char key = NULL;
int block1x = WIDTH*4/3;
int block2x = WIDTH*4/3 + BLOCK_WIDTH + BLOCK_SPACE;
int block1Bottom_height, block1Top_height, block2Bottom_height, block2Top_height;

int score = 0;

void clearScreen(){
    system("cls");
}

void moveCursor(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1); // stek overflou
}

int randomHeight(){ // generise random visinu bloka u rejndzu [1, HEIGHT - HOLE - 1)
    int min = 1;
    int max = HEIGHT - HOLE - 1;
    return min + rand() % (max - min + 1);
}

int sudar(){
    return (block1x < birdX && birdX <= block1x + BLOCK_WIDTH && (birdY < block1Top_height || birdY >= block1Top_height + HOLE))
            || (block2x < birdX && birdX <= block2x + BLOCK_WIDTH && (birdY < block2Top_height || birdY >=  block2Top_height + HOLE));
}

int max(int a, int b){
    if(a>b)
        return a;
    return b;
}

void stop(){
    moveCursor(0, HEIGHT);
    printf("Crkosi, skor ti je %d\n", score);
    usleep(2000*1000);
    exit(0);
}

int invalidPosition(){
    return birdY >= HEIGHT || birdY < 0 || sudar();
}

void render(){

    if (invalidPosition()){ // ako ptica izadje van ekrana ili se slupa
        stop();
    }

    // inkrement skora

    if(birdX == block1x + BLOCK_WIDTH || birdX == block2x + BLOCK_WIDTH)
        score++;

    // generisi random visine za oba para blokova, ako su van ekrana
    if(block1x + BLOCK_WIDTH <= 0 || block1x >= WIDTH){
        block1Bottom_height = randomHeight();
        block1Top_height = HEIGHT - block1Bottom_height - HOLE;
    }

    if(block2x + BLOCK_WIDTH <= 0 || block2x >= WIDTH){
        block2Bottom_height = randomHeight();
        block2Top_height = HEIGHT - block2Bottom_height - HOLE;
    }

    // okvir
    for(int y=1; y<=HEIGHT; ++y){
        for(int x=1; x<=WIDTH; ++x){
            printf(" ");
        }
        printf("\n");
    }
    for(int y=0; y<HEIGHT; y++){
        moveCursor(WIDTH, y);
        printf(".");
    }
    for(int x=0; x<=WIDTH; x++){
        moveCursor(x, HEIGHT-1);
        printf(".");
    }
    moveCursor(0, HEIGHT);
    printf("Score: %d", score);

    moveCursor(0, 0);

    // render 1. donjeg bloka

    for(int x=max(0, block1x); x < block1x + BLOCK_WIDTH && x < WIDTH; ++x){

        for(int y=HEIGHT-1; y>HEIGHT-block1Bottom_height-1; --y){
            moveCursor(x, y);
            printf("|");
        }

    }

    // render 1. gornjeg bloka

    for(int x=max(0, block1x); x < block1x + BLOCK_WIDTH && x < WIDTH; ++x){

        for(int y=0; y<block1Top_height; ++y){
            moveCursor(x, y);
            printf("|");
        }

    }

    // render 2. donjeg bloka

    for(int x=max(0, block2x); x < block2x + BLOCK_WIDTH && x < WIDTH; ++x){

        for(int y=HEIGHT-1; y>HEIGHT-block2Bottom_height-1; --y){
            moveCursor(x, y);
            printf("|");
        }

    }

    // render 2. gornjeg bloka

    for(int x=max(0, block2x); x < block2x + BLOCK_WIDTH && x < WIDTH; ++x){

        for(int y=0; y<block2Top_height; ++y){
            moveCursor(x, y);
            printf("|");
        }

    }

    if(block1x + BLOCK_WIDTH <= 0)
        block1x = WIDTH;
    if(block2x + BLOCK_WIDTH <= 0)
        block2x = block1x + BLOCK_WIDTH + BLOCK_SPACE;

    // pritisnuto nesto -> skoci za 5
    if((key) != NULL){
        for(int jump=1; jump<=JUMP; ++jump){
            birdY -= jump;
            if(invalidPosition()){
                moveCursor(birdX, birdY);
                printf("#");
                stop();
            }

        }
        //birdY -= JUMP;
        (key) = NULL;
    }
    else
        birdY += 1;

    // render ptice
    moveCursor(birdX, birdY);
    printf("#");

    block1x -= 1;
    block2x -= 1;
}

int main()
{
    clearScreen();
    srand(time(NULL));

    while (1){
        if (_kbhit()){
            key = _getch();
        }
        render();
        moveCursor(0,0);
        usleep(120*1000); // cekaj 120 ms
    }
    return 0;
}
