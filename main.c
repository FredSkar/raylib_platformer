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
#include <stdlib.h>
#include "raylib.h"
#include "tmx.h"

#define SCREENWIDTH 1300
#define SCREENHEIGHT 1000
#define FPS 60

struct Player_Attrib {
    Vector2 speed;
    Vector2 acc;
    Vector2 pos;
    Texture2D texture;
    int width;
    int height;
    struct Rectangle frame;
    int move;
    int falling;
    int jumping;
    int flipped;
    char dir;

};

int frame_counter = 0;
int frame_speed = 10;
int current_frame = 0;

void InitPlayer(struct Player_Attrib *player);
void UpdateMovement(struct Player_Attrib *player);
void MapRenderToBuf(const char *mapFile, RenderTexture2D *buf);
void DrawTmxLayer(tmx_map *map, tmx_layer *layer);

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

    // Load terain
    Texture2D terain = LoadTexture("asset/Spritesheets/spritesheet_ground.png");
    int t_width = terain.width / 8;
    int t_height = terain.height / 16;
    Rectangle t_frame = {0, (float )t_height, (float)t_width, (float)t_height};
    Vector2 t_pos = {0 , 0};

    MapRenderToBuf("asset/test_map.tmx", &mapBuffer);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
//        UpdateMovement(&Player);
//
//        t_pos.x = 0;
//        t_pos.y = (float)(SCREENHEIGHT - t_height);
//
//        // Draw
//        //----------------------------------------------------------------------------------
//        BeginDrawing();
//
//        //ClearBackground(RED);
//
//        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
//
//        /*while (t_pos.x < (float)SCREENWIDTH){
//            if (CheckCollisionRecs((Rectangle){p_pos.x, p_pos.y, p_frame.width, p_frame.height}, (Rectangle){t_pos.x, t_pos.y, t_frame.width, t_frame.height})){
//                Rectangle rect = GetCollisionRec((Rectangle){p_pos.x, p_pos.y, p_frame.width, p_frame.height}, (Rectangle){t_pos.x, t_pos.y, t_frame.width, t_frame.height});
//                p_pos.y = t_pos.y - p_frame.height + 1;
//                falling = false;
//            }
//            DrawTextureRec(terain, t_frame, t_pos, RAYWHITE);
//            t_pos.x += t_frame.width;
//        }*/
//
//        DrawTextureRec(Player.texture, Player.frame, Player.pos, RAYWHITE);
//
//        //DrawRectangleLines(p_frame.x, p_frame.y, p_frame.width, p_frame.height, BLUE);
//        //DrawRectangle(x, y, 100, 100, BLUE);
//
//        EndDrawing();
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //BeginMode2D(camera);
        // Flip along the y axis because OpenGL origin is at bottom left corner while Raylib is top left
        DrawTextureRec(
                mapBuffer.texture,
                (Rectangle){0, 0, mapBuffer.texture.width, -mapBuffer.texture.height},
                (Vector2){0.0, 0.0},
                WHITE);
        EndMode2D();
        EndDrawing();

        //---------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(Player.texture);
    UnloadTexture(terain);
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
    player->pos = (Vector2){0, 0};
    player->move = false;
    player->dir = 0;
    player->flipped = false;
    player->falling = true;
    player->jumping = false;

}

