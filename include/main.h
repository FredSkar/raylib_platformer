//
// Created by fredrik on 1/26/20.
//

#ifndef RAYLIB_PLATFORMER_MAIN_H
#define RAYLIB_PLATFORMER_MAIN_H

/*--------- includes ------------
 *
 * All includes required for the project should be specified below.
 *
*/
#include <unistd.h>
#include <stdlib.h>
/* Library for drawing graphics. */
#include "raylib.h"
/* Library for loading and working with tmx maps. */
#include "tmx.h"

/*---------- definitions ---------
 *
 *  All definitions used for the project should be defined below.
 *
*/
/* Defines the size of the screen. */
#define SCREENWIDTH 1300
#define SCREENHEIGHT 1000
/* Set the target FPS to run the game at. */
#define FPS 60

/* The attributes for the player character. */
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

/* Initiates the starting values for the player. */
void InitPlayer(struct Player_Attrib *player);
/* Updates the movement for the player, should be called once every cycle for the game engine. */
void UpdateMovement(struct Player_Attrib *player, Camera2D *cam);
/* Loads a new tmx map into the buffer that is drawn. */
void MapRenderToBuf(const char *mapFile, RenderTexture2D *buf);

#endif //RAYLIB_PLATFORMER_MAIN_H
