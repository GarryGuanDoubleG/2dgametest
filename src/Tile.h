#ifndef _TILES_H_
#define _TILES_H_
#include <fstream>
#include <stdio.h>
#include <string>
#include "graphics.h"
#include "sprite.h"
#include "Tile_define.h"
#include "vector.h"
#include "inventory.h"

extern const int SCREEN_HEIGHT;
extern const int SCREEN_WIDTH;
extern const int TOTAL_TILES;
extern const int TILE_WIDTH;
extern const int TILE_HEIGHT; 
extern const int TOTAL_TILES_X;
extern const int TOTAL_TILES_Y;
extern const int PLAYER_FRAMEW;
extern const int PLAYER_FRAMEH;

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
	int hits;//how many hits left until collapse
}Destructable_Tile;
extern Tile *tile_list;
extern Destructable_Tile *dest_tile_list;

void tile_init_system();
Sprite2 * tile_load(char *filename);
void tile_set();
void tile_free(Tile *tile);
void tile_render ();
void tile_close_system();
Tile tile_start();
int tile_collision(Vec2d pos, SDL_Rect bound);
int tile_forage(Vec2d pos, SDL_Rect bound, int face_dir);
int tile_get_tile_number(Vec2d pos);
int ** tile_get_heuristic(int start, int target, int **curr_tile_list);
#define MIN(a,b) (a < b ? a : b)
#define DISTANCE_CENTER(a)(abs(a - (TOTAL_TILES/2 + TOTAL_TILES_X/2)))
//couldn't put enum dir here, defined them manually
#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3

#endif

