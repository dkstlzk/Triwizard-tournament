#include <stdio.h> 
#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ALL_GAMES_H.h"


// defining constants for maze dimensions and states that a cell can have
#define WIDTH 21
#define HEIGHT 15
#define CELLSIZE 40
#define WALL 1
#define PATH 0
#define START 2
#define END 3

// dynamically allocated maze grid
int **maze;
float lastwallmovetime= 0;
float wallmoveinterval= 0.3;

float lastgoalmovetime= 0;
float goalmoveinterval= 6.0;

int goalX= WIDTH-2;    // initial goal position
int goalY= HEIGHT-2;

float gamestarttime = 0;
float gameduration = 120.0;     // game duration in secs (2 mins)
bool gameover = false;
bool gamelost = false;


// dynamically allocated player
typedef struct{
    int x, y;   // coordinates of player's position 
} Player;

Player *player;
bool gamewon= false; 

void initmaze(){
    // walls initialization in maze
    for (int y=0; y<HEIGHT; y++){
        for (int x=0; x<WIDTH; x++){
            maze[y][x]= WALL;
        }
    }
}

// generating maze using random algorithm 
void genmaze(){
    initmaze();    // initialize maze with walls 
    int visitedcells=1;
    int totalcells= ((WIDTH-1)/2)*((HEIGHT-1)/2);    // total no of cells in maze
    int x= 1+(2*(rand()%((WIDTH-1)/2)));     // start x-coord
    int y= 1+(2*(rand()%((HEIGHT-1)/2)));      //start y-coord
    maze[y][x]= PATH;         

    while (visitedcells<totalcells){
        int dirs[4][2]= {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};
        int randomdir= rand()%4;       // randomly select one direction
        int nx= x+dirs[randomdir][0];      // new x-coord
        int ny= y+dirs[randomdir][1];       // new y-coord 

        // check if new position is valid, and then make new path
        if (nx>0 && nx<WIDTH-1 && ny>0 && ny<HEIGHT-1 && maze[ny][nx]==WALL){
            maze[ny][nx]=PATH;       // new position becomes path
            maze[y+dirs[randomdir][1]/2][x+dirs[randomdir][0]/2]=PATH;    // in between positions becomes path
            visitedcells++;      
        }

        // a new start position is picked randomly 
        x= 1+(2*(rand()%((WIDTH-1)/2)));
        y= 1+(2*(rand()%((HEIGHT-1)/2)));
    }

    maze[1][1]= START;      // start pos
    goalX= WIDTH-2;         // initial goal pos
    goalY= HEIGHT-2;        
    maze[goalY][goalX]= END;       // end pos set
}

