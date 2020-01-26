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
#include "main.h"

int main(void)
{
    struct RenderTexture2D mapBuffer;
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(FPS);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    struct Player_Attrib Player;
    InitPlayer(&Player);

    Camera2D camera;
    camera.target = (Vector2){ Player.pos.x, Player.pos.y };
    camera.offset = (Vector2){ ((float)SCREENWIDTH/2) - (float )Player.width, ((float)SCREENHEIGHT/2) - (float)Player.height };
    camera.rotation = 0.0;
    camera.zoom = 1.0;

    MapRenderToBuf("asset/test_map.tmx", &mapBuffer);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        UpdateMovement(&Player, &camera);
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);
        // Flip along the y axis because OpenGL origin is at bottom left corner while Raylib is top left
        DrawTextureRec(
                mapBuffer.texture,
                (Rectangle){0, 0, mapBuffer.texture.width, -mapBuffer.texture.height},
                (Vector2){0.0, 0.0},
                WHITE);
        DrawTextureRec(Player.texture, Player.frame, Player.pos, RAYWHITE);
        EndMode2D();
        EndDrawing();

        //---------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(Player.texture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void InitPlayer(struct Player_Attrib *player){
    // Init Player
    player->texture = LoadTexture("asset/Spritesheets/players.png");
    player->width = player->texture.width / 8;
    player->height = player->texture.height / 8;
    player->frame = (Rectangle){0, 0, (float )player->width, (float)player->height};
    player->pos = (Vector2){300, 300};
    player->move = false;
    player->dir = 0;
    player->flipped = false;
    player->falling = true;
    player->jumping = false;

}
