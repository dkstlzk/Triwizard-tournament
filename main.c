#include "raylib.h"
#include "ALL_GAMES_H.h"

typedef enum{
    MENU,
    HELP,
    LIST,
    GAME1,
    GAME2,
    GAME3,
    TRANSITION,
    END,
    EXIT
} GameScreen;

int main(void){
    const int screenwidth=800;
    const int screenheight=600;
    InitWindow(screenwidth, screenheight, "THE TRIWIZARD TOURNAMENT");

    GameScreen currentscreen= MENU;
    int lastgame=0;
    bool lastgamewon= false;
    bool game1won= false;
    bool game2won= false;
    bool game3won= false;

    SetTargetFPS(60);

    while (!WindowShouldClose() && currentscreen!=EXIT){
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentscreen){
            case MENU:
                DrawText("THE TRIWIZARD TOURNAMENT", 95, 80, 40, YELLOW);
                DrawText("Press [ENTER] to Start", 260, 180, 20, RAYWHITE);
                DrawText("Press [H] for Controls", 260, 220, 20, LIGHTGRAY);
                DrawText("Press [ESC] to Quit", 280, 260, 20, RAYWHITE);
            
                if (IsKeyPressed(KEY_ENTER)) currentscreen= LIST;
                if (IsKeyPressed(KEY_H)) currentscreen= HELP;
                if (IsKeyPressed(KEY_ESCAPE)) currentscreen= EXIT;
                break;
        
            case HELP:
                DrawText("Game Controls", 280, 60, 30, SKYBLUE);
            
                DrawText(" Dragon Game:", 100, 120, 20, ORANGE);
                DrawText("Use Arrow Keys to move.", 120, 150, 18, RAYWHITE);
                DrawText("Collect keys and reach the egg while avoiding the dragon.", 120, 170, 18, RAYWHITE);
            
                DrawText("Snitch Escape:", 100, 210, 20, ORANGE);
                DrawText("Use Arrow Keys to move the Snitch.", 120, 240, 18, RAYWHITE);
                DrawText("Avoid Death Eaters and collect all pellets.", 120, 260, 18, RAYWHITE);
            
                DrawText("Enchanted Maze:", 100, 300, 20, ORANGE);
                DrawText("Use Arrow Keys to navigate.", 120, 330, 18, RAYWHITE);
                DrawText("Reach the moving goal before time runs out.", 120, 350, 18, RAYWHITE);
                DrawText("Walls change positions randomly!", 120, 370, 18, RAYWHITE);
            
                DrawText("Press [B] to go back.", 280, 440, 20, LIGHTGRAY);
            
                if (IsKeyPressed(KEY_B)) currentscreen= MENU;
                break;
        

            case LIST:
                DrawText("1. Get the Dragon's Egg", 250, 180, 25, PURPLE);
                DrawText("2. Snitch Escape", 250, 230, 25, PURPLE);
                DrawText("3. Enchanted Maze", 250, 280, 25, PURPLE);
                DrawText("Press [ENTER] to begin your first task...", 180, 360, 20, RAYWHITE);
                if (IsKeyPressed(KEY_ENTER)) currentscreen= GAME1;
                break;

            case GAME1:
                lastgame=1;  // Set before transition
                lastgamewon= dragon_game();
                game1won= lastgamewon;
                currentscreen= TRANSITION;
                break; 
            
            case GAME2:
                lastgame=2;
                lastgamewon= snitch_escape();
                game2won= lastgamewon;
                currentscreen= TRANSITION;
                break;
            
            case GAME3:
                lastgame=3;
                lastgamewon= maze_game();
                game3won= lastgamewon;
                currentscreen=TRANSITION;
                break;
            
            

            case TRANSITION:
                if (lastgame==1){
                    DrawText("Dragon Task Complete!", 240, 180, 30, ORANGE);
                    DrawText(lastgamewon ? "You got the egg!" : "You were caught by the dragon!", 200, 230, 25, lastgamewon ? GREEN : RED);
                    DrawText("Press [ENTER] to continue...", 230, 300, 20, RAYWHITE);
                } else if (lastgame==2){
                    DrawText("Snitch Task Complete!", 240, 180, 30, ORANGE);
                    DrawText(lastgamewon ? "You escaped with the Snitch!" : "The snitch outsmarted you!", 200, 230, 25, lastgamewon ? GREEN : RED);
                    DrawText("Press [ENTER] to continue...", 230, 300, 20, RAYWHITE);
                } else if (lastgame==3){
                    DrawText("Maze Task Complete!", 240, 180, 30, ORANGE);
                    DrawText(lastgamewon ? "You survived the maze!" : "The maze trapped you forever!", 220, 230, 25, lastgamewon ? GREEN : RED);
                    DrawText("Press [ENTER] to see your final result...", 180, 300, 20, RAYWHITE);
                }
            
                if (IsKeyPressed(KEY_ENTER)){
                    if (lastgame==1) currentscreen= GAME2;
                    else if (lastgame==2) currentscreen= GAME3;
                    else if (lastgame==3) currentscreen= END;
                }
                break;


            case END:
                if (game1won && game2won && game3won){
                    DrawText("Congratulations, Champion!", 180, 200, 40, GREEN);
                    DrawText("You won the Triwizard Tournament Cup!", 160, 260, 20, RAYWHITE);
                } else{
                    DrawText("The Tournament is Over.", 230, 200, 35, RED);
            
                    int Yoffset=300;
                    if (game1won){
                        DrawText("1. Dragon Game: Won", 200, Yoffset, 20, GREEN);
                    } else{
                        DrawText("1. Dragon Game: Lost", 200, Yoffset, 20, RED);
                    }
                    Yoffset+=30;
            
                    if (game2won){
                        DrawText("2. Snitch Escape: Won", 200, Yoffset, 20, GREEN);
                    } else{
                        DrawText("2. Snitch Escape: Lost", 200, Yoffset, 20, RED);
                    }
                    Yoffset+=30;
            
                    if (game3won){
                        DrawText("3. Enchanted Maze: Won", 200, Yoffset, 20, GREEN);
                    } else{
                        DrawText("3. Enchanted Maze: Lost", 200, Yoffset, 20, RED);
                    }
            
                    if (game1won && game2won && game3won){
                        DrawText("You have won the Triwizard Cup!", 160, Yoffset+60, 20, GREEN);
                    } else{
                        DrawText("Better luck next time!", 180, Yoffset+60, 20, GRAY);
                    }
            
                    DrawText("Press [ENTER] to play again or [ESC] to exit.", 180, Yoffset+120, 20, RAYWHITE);
            
                    if (IsKeyPressed(KEY_ENTER)) currentscreen= MENU;
                    if (IsKeyPressed(KEY_ESCAPE)) currentscreen= EXIT;
                }
                break;            
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
