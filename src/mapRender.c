//
// Created by fredrik on 1/26/20.
//

#include "main.h"

/* Helper function to draw the map when loaded into the buffer, should move to local map file. */
void DrawTmxLayer(tmx_map *map, tmx_layer *layer);

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

                    //TODO: Add other layer types, most important is the object layer to use for collision calculation.
                    // Issue #8
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