// counting number of walls adjacent to a position
int adjwallcount(int x, int y){
    int count= 0;
    int dirs[4][2]= {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    for (int i=0; i<4; i++){
        int nx= x+dirs[i][0];    // new x-coord
        int ny= y+dirs[i][1];     // new y-coord
        if (nx>=0 && nx<WIDTH && ny>=0 && ny<HEIGHT && maze[ny][nx]==WALL)
            count++;    // count++ if a wall is found
    }
    return count;  
}
// setting new goal/endpoint
void newgoalpos(){
    float timenow= GetTime();
    if (timenow-lastgoalmovetime >= goalmoveinterval){
        int bestX= goalX, bestY= goalY;
        float bestscore= -9999.0;  
        int tries= 30;   // no of attempts to find a new goal pos


        // quad means quadrant wherever it is used

        // determine current quad of player
        int currentquadX= player->x < WIDTH/2 ? 0 : 1;
        int currentquadY= player->y < HEIGHT/2 ? 0 : 1;

        for (int i=0; i<tries; i++){
            int x= rand()%(WIDTH-2)+1;      // random coords for new goal
            int y= rand()%(HEIGHT-2)+1;

            if (maze[y][x]!=PATH || (x==player->x && y==player->y)) continue;

            // calc dist from player
            float dist= sqrtf((float)((x-player->x)*(x-player->x)+(y-player->y)*(y-player->y)));
            if (dist<4) continue;      // skip if goal is too close to the player 

            int wallsnearby= adjwallcount(x, y);       // count no of walls nearby/adjacent


            //find quadrant of new goal pos
            int newquadX= x<WIDTH/2 ? 0 : 1;
            int newquadY= y<HEIGHT/2 ? 0 : 1;
            int quadbonus= (newquadX!=currentquadX || newquadY!=currentquadY) ? 5 : 0;    // bonus for moving to a new quad


            //calc score for new pos
            float score=dist-(wallsnearby*2)+quadbonus;

            // update best score and pos if reqd
            if (score>bestscore){
                bestscore= score;
                bestX= x;
                bestY= y;
            }
        }

        maze[goalY][goalX]= PATH;     // set old goal pos as path now
        maze[bestY][bestX]= END;        // set new goal pos as end 
        goalX= bestX;
        goalY= bestY;
        lastgoalmovetime= timenow;       //update teh time the goal was last moved
    }
}

void updatewalls(){
    float timenow= GetTime();
    if (timenow-lastwallmovetime>=wallmoveinterval){
        for (int y=1; y<HEIGHT-1; y++){
            for (int x=1; x<WIDTH-1; x++){
                if ((x==player->x && y==player->y) || (x==goalX && y==goalY)) continue;          // skip if pos is occupied by player or goal

                float chance= (float)rand()/RAND_MAX;         // random chance to toggle the wall state
                if (maze[y][x]==WALL && chance<0.2f){            
                    maze[y][x]= PATH;                         // change wall to path w 20% chance 
                } else if (maze[y][x]==PATH && chance<0.2f){
                    maze[y][x]= WALL;                   // change path to wall w 20% chance
                }
            }
        }
        lastwallmovetime= timenow;             // update time
    }
}

// move player by dx and dy
void moveplayer(int dx, int dy){
    int newX= player-> x+dx;
    int newY= player-> y+dy;
    if (maze[newY][newX]!=WALL){         // check if new pos isnt a wall 
        player->x =newX;
        player->y =newY;
    }
}

void drawmaze(){
    for (int y=0; y<HEIGHT; y++){
        for (int x=0; x<WIDTH; x++) {
            Color color= (maze[y][x]==WALL) ? DARKGRAY : LIGHTGRAY;
            if (maze[y][x]==START) color=GREEN;
            if (maze[y][x]==END) color=RED;

            // draw current cell
            DrawRectangle(x*CELLSIZE, y*CELLSIZE, CELLSIZE, CELLSIZE, color); 
            if (maze[y][x]==WALL)
                DrawRectangleLines(x*CELLSIZE, y*CELLSIZE, CELLSIZE, CELLSIZE, BLACK);
        }
    }
    DrawCircle(goalX*CELLSIZE + CELLSIZE/2, goalY*CELLSIZE + CELLSIZE/2, CELLSIZE/3, Fade(RED, 0.3));
}

void drawplayer(){ 
    DrawCircle(player-> x*CELLSIZE + CELLSIZE/2, player->y *CELLSIZE + CELLSIZE/2, CELLSIZE/3, BLUE);  // circular blue ball
    DrawCircleLines(player-> x*CELLSIZE + CELLSIZE/2, player->y *CELLSIZE + CELLSIZE/2, CELLSIZE/3, BLACK); // ball outline
}

bool checkwin(){
    return (player->x == goalX && player->y == goalY);
}

void countdown(){
    if (gamewon || gamelost) return;

    float timenow= GetTime();
    float elapsedtime= timenow-gamestarttime;
    float timeleft= gameduration-elapsedtime;

    char timertext[20];
    sprintf(timertext, "Time Left: %.0fs", timeleft);
    DrawText(timertext, 10, 10, 20, WHITE);       //display countdown

    if (timeleft<=0){
        gameover= true;
        gamelost= true;
    }
}

void drawendscreen(){
    if (!gamewon && !gamelost) return;

    const char* message= gamewon ? "YOU WIN!" : "TIME'S UP! YOU LOSE.";
    Color msgcolor= gamewon ? GREEN : RED;

    DrawText(message, (WIDTH*CELLSIZE/2)-120, HEIGHT*CELLSIZE/2 -40, 40, msgcolor);
    DrawText("Press R to Restart or ESC to Exit", (WIDTH*CELLSIZE/2) - 160, HEIGHT*CELLSIZE/2 +10, 20, WHITE);
}


int maze_game(){
    srand(time(NULL));
    //InitWindow(WIDTH*CELLSIZE, HEIGHT*CELLSIZE, "Labyrinth Maze Game");

    // DMA for maze and player
    maze = (int **)malloc(HEIGHT*sizeof(int *));
    for (int i=0; i<HEIGHT; i++){
        maze[i]= (int *)malloc(WIDTH*sizeof(int));
    }

    player= (Player *)malloc(sizeof(Player));

    genmaze();
    player-> x=1;
    player-> y=1;

    SetTargetFPS(60);    // set FPS as 60
    gamestarttime = GetTime();

    while (!WindowShouldClose()){
        if (!gameover){
            if (IsKeyPressed(KEY_UP)) moveplayer(0, -1);
            if (IsKeyPressed(KEY_DOWN)) moveplayer(0, 1);
            if (IsKeyPressed(KEY_LEFT)) moveplayer(-1, 0);
            if (IsKeyPressed(KEY_RIGHT)) moveplayer(1, 0);

            updatewalls();
            newgoalpos();

            if (checkwin()){
                gamewon = true;
                gameover = true;
                DrawText("press esc",200,350,20,WHITE);

            }

        } else {
            if (IsKeyPressed(KEY_R)){
                genmaze();
                player->x =1;
                player->y =1;
                gamewon= false;
                gamelost= false;
                gameover= false;
                gamestarttime= GetTime();
            }
        }


        BeginDrawing();
        ClearBackground(DARKGRAY);
        drawmaze();
        drawplayer();
        drawendscreen();
        countdown();  // display countdown
        EndDrawing();
    }

    // free DMA
    for (int i=0; i<HEIGHT; i++){
        free(maze[i]);
    }
    free(maze);
    free(player);

    if(gamewon){
        return 1;
    }else if(gamelost){
        return 0;
    }
    //CloseWindow(); 
}
