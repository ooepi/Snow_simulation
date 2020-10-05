#include <stdio.h>
#include <ncurses.h>
#include <windows.h>
#include <time.h>

#define HEIGHT 50
#define WIDTH 150
#define RATE 1          //lower is faster
#define DROPLET "*"
#define RANDOMRAIN 1     //1 for on, 0 for off
#define RANDOMBOXES 15   //amount of boxes
#define COLORS 1         //1 for on, 0 for off
#define FRAME 1          //1 fonr on, 0 for off



struct cell
{
    int current;
    int future;
    int stationary;
};

void randomBoxes(struct cell screen[HEIGHT][WIDTH]);
void frameScreen(struct cell screen[HEIGHT][WIDTH]);
void init(void);


int main(void){

    int counter = 0;
    struct cell screen[HEIGHT][WIDTH] = {0,0};

    init();

    int framechoice = FRAME;
    if(framechoice == 1){
        frameScreen(screen);
    }
    
    randomBoxes(screen);


    


    while(1){

        if(RANDOMRAIN == 0){
            if(counter == RATE){
                screen[1][WIDTH/2].current = 1;
                counter = 0;
            }
        }
        if(RANDOMRAIN == 1){
            if(counter == RATE){
                int r = rand()%WIDTH;
                screen[1][r].current = 1;
                counter = 0;
            }
        }

        for(int i = 0; i<HEIGHT-1; i++){
            for(int j = 0; j<WIDTH; j++){
                if(screen[i][j].current == 1){
                    if(screen[i+1][j].current == 0 && screen[i+1][j].stationary == 0 && j<WIDTH && j>0 && i<HEIGHT-1){
                        if(i+1==HEIGHT){
                            continue;
                        } else {
                            screen[i+1][j].future = 1;
                            screen[i][j].future = 0;
                        }
                    }
                    if(screen[i+1][j].current == 1 && screen[i+1][j-1].current == 1 && screen[i+1][j+1].future == 1 && j<WIDTH && j>0 && i<HEIGHT-1){
                        screen[i][j].future = 1;
                        continue;
                    }
                    if(screen[i+1][j].current == 1 && screen[i+1][j-1].current == 0 && screen[i+1][j+1].future == 0 && j<WIDTH && j>0 && i<HEIGHT-1){
                        if(screen[i][j+1].stationary == 0 && screen[i][j-1].stationary == 0){
                            if(j+1==WIDTH){
                                screen[i][j].future = 1;
                            } if(j-1==0){
                                screen[i][j].future = 1;
                            } else {
                                int r = rand()%2;
                            if(r==0){
                                r=-1;
                            }
                            screen[i+1][j+r].future = 1;
                            screen[i][j].future = 0;
                            }
                            
                        }
                        if(screen[i][j+1].stationary == 1 && screen[i][j-1].stationary == 0 && j<WIDTH && j>0 && i<HEIGHT-1){
                            screen[i+1][j-1].future = 1;
                            screen[i][j].future = 0;
                        }
                        if(screen[i][j+1].stationary == 0 && screen[i][j-1].stationary == 1 && j<WIDTH && j>0 && i<HEIGHT-1){
                            screen[i+1][j+1].future = 1;
                            screen[i][j].future = 0;
                        } 
                    }
                    if(screen[i+1][j].current == 1 && screen[i+1][j-1].current == 1 && screen[i][j+1].stationary == 0 && j<WIDTH && j>0 && i<HEIGHT-1){
                        screen[i+1][j+1].future = 1;
                        screen[i][j].future = 0;
                    }
                    if(screen[i+1][j].current == 1 && screen[i+1][j+1].current == 1 && screen[i][j-1].stationary == 0 && j<WIDTH && j>0 && i<HEIGHT-1){
                        screen[i+1][j-1].future = 1;
                        screen[i][j].future = 0;
                    }
                }
            }
        }

        for(int i = 0; i<HEIGHT; i++){
            for(int j = 0; j<WIDTH; j++){
                screen[i][j].current = screen[i][j].future;
            }
        }
        
        
        for(int i = 0; i<HEIGHT; i++){
            for(int j = 0; j<WIDTH; j++){
                if(screen[i][j].current == 1){
                    if(COLORS == 1){
                        attron(COLOR_PAIR(1));
                        mvprintw(i, j, DROPLET);
                        attroff(COLOR_PAIR(1));
                    } else {
                        mvprintw(i, j, DROPLET);
                    }
                    
                }
                if(screen[i][j].current == 0){
                    if(COLORS == 1){
                        attron(COLOR_PAIR(3));
                        mvprintw(i, j, " ");
                        attroff(COLOR_PAIR(3));
                    } else {
                        mvprintw(i, j, " ");
                    }
                }
                if(screen[i][j].stationary == 1){
                    if(COLORS == 1){
                        attron(COLOR_PAIR(2));
                        mvprintw(i, j, "=");
                        attroff(COLOR_PAIR(2));
                    } else {
                        mvprintw(i, j, "=");
                    }
                }
            }
        }
        refresh();
        Sleep(10);
        counter++;
    }
    endwin();
}

void randomBoxes(struct cell screen[HEIGHT][WIDTH]){

    for(int b = 0; b<RANDOMBOXES; b++){

        int boxHeight = rand()%6+2;
        int boxWidth = rand()%10+2;
        int boxPosY = rand()%HEIGHT;
        int boxPosX = rand()%WIDTH;

        if(boxPosX + boxWidth > WIDTH){
            int over = boxPosX + boxWidth - WIDTH;
            boxPosX = boxPosX - over;
        }
        if(boxPosY + boxHeight > HEIGHT){
            int over = boxPosY + boxHeight - HEIGHT +1;
            boxPosY = boxPosY - over;
        }

        for(int k = boxPosY; k<boxHeight+boxPosY; k++){
            for(int l = boxPosX; l<boxWidth+boxPosX; l++){
                screen[k][l].stationary = 1;
            }
        }
    }
}

void init(void){

    initscr();
    curs_set(0);
    srand(time(NULL));

    if(COLORS == 1){
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_WHITE); //Snow
        init_pair(2, COLOR_BLACK, COLOR_BLACK); //Boxes
        init_pair(3, COLOR_BLUE, COLOR_BLUE);   //Background
    }

}

void frameScreen(struct cell screen[HEIGHT][WIDTH]){

    //framing the screen to prevent spills

    for(int i = 0; i<WIDTH; i++){
        screen[HEIGHT-1][i].stationary = 1;
        screen[0][i].stationary = 1;
    }
    for(int i = 0; i<HEIGHT; i++){
        screen[i][WIDTH-1].stationary = 1;
        screen[i][0].stationary = 1;
    }

}

