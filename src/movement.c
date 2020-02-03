#include "main.h"

int frame_counter = 0;
int current_frame = 0;

/* The frame speed of how often to update the animations. */
int frame_speed = 10;

int mapLimY = false;
int mapLimX = false;

void UpdateMovement(struct Player_Attrib *player, Camera2D *cam, struct Rectangle mapLim){

    //TODO: Add check so the player can't move outside the map. issue #5
    //TODO: Add check so the camera can't move outside the map. issue #6
    //TODO: Add global variable as the player speed. issue #7
    float time;

    time = GetFrameTime();
    player->move = false;
    if (IsKeyDown(KEY_A)){
        player->pos.x -= (float)(time * 200.0);
        if(!mapLimX){
            cam->offset.x += (float)(time * 200.0);
        }
        player->move = true;
        player->dir = 'L';
        if (player->flipped == false){
            player->frame.width *= -1;
            player->flipped = true;
        }
    }
    else if (IsKeyDown(KEY_D)){
        player->pos.x += (float)(time * 200.0);
        if(!mapLimX){
            cam->offset.x -= (float)(time * 200.0);
        }
        player->move = false;
        //player->pos.x = 0;
        //player->pos.y = (float)(SCREENHEIGHT - t_height);
        player->move = true;
        player->dir = 'R';
        if (player->flipped == true){
            player->frame.width *= -1;
            player->flipped = false;
        }
    }
    else if (IsKeyDown(KEY_W)){
        player->pos.y -= (float)(time * 200.0);
        if(!mapLimY){
            cam->offset.y += (float)(time * 200.0);
        }
        player->dir = 'U';
    }
    else if (IsKeyDown(KEY_S)){
        player->pos.y += (float)(time * 200.0);
        if(!mapLimY){
            cam->offset.y -= (float)(time * 200.0);
        }
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
        //player->pos.y += (float)(time * 200.0);
    }

    if (player->pos.x <= 0){
        player->pos.x = 0;
    }
    else if (player->pos.x >= (mapLim.width - (float)player->width)){
        player->pos.x = mapLim.width - (float)player->width;
    }
    if (player->pos.y <= 0){
        player->pos.y = 0;
    }
    else if (player->pos.y >= (mapLim.height - (float)player->height)){
        player->pos.y = mapLim.height - (float)player->height;
    }

    float upperLim = ((float)SCREENHEIGHT / 2) - (float)player->height;
    float lowerLim = mapLim.height - ((float)SCREENHEIGHT / 2) - (float)player->height;
    float leftLim = ((float)SCREENWIDTH / 2) - (float)player->width;
    float rightLim = mapLim.width - ((float)SCREENHEIGHT / 2) - ((float)player->width * 2);

    if (player->pos.y <= upperLim || player->pos.y >= lowerLim){
        mapLimY = true;
    } else{
        mapLimY = false;
    }

    if (player->pos.x <= leftLim || player->pos.x >= rightLim){
        mapLimX = true;
    }else{
        mapLimX = false;
    }

}
