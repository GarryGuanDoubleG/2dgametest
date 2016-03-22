#ifndef _TILES_H_
#define _TILES_H_
#include <fstream>
#include <stdio.h>
#include <string>
#include <math.h>
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

typedef struct{
	int *tile_index;
	int *distance_to_target;
	int size;
	int start;
	int target;
}tile_heuristic;

extern Tile *tile_list;
extern Destructable_Tile *dest_tile_list;

void tile_init_system();
Sprite2 * tile_load(char *filename);
void tile_set();
void tile_free(Tile *tile);
void tile_render ();
void tile_close_system();

Tile tile_start();
int tile_get_type(int index);
int tile_get_tile_number(Vec2d pos);

int tile_collision(Vec2d pos, SDL_Rect bound);
int tile_forage(Vec2d pos, SDL_Rect bound, int face_dir);

void tile_list_heuristic_free(tile_heuristic * tile_list);
float tile_dist_to_target(int start, int target);
Vec2d tile_get_pos(int index);
tile_heuristic * tile_get_heuristic(int size, int start, int target);

#define MIN(a,b) (a < b ? a : b)
#define DISTANCE_CENTER(a)(abs(a - (TOTAL_TILES/2 + TOTAL_TILES_X/2)))

//couldn't put enum dir here, defined them manually

#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3

#define TILE_CAN_MOVE_LEFT(a)  (a % TOTAL_TILES_X !=0 ? true: false)
#define TILE_CAN_MOVE_RIGHT(a) (( a % (TOTAL_TILES_X-1) !=0 || a == 0) ? true: false)
#define TILE_CAN_MOVE_DOWN(a) (a > TOTAL_TILES - TOTAL_TILES_X ? true : false )
#define TILE_CAN_MOVE_UP(a) (a >= TOTAL_TILES_X ? true : false)
//takes vec2d
#define TILE_CENTER_X(a) (a.x + TILE_WIDTH/2)
#define TILE_CENTER_Y(a) (a.y + TILE_HEIGHT/2)

#endif

