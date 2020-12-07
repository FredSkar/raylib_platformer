/*******************************************************************************************
*
*
********************************************************************************************/
#include "main.h"
#include <stdio.h>

int main(void) {
    struct RenderTexture2D mapBuffer;
    struct Rectangle HitList[2];

	bool hit = false;

    node head = NULL;
    node current = NULL;
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(FPS);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    struct Player_Attrib Player;
    InitPlayer(&Player);

    Camera2D camera;
    camera.target = (Vector2) {Player.pos.x, Player.pos.y};
    camera.offset = (Vector2) {((float) SCREENWIDTH / 2) - (float) Player.width,
                               ((float) SCREENHEIGHT / 2) - (float) Player.height};
    //camera.offset = (Vector2){0,0};
    camera.rotation = 0.0;
    camera.zoom = 1.0;

    /* Loads all map data and loads all object data into a linked list to use for collision test. */
    head = MapRenderToBuf("asset/test_map.tmx", &mapBuffer);

    float upperLim = ((float) SCREENHEIGHT / 2) - (float) Player.height;
    float lowerLim = (float) mapBuffer.texture.height - ((float) SCREENHEIGHT / 2) - (float) Player.height;
    float leftLim = ((float) SCREENWIDTH / 2) - (float) Player.width;
    float rightLim = (float) mapBuffer.texture.width - ((float) SCREENHEIGHT / 2) - ((float) Player.width * 2);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        UpdateMovement(&Player, &camera,
                       (Rectangle) {0, 0, (float) mapBuffer.texture.width,
                                    (float) mapBuffer.texture.height}, head);

        if( Player.pos.y < upperLim && Player.pos.x <leftLim)
        {
            camera.target = (Vector2) {leftLim, upperLim};
        }
        else if( Player.pos.y < upperLim && Player.pos.x > rightLim)
        {
            camera.target = (Vector2) {rightLim, upperLim};
        }
        else if( Player.pos.y > lowerLim && Player.pos.x > rightLim)
        {
            camera.target = (Vector2) {rightLim, lowerLim};
        }
        else if( Player.pos.y > lowerLim && Player.pos.x < leftLim)
        {
            camera.target = (Vector2) {leftLim, lowerLim};
        }
        else if( Player.pos.x > rightLim)
        {
            camera.target = (Vector2) {rightLim, Player.pos.y};
        }
        else if( Player.pos.y < upperLim)
        {
            camera.target = (Vector2) {Player.pos.x, upperLim};
        }
        else if( Player.pos.x < leftLim)
        {
            camera.target = (Vector2) {leftLim, Player.pos.y};
        }
        else if( Player.pos.y > lowerLim)
        {
            camera.target = (Vector2) {Player.pos.x, lowerLim};
        }
        else
        {
            camera.target = (Vector2) {Player.pos.x, Player.pos.y};
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode2D(camera);
        // Flip along the y axis because OpenGL origin is at bottom left corner while Raylib is top left
        DrawTextureRec(
                mapBuffer.texture,
                (Rectangle) {0, 0, mapBuffer.texture.width, -mapBuffer.texture.height},
                (Vector2) {0.0, 0.0},
                WHITE);
        DrawTextureRec(Player.texture, Player.frame, Player.pos, RAYWHITE);
        //DrawRectangle(Player.pos.x, Player.pos.y, Player.frame.width, Player.frame.height, (Color)  {0xff, 0xff, 0xff, 0xff});
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

void InitPlayer(struct Player_Attrib *player) {
    // Init Player
    player->texture = LoadTexture("asset/Spritesheets/players.png");
    player->width = player->texture.width / 8;
    player->height = player->texture.height / 8;
    player->frame = (Rectangle) {0, 0, (float) player->width, (float) player->height};
    player->pos = (Vector2) {600, 600};
    player->move = false;
    player->dir = 0;
    player->flipped = false;
    player->falling = true;
    player->jumping = false;

}
