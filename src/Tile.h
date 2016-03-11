#ifndef _TILES_H_
#define _TILES_H_
#include <fstream>
#include <stdio.h>
#include <string>
#include "graphics.h"
#include "sprite.h"
#include "Tile_define.h"

extern const int SCREEN_HEIGHT;
extern const int SCREEN_WIDTH;
extern const int TOTAL_TILES;
extern const int TILE_WIDTH;
extern const int TILE_HEIGHT; 

//can probably take out mBox and use array index for location
//faster and less memory
typedef struct{

	SDL_Rect mBox;
	//Type of tile
	int mType;
}Tile;

typedef struct{
	SDL_Rect mBox;
	int mType;
}Destructable_Tile;
extern Tile *tile_list;

void tile_init_system();
Sprite2 * tile_load(char *filename);
void tile_set();
void tile_free(Tile *tile);
void tile_render (SDL_Rect *camera);
void tile_close_system();


#endif

