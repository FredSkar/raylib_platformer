//
// Created by fredrik on 1/26/20.
//

#include "main.h"

/* Helper function to draw the map when loaded into the buffer, should move to local map file. */
void DrawTmxLayer(tmx_map *map, tmx_layer *layer);
node DrawObjects(tmx_object_group *objgr);

node createNode();
node addNode(node head, Rectangle nodeValue);

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

node MapRenderToBuf(const char *mapFile, RenderTexture2D *buf){

    tmx_layer *layer = NULL;
    node temp = NULL;

    // Setting these two function pointers allows TMX lib to load the tileset graphics and
    // set each tile's resource_image property properly.
    tmx_img_load_func = (void *(*)(const char *))LoadMapTexture;
    tmx_img_free_func = (void (*)(void *))UnloadMapTexture;

    tmx_map *mapTmx = tmx_load(mapFile);
    if (mapTmx == NULL){
        return temp;
    }

    /* Add all texture to the buffer */
    *buf = LoadRenderTexture((int)(mapTmx->width * mapTmx->tile_width), (int)(mapTmx->height * mapTmx->tile_height));

    BeginTextureMode(*buf);
    ClearBackground(SKYBLUE);

    layer = mapTmx->ly_head;

    /* Read the data from the different layers of the map */
    while(layer)
    {
        if(layer->visible){
            switch (layer->type){
                case L_LAYER:
                    TraceLog(LOG_INFO, "Render TMX layer: %s", layer->name);
                    DrawTmxLayer(mapTmx, layer);
                    break;

                case L_OBJGR:
                    temp = DrawObjects(layer->content.objgr);
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
    return(temp);
}

node DrawObjects(tmx_object_group *objgr) {
    node llhead = NULL;
    node *current = NULL;
    tmx_object *head = objgr->head;
    //Color color = int_to_color(objgr->color);
    struct Color white;
    white.a = 0xff;
    white.b = 0xff;
    white.g = 0xff;
    white.r = 0xff;

    struct Color red;
    red.a = 0xff;
    red.b = 0x00;
    red.g = 0x00;
    red.r = 0xff;

    struct Color blue;
    blue.a = 0xff;
    blue.b = 0xff;
    blue.g = 0x00;
    blue.r = 0x00;

    struct Color green;
    green.a = 0xff;
    green.b = 0x00;
    green.g = 0xff;
    green.r = 0x00;

    struct Color black;
    black.a = 0xff;
    black.b = 0x00;
    black.g = 0x00;
    black.r = 0x00;

    int index = 0;
    struct Color array[5] = {white, red, green, blue, black};

    while (head) {
        if (head->visible) {
            if (head->obj_type == OT_SQUARE) {
                DrawRectangleLinesEx((Rectangle){head->x, head->y, head->width, head->height}, 5, array[index]);
                llhead = addNode(llhead, (Rectangle) {head->x, head->y, head->width, head->height});
                index++;
            }
//            else if (head->obj_type  == OT_POLYGON) {
//                draw_polygon(head->x, head->y, head->content.shape->points, head->content.shape->points_len, color);
//            }
//            else if (head->obj_type == OT_POLYLINE) {
//                draw_polyline(head->x, head->y, head->content.shape->points, head->content.shape->points_len, color);
//            }
//            else if (head->obj_type == OT_ELLIPSE) {
//                DrawEllipseLines(head->x + head->width/2.0, head->y + head->height/2.0, head->width/2.0, head->height/2.0, color);
//            }
        }
        head = head->next;
    }
    return(llhead);
}

node addNode(node head, Rectangle nodeValue){
    node temp,p;// declare two nodes temp and p
    temp = createNode();//createNode will return a new node with data = value and next pointing to NULL.
    temp->data = nodeValue; // add element's value to data part of node
    if(head == NULL){
        head = temp;     //when linked list is empty
    }
    else{
        p  = head;//assign head to p
        while(p->next != NULL){
            p = p->next;//traverse the list until p is the last node.The last node always points to NULL.
        }
        p->next = temp;//Point the previous last node to the new node created.
    }
    return head;
}

node createNode(){
    node temp; // declare a node
    temp = (node)malloc(sizeof(struct Hit_List)); // allocate memory using malloc()
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}