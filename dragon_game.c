#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ALL_GAMES_H.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CELL_SIZE 40
#define GRID_WIDTH (SCREEN_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / CELL_SIZE)
#define MAX_KEYS 5

int frameCounter = 0;   
bool won = false;
bool lost = false;

typedef struct {
    Vector2 pos;
    int lives;
}player1r;

typedef struct fireballnode {
    Vector2 pos;
    Vector2 direction;
    bool active;
    struct fireballnode* next;
}fireballnode;

typedef struct obstNode {
    Vector2 pos;
    struct obstNode* next;
}obstNode;

typedef struct keynode {
    Vector2 pos;
    bool ifcollected;
    struct keynode* next;
}keynode;


// Game elements
player1r player1 = {{0, 0}, 3};
Vector2 dragon = {(float)(GRID_WIDTH - 1), (float)(GRID_HEIGHT - 1)};
Vector2 egg ={(float)(GRID_WIDTH - 1), (float)(GRID_HEIGHT / 2)}; 
fireballnode* fireballs = NULL;                         // head of linkedLfor fireballs
obstNode* obstacles = NULL;                              // head of linkedL for obstacles
keynode* keys = NULL;                                     // head of linkedL for keys


void add_obst(Vector2 pos) {     //LinledL of obstacless
    obstNode* new =(obstNode*)malloc(sizeof(obstNode));
    new->pos= pos;
    new->next= NULL;
    if (obstacles== NULL) {
        obstacles= new;
    } else {
        obstNode* temp= obstacles;
        while (temp->next!= NULL) {
            temp= temp->next;
        }
        temp->next= new;
    }
}

void add_key(Vector2 pos) {
    keynode* new = (keynode*)malloc(sizeof(keynode));
    new->pos =pos;
    new->ifcollected =false;
    new->next=NULL;
    if (keys ==NULL) {
        keys =new;
    } else {
        keynode* temp= keys;
        while (temp->next !=NULL) {
            temp=temp->next;
        }
        temp->next = new;
    }
}

void player1_movement(Vector2 dir) {          //basically checking  playermovement with condns
    if (lost || won) {
        return;
    }
    Vector2 newpos =Vector2Add(player1.pos, dir);
    if (newpos.x < 0 || newpos.y < 0 || newpos.x >= GRID_WIDTH || newpos.y >= GRID_HEIGHT) {
        return; 
    }
    obstNode* obst=obstacles;
    while (obst != NULL) {
        if (Vector2Equals(newpos, obst->pos)) {
            return; 
        }
        obst= obst->next;
    }
    player1.pos = newpos;
}

void dragon_movement() {
    if (frameCounter < 120) { 
        return;              //so that dragon starts late
    }
    if (frameCounter % 30 ==0) {
        Vector2 diff =Vector2Subtract(player1.pos, dragon);
        if (fabs(diff.x) > fabs(diff.y)) {
            if(diff.x > 0) {
                dragon.x += 1;
            } else {
                dragon.x += -1;
            }
        } else {
            if(diff.y > 0) {
                dragon.y += 1;
            } else {
                dragon.y += -1;
            }
        }
    }
}

bool hitobst(Vector2 obstpos) {       // fireball hitting obst
    obstNode* temp=obstacles;
    while (temp != NULL) {
        if ((int)obstpos.x ==(int)temp->pos.x && (int)obstpos.y ==(int)temp->pos.y) {  // int bcz even if it touches the boundary returns true
            return true;
        }
        temp =temp->next;
    }
    return false;
}


void makefireball() {                   //linkedL of fireball
    Vector2 sub= Vector2Subtract(player1.pos, dragon);
    Vector2 dir= Vector2Normalize(sub);

    fireballnode* new= (fireballnode*)malloc(sizeof(fireballnode));
    new->pos =dragon;
    new->direction= dir;
    new->active =true;
    new->next=NULL;

    if (fireballs == NULL) {
        fireballs = new;
    } else {
        fireballnode* temp =fireballs;
        while (temp->next != NULL) {
            temp =temp->next;
        }
        temp->next= new;
    }
}

void shootfireball() {
    fireballnode* current =fireballs;                         //condn fireball shoot karne ke liye
    while (current != NULL) {
        if (current->active) {
            current->pos =Vector2Add(current->pos, current->direction);

            if (hitobst(current->pos)) {
                current->active=false;
            }

            if (current->pos.x < 0 ||current->pos.x >= GRID_WIDTH ||current->pos.y < 0 ||current->pos.y >= GRID_HEIGHT) {
                current->active=false;
            }

            if ((int)player1.pos.x ==(int)current->pos.x && (int)player1.pos.y ==(int)current->pos.y){
                current->active=false;
                player1.lives--;
                if (player1.lives <= 0) lost = true;
            }
        }
        current = current->next;
    }
}

bool collected_allkey() {         // to check allkeys collected or not
    keynode* key=keys;
    while (key!= NULL) {
        if (!key->ifcollected){
            return false;}
        key =key->next;
    }
    return true;
}

