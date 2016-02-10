#ifndef _TILES_H_
#define _TILES_H_
#include <fstream>
#include <stdio.h>
#include <string>
#include "graphics.h"
#include "sprite.h"

extern const int TOTAL_TILES;
extern const int TILE_WIDTH;
extern const int TILE_HEIGHT; 
extern const int TILE_GRASS1;

extern SDL_Texture * tile_sprite_grass;
typedef struct{
public:

	SDL_Rect mBox;
	//Type of tile
	int mType;

}Tile;

extern Tile *tile_list;



void tile_init_system();
void tile_load(char *filename);
void tile_set();
void tile_free(Tile *tile);
void tile_render (SDL_Rect *camera);
void tile_close_system();


#endif

