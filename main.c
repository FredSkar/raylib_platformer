/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include "raylib.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 900;
    int fps = 60;
    int frame_counter = 0;
    int frame_speed = 10;
    int current_frame = 0;
    float time;

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(fps);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // Init Player
    Texture2D player = LoadTexture("../asset/Spritesheets/players.png");
    int p_width = player.width / 8;
    int p_height = player.height / 8;
    Rectangle p_frame = {0, 0, (float )p_width, (float)p_height};
    Vector2 p_pos = {0, 0};
    int p_move;
    char p_dir = 0;
    int p_fliped = false;
    int falling = true;
    int jump = false;

    // Load terain
    Texture2D terain = LoadTexture("../asset/Spritesheets/spritesheet_ground.png");
    int t_width = terain.width / 8;
    int t_height = terain.height / 16;
    Rectangle t_frame = {0, (float )t_height, (float)t_width, (float)t_height};
    Vector2 t_pos = {0 , 0};

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        time = GetFrameTime();
        p_move = false;
        if (IsKeyDown(KEY_A)){
            p_pos.x -= (float)(time * 100.0);
            p_move = true;
            p_dir = 'L';
            if (p_fliped == false){
                p_frame.width *= -1;
                p_fliped = true;

            }
        }
        else if (IsKeyDown(KEY_D)){
            p_pos.x += (float)(time * 100.0);
            p_move = true;
            p_dir = 'R';
            if (p_fliped == true){
                p_frame.width *= -1;
                p_fliped = false;
            }
        }
        else if (IsKeyDown(KEY_W)){
            p_pos.y -= (float)(time * 100.0);
            p_dir = 'U';
        }
        else if (IsKeyDown(KEY_S)){
            p_pos.y += (float)(time * 100.0);
            p_dir = 'D';
        }

        if (IsKeyPressed(KEY_SPACE)){
            jump = true;
        }

        if (p_move && (jump == false && falling == false)){
            frame_counter++;
            if (frame_counter >= fps/frame_speed){
                frame_counter = 0;
                current_frame++;

                if (current_frame > 1){
                    current_frame = 0;
                }
                if (p_dir == 'R' || p_dir == 'L'){
                    p_frame.y = (float)(current_frame * p_height);
                }

            }
        }else{
            p_frame.y = (float)(7 * p_height);
            if (p_fliped == true){
                p_frame.width *= -1;
                p_fliped = false;
            }
        }

        if (jump == true){
            p_pos.y -= (float)(time * 300.0);

            frame_counter++;
            if (frame_counter >= (fps)){
                frame_counter = 0;
                jump = false;
                falling = true;
            }

        }
        t_pos.x = 0;
        t_pos.y = (float)(screenHeight - t_height);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RED);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        while (t_pos.x < (float)screenWidth){
            if (CheckCollisionRecs((Rectangle){p_pos.x, p_pos.y, p_frame.width, p_frame.height}, (Rectangle){t_pos.x, t_pos.y, t_frame.width, t_frame.height})){
                Rectangle rect = GetCollisionRec((Rectangle){p_pos.x, p_pos.y, p_frame.width, p_frame.height}, (Rectangle){t_pos.x, t_pos.y, t_frame.width, t_frame.height});
                p_pos.y = t_pos.y - p_frame.height + 1;
                falling = false;
            }
            DrawTextureRec(terain, t_frame, t_pos, RAYWHITE);
            t_pos.x += t_frame.width;
        }
        if (falling){
            p_pos.y += (float)(time * 200.0);
        }
        DrawTextureRec(player, p_frame, p_pos, RAYWHITE);

        //DrawRectangleLines(p_frame.x, p_frame.y, p_frame.width, p_frame.height, BLUE);
        //DrawRectangle(x, y, 100, 100, BLUE);

        EndDrawing();
        //---------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(player);
    UnloadTexture(terain);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