int dragon_game() {
    //Initwondow(SCREEN_WIDTH, SCREEN_HEIGHT, "Get the Golden Egg");
    SetTargetFPS(60);
    srand(time(NULL));                 // for generating random values
    //adding the obstacles
    add_obst((Vector2){6, 4});
    add_obst((Vector2){10, 7});
    add_obst((Vector2){3, 10});
    add_obst((Vector2){8, 1});
    add_obst((Vector2){1, 5});
    add_obst((Vector2){15, 11});
    add_obst((Vector2){16, 3});
    add_obst((Vector2){9, 13});

    // adding key positions
    for (int i = 0; i < MAX_KEYS; i++) {
        bool valid = false;
        Vector2 pos;
        while (!valid) {      // positions check key ke liye
            pos.x = GetRandomValue(1, GRID_WIDTH - 2);
            pos.y = GetRandomValue(1, GRID_HEIGHT - 2);
            valid = true;

            if (Vector2Equals(pos, player1.pos)||Vector2Equals(pos, dragon)|| Vector2Equals(pos, egg)){
                valid = false;
            }

            obstNode* obstac = obstacles;
            while (obstac != NULL) {
                if (Vector2Equals(pos, obstac->pos)){
                    valid = false;}
                obstac = obstac->next;
            }

            keynode* currentKey = keys;
            while (currentKey != NULL) {
                if (Vector2Equals(pos, currentKey->pos)){
                    valid = false;}
                currentKey = currentKey->next;
            }
        }
        add_key(pos);  // Add key to the linked list
    }

    while (!WindowShouldClose()) {            // drawinggg starts
        BeginDrawing();
        ClearBackground(BLACK);

        frameCounter++;
        if (!lost && !won) {
            if(IsKeyPressed(KEY_UP)){
                player1_movement((Vector2){0, -1});}
            if(IsKeyPressed(KEY_DOWN)){
                player1_movement((Vector2){0, 1});}
            if(IsKeyPressed(KEY_LEFT)){
                player1_movement((Vector2){-1, 0});}
            if(IsKeyPressed(KEY_RIGHT)){
                player1_movement((Vector2){1, 0});}

            if(frameCounter % 90 == 0 && frameCounter > 120){     //fireballs shoot only after ometime
                makefireball();
            }
            shootfireball();
            dragon_movement();

            keynode* k1 = keys;
            while(k1 != NULL) {
                if(Vector2Equals(player1.pos, k1->pos) && k1->ifcollected==false) {
                    k1->ifcollected = true;
                }
                k1 =k1->next;
            }

            if(Vector2Equals(player1.pos, egg) && collected_allkey()){
                won=true;
            }
            if(Vector2Equals(player1.pos, dragon)){
                lost =true;
            }
        }

        DrawRectangle(player1.pos.x * CELL_SIZE, player1.pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
        DrawRectangle(dragon.x * CELL_SIZE, dragon.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);
        DrawEllipse(egg.x * CELL_SIZE + CELL_SIZE / 2, egg.y * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE / 2, CELL_SIZE * 0.6, GOLD);

        // draw fireballs
        fireballnode* currentFireball = fireballs;
        while (currentFireball != NULL){
            if(currentFireball->active){
                DrawCircle(currentFireball->pos.x * CELL_SIZE + CELL_SIZE / 2,currentFireball->pos.y * CELL_SIZE + CELL_SIZE / 2, 5, ORANGE);
            }
            currentFireball=currentFireball->next;
        }

        //obstacles drawong
        obstNode* obs =obstacles;
        while (obs!=NULL){
            DrawRectangle(obs->pos.x * CELL_SIZE, obs->pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GRAY);
            obs=obs->next;
        }

        //keys draw
        keynode* key=keys;
        while (key!=NULL) {
            if(key->ifcollected==false){
                DrawCircle(key->pos.x * CELL_SIZE + CELL_SIZE / 2,key->pos.y * CELL_SIZE + CELL_SIZE / 2, 6, YELLOW);
            }
            key= key->next;
        }

        DrawText(TextFormat("Lives: %d", player1.lives), 10, 10, 20, WHITE);

        int collectedCount=0;           // display ke liye
        keynode* keyy=keys;
        while(keyy !=NULL) {
            if (keyy->ifcollected){
                collectedCount++;
            }
            keyy=keyy->next;
        }

        DrawText(TextFormat("Keys Collected: %d / %d", collectedCount, MAX_KEYS), 10, 40, 20, YELLOW);

        if(won){
            DrawText("You got the egg! You won!", 200, 300, 30, GREEN);
            DrawText("press esc",200,350,20,WHITE);
        }
        if(lost){
            DrawText("You were caught! Game Over!", 200, 300, 30, RED);
            DrawText("press esc",200,350,20,WHITE);
            //
        }
        EndDrawing();
    }
    fireballnode* tempfireball;
    while(fireballs!= NULL){
        tempfireball=fireballs;
        fireballs=fireballs->next;
        free(tempfireball);
    }

    obstNode* tempobst;
    while (obstacles !=NULL){
        tempobst=obstacles;
        obstacles=obstacles->next;
        free(tempobst);
    }
    keynode* tempkey;
    while(keys!= NULL){
        tempkey=keys;
        keys =keys->next;
        free(tempkey);
    }
    //Closewondow();
    if(won){
        return 1;
    }else if(lost){
        return 0;
    }
}