void UpdateMovement(struct Player_Attrib *player){

    float time;

    time = GetFrameTime();
    player->move = false;
    if (IsKeyDown(KEY_A)){
        player->pos.x -= (float)(time * 100.0);
        player->move = true;
        player->dir = 'L';
        if (player->flipped == false){
            player->frame.width *= -1;
            player->flipped = true;
        }
    }
    else if (IsKeyDown(KEY_D)){
        player->pos.x += (float)(time * 100.0);
        player->move = false;
        if (IsKeyDown(KEY_A)){
            player->pos.x -= (float)(time * 100.0);
            player->move = true;
            player->dir = 'L';
            if (player->flipped == false){
                player->frame.width *= -1;
                player->flipped = true;
            }
        }
        else if (IsKeyDown(KEY_D)){
            player->pos.x += (float)(time * 100.0);
            player->move = true;
            player->dir = 'R';
            if (player->flipped == true){
                player->frame.width *= -1;
                player->flipped = false;
            }
        }
        else if (IsKeyDown(KEY_W)){
            player->pos.y -= (float)(time * 100.0);
            player->dir = 'U';
        }
        else if (IsKeyDown(KEY_S)){
            player->pos.y += (float)(time * 100.0);
            player->dir = 'D';
        }

        if (IsKeyPressed(KEY_SPACE)){
            player->jumping = true;
        }

        if (player->move && (player->jumping == false && player->falling == false)){
            frame_counter++;
            if (frame_counter >= FPS/frame_speed){
                frame_counter = 0;
                current_frame++;

                if (current_frame > 1){
                    current_frame = 0;
                }
                if (player->dir == 'R' || player->dir == 'L'){
                    player->frame.y = (float)(current_frame * player->height);
                }

            }
        }else{
            player->frame.y = (float)(7 * player->height);
            if (player->flipped == true){
                player->frame.width *= -1;
                player->flipped = false;
            }
        }

        if (player->jumping == true){
            player->pos.y -= (float)(time * 300.0);

            frame_counter++;
            if (frame_counter >= (FPS)){
                frame_counter = 0;
                player->jumping = false;
                player->falling = true;
            }

        }
        player->pos.x = 0;
        //player->pos.y = (float)(SCREENHEIGHT - t_height);
        player->move = true;
        player->dir = 'R';
        if (player->flipped == true){
            player->frame.width *= -1;
            player->flipped = false;
        }
    }
    else if (IsKeyDown(KEY_W)){
        player->pos.y -= (float)(time * 100.0);
        player->dir = 'U';
    }
    else if (IsKeyDown(KEY_S)){
        player->pos.y += (float)(time * 100.0);
        player->dir = 'D';
    }

    if (IsKeyPressed(KEY_SPACE)){
        player->jumping = true;
    }

    if (player->move && (player->jumping == false && player->falling == false)){
        frame_counter++;
        if (frame_counter >= FPS/frame_speed){
            frame_counter = 0;
            current_frame++;

            if (current_frame > 1){
                current_frame = 0;
            }
            if (player->dir == 'R' || player->dir == 'L'){
                player->frame.y = (float)(current_frame * player->height);
            }

        }
    }else{
        player->frame.y = (float)(7 * player->height);
        if (player->flipped == true){
            player->frame.width *= -1;
            player->flipped = false;
        }
    }

    if (player->jumping == true){
        player->pos.y -= (float)(time * 300.0);

        frame_counter++;
        if (frame_counter >= (FPS)){
            frame_counter = 0;
            player->jumping = false;
            player->falling = true;
        }

    }
    if (player->falling){
        player->pos.y += (float)(time * 200.0);
    }
}

Texture2D *LoadMapTexture(const char *fileName)
{
    Texture2D *tex = (Texture2D *)malloc(sizeof(Texture2D));
    if (tex != NULL)
    {
        *tex = LoadTexture(fileName);
        TraceLog(LOG_INFO, "TMX texture loaded from %s", fileName);
        return tex;
    }
    return NULL;
}

void UnloadMapTexture(Texture2D *tex)
{
    if (tex != NULL)
    {
        UnloadTexture(*tex);
        free(tex);
    }
}

