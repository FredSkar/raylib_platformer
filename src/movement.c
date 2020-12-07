#include "main.h"
#include <math.h>

int frame_counter = 0;
int current_frame = 0;

/* The frame speed of how often to update the animations. */
int frame_speed = 10;

Vector2 playerAcc = {0, 0};

void UpdateMovement(struct Player_Attrib *player, Camera2D *cam, struct Rectangle mapLim, node head){

    //TODO: Add check so the player can't move outside the map. issue #5
    //TODO: Add check so the camera can't move outside the map. issue #6
    //TODO: Add global variable as the player speed. issue #7
    float time;
    float plyPosMovX = 0;
    float plyPosMovY = 0;
    bool collision = false;
    bool collisionSide = false;
    bool collisionUpDown = false;

    /************************************************************/
    /* Calculate the new position of the player and what direction the player moves. */

    time = GetFrameTime();
    player->move = false;
    player->dir = 'n';
    if (IsKeyDown(KEY_A)){
        playerAcc.x -= (float)(time * 100.0);
        player->move = true;
        player->dir = 'L';
        if (player->flipped == false){
            player->frame.width *= -1;
            player->flipped = true;
        }
    }
    else if (IsKeyDown(KEY_D)){
        playerAcc.x += (float)(time * 100.0);
        player->move = true;
        player->dir = 'R';
        if (player->flipped == true){
            player->frame.width *= -1;
            player->flipped = false;
        }
    }
    else if (IsKeyDown(KEY_W) && (player->falling == false) && (player->jumping == false)){
        playerAcc.y -= (float)(time * 100.0);
        player->dir = 'U';
    }
    else if (IsKeyDown(KEY_S) && (player->falling == false) && (player->jumping == false)){
        playerAcc.y += (float)(time * 100.0);
        player->dir = 'D';
    }

    if (IsKeyPressed(KEY_SPACE)){
        //player->jumping = true;
        playerAcc.y = -PLAYERJUMPSPEED;
        player->dir = 'U';
        //player->falling = false;
    }

    /**************************************************************************/
    /* Calculate the animation speed of the player. */

    if (player->move && (player->jumping == false )){//&& player->falling == false)){
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

    /****************************************************************/
    /* Update the position if the player is jumping or falling */
    if(player->dir == 'n' && playerAcc.x > 0){
        if(playerAcc.x < 0.5){
            playerAcc.x = 0;
        }
        else {
            playerAcc.x -= (float) (time * 100.0);
        }
    }
    else if(player->dir == 'n' && playerAcc.x < 0){
        if(playerAcc.x > -0.5){
            playerAcc.x = 0;
        }
        else {
            playerAcc.x += (float) (time * 100.0);
        }
    }

    if(player->falling){
        playerAcc.y += (float)(time * 200.0);
        //player->dir = 'D';
    }
    else{
        playerAcc.y = 0;
        //player->dir = 'n';
    }

    if(playerAcc.x > PLAYERMOVESPEED){
        playerAcc.x = PLAYERMOVESPEED;
    }
    else if(playerAcc.x < -PLAYERMOVESPEED){
        playerAcc.x = -PLAYERMOVESPEED;
    }

    if(playerAcc.y > PLAYERJUMPSPEED){
        playerAcc.y = PLAYERJUMPSPEED;
    }
    else if(playerAcc.y < -PLAYERFALLSPEED){
        playerAcc.y = -PLAYERFALLSPEED;
    }

    plyPosMovX = playerAcc.x;
    plyPosMovY = playerAcc.y;

    /**********************************************************/
    /* Check if the player about to collide with an object. In that case don't update the position. */

    int collisionCount = 0;
    while(head->next != NULL) {
        collision = CheckCollisionPointRec((Vector2) {(player->pos.x + (player->width/2)),
                                                      (player->pos.y + player->height + plyPosMovY + (float)(time * 200.0))}, head->data);
        if(collision && player->jumping == false){
            collisionCount++;
        }
        else{
            player->falling = false;
        }
        if(player->dir == 'L' || player->dir == 'R'){
            collisionSide = CheckCollisionRecs((Rectangle) {player->pos.x + plyPosMovX, player->pos.y, player->width,
                                                        player->height}, head->data);
        }
        if(player->dir == 'U' || player->dir == 'D' || player->falling == true) {
            collisionUpDown = CheckCollisionRecs((Rectangle) {player->pos.x, player->pos.y + plyPosMovY, player->width,
                                                        player->height}, head->data);
        }
        if(collisionUpDown){
            player->falling = false;
            player->jumping = false;
            break;
        }
        if(collisionSide){
            player->move = false;
            break;
        }
        else{
            head = head->next;
        }
    }

    if (collisionCount == 0 ){//&& player->jumping == false){
        //printf("falling\n");
        //player->falling = true;
        //player->dir = 'D';
        //plyPosMovY += (float)(time * 200.0);
    }

    /**********************************************************************/
    /* Check that the player is moving within the boundary of the map. */

    if(!collisionSide) {
        player->pos.x += plyPosMovX;
    }
    if(!collisionUpDown) {
        player->pos.y += plyPosMovY;
        player->falling = true;
    }

    if (player->pos.x <= 0) {
        player->pos.x = 0;
    } else if (player->pos.x >= (mapLim.width - (float) player->width)) {
        player->pos.x = mapLim.width - (float) player->width;
    }
    if (player->pos.y <= 0) {
        player->pos.y = 0;
    } else if (player->pos.y >= (mapLim.height - (float) player->height)) {
        player->pos.y = mapLim.height - (float) player->height;
    }

}
