#include "raylib.h";
#include "../main.c";

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