/* Loading and drawing map example from: https://github.com/OnACoffeeBreak/raylib_tiled_import_with_tmx/blob/master/src/raylib_tiled_import.c */
void DrawTmxLayer(tmx_map *map, tmx_layer *layer)
{
    unsigned long row, col;
    unsigned int gid;
    unsigned int flip;
    tmx_tile *tile;
    unsigned int tile_width;
    unsigned int tile_height;
    Rectangle sourceRect;
    Rectangle destRect;
    Texture2D *tsTexture; // tileset texture
    float rotation = 0;
    Vector2 origin = {0, 0};

    for (row = 0; row < map->height; row++)
    {
        for (col = 0; col < map->width; col++)
        {
            gid = layer->content.gids[(row * map->width) + col];
            flip = gid & ~TMX_FLIP_BITS_REMOVAL;    // get flip operations from GID
            gid = gid & TMX_FLIP_BITS_REMOVAL;      // remove flip operations from GID to get tile ID
            tile = map->tiles[gid];
            if (tile != NULL)
            {
                // Get tile's texture out of the tileset texture
                if (tile->image != NULL)
                {
                    tsTexture = (Texture2D *)tile->image->resource_image;
                    tile_width = tile->image->width;
                    tile_height = tile->image->height;
                }
                else
                {
                    tsTexture = (Texture2D *)tile->tileset->image->resource_image;
                    tile_width = tile->tileset->tile_width;
                    tile_height = tile->tileset->tile_height;
                }

                sourceRect.x = (float)tile->ul_x;
                sourceRect.y = (float)tile->ul_y;
                sourceRect.width = destRect.width = (float)tile_width;
                sourceRect.height = destRect.height = (float)tile_height;
                destRect.x = (float)(col * tile_width);
                destRect.y = (float)(row * tile_height);

                // Deal with flips
                origin.x = 0;
                origin.y = 0;
                rotation = 0;
                switch(flip)
                {
                    case TMX_FLIPPED_DIAGONALLY:
                    {
                        sourceRect.height = -1 * sourceRect.height;
                        rotation = 90;
                    } break;
                    case TMX_FLIPPED_VERTICALLY:
                    {
                        sourceRect.height = -1 * sourceRect.height;
                    } break;
                    case TMX_FLIPPED_DIAGONALLY + TMX_FLIPPED_VERTICALLY:
                    {
                        rotation = -90;
                    } break;
                    case TMX_FLIPPED_HORIZONTALLY:
                    {
                        sourceRect.width = -1 * sourceRect.width;
                    } break;
                    case  TMX_FLIPPED_DIAGONALLY + TMX_FLIPPED_HORIZONTALLY:
                    {
                        rotation = 90;
                    } break;
                    case TMX_FLIPPED_HORIZONTALLY + TMX_FLIPPED_VERTICALLY:
                    {
                        rotation = 180;
                    } break;
                    case TMX_FLIPPED_DIAGONALLY + TMX_FLIPPED_HORIZONTALLY + TMX_FLIPPED_VERTICALLY:
                    {
                        sourceRect.width = -1 * sourceRect.width;
                        rotation = 90;
                    } break;
                    default:
                    {
                        origin.x = 0;
                        origin.y = 0;
                        rotation = 0;
                    } break;
                }

                // Adjust origin to rotate around the center of the tile,
                // which means destination recangle's origin must be adjusted.
                if (rotation != 0.0)
                {
                    origin.x = (float)tile_width / 2;
                    origin.y = (float)tile_height / 2;
                    destRect.x += (float)tile_width / 2;
                    destRect.y += (float)tile_height / 2;
                }

                // TODO: Take layer opacity into account
                DrawTexturePro(*tsTexture, sourceRect, destRect, origin, rotation, WHITE);
            }
        }
    }
}

void MapRenderToBuf(const char *mapFile, RenderTexture2D *buf){

    tmx_layer *layer = NULL;

    // Setting these two function pointers allows TMX lib to load the tileset graphics and
    // set each tile's resource_image property properly.
    tmx_img_load_func = (void *(*)(const char *))LoadMapTexture;
    tmx_img_free_func = (void (*)(void *))UnloadMapTexture;

    tmx_map *mapTmx = tmx_load(mapFile);
    if (mapTmx == NULL){
        return;
    }

    *buf = LoadRenderTexture((int)(mapTmx->width * mapTmx->tile_width), (int)(mapTmx->height * mapTmx->tile_height));

    BeginTextureMode(*buf);
    ClearBackground(SKYBLUE);

    layer = mapTmx->ly_head;

    while(layer)
    {
        if(layer->visible){
            switch (layer->type){
                case L_LAYER:
                    TraceLog(LOG_INFO, "Render TMX layer: %s", layer->name);
                    DrawTmxLayer(mapTmx, layer);
                    break;
                default:
                    TraceLog(LOG_INFO, "Nothing to load.");
                    break;

            }
        }
        layer = layer->next;
    }
    EndTextureMode();
    tmx_map_free(mapTmx);
}
