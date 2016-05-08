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
extern const int TILE_ROWS;
extern const int TILE_COLUMNS;
extern const int PLAYER_FRAMEW;
extern const int PLAYER_FRAMEH;

/**
* @brief tile structure with tile dimensions and type to be rendered
*/
typedef struct{

	SDL_Rect mBox; /**< SDL_Rect with position and dimensions.*/
	//Type of tile
	int mType; /**< int type of the tile*/
}Tile;
/**
* @brief destructable tiles to be rendered over base layer tiles
* applies collision on entites and can be destroyed with special skills for resources
*/
typedef struct{
	SDL_Rect mBox; /**< SDL_Rect with position and dimensions.*/
	int mType;/**< int type of the tile*/
	int hits;/**< int how many hits left until destroyed */
}Destructable_Tile;

/**
* @brief list of all tiles in the game instance
*/
extern Tile *tile_list;
/**
* @brief list of all destructable tiles in the game instance
*/
extern Destructable_Tile *dest_tile_list;
/**
* @brief loads tile sprites into memeory
*/
void tile_init_system(int new_game);

void tile_load_from_def(int *tile_map);
/**
* @brief loads tile sprite from filepath
* @param cstring filepath of tile
*/
Sprite * tile_load(char *filename);
/**
* @brief sets the type and mbox of all tiles
*/
void tile_set();
/**
* @brief frees tile from memory
*/
void tile_free(Tile *tile);
/**
* @brief renders tile onto screen
*/
void tile_render();
/**
* @brief frees all tiles in the tilelist
*/
void tile_close_system();

/**
* @brief Used to start procedurally generating the world.
* Chooses start location on each edge of the map and starts a manicured drunken man's walk towards the center of the map 
* man's path becomes roads
*/
Tile tile_start();
/**
* @brief returns the tile type of a tile
* @param int index for tile
* @return int type of tile
*/
int tile_get_type(int index);
/*
* @brief gets the tile index based on entity position and bouding box
* @param position of entity to match with tile position. Bounding box used for center postion of ent
* @return int tile index matching position
*/
int tile_get_tile_number(Vec2d pos, SDL_Rect bound);

/*
* @brief returns true if tile collides with certain position and its bounding box
* @param Vec2d position of collision. SDL_Rect bound to check entity collisions
* @return true if position is collides with tile
*/
int tile_collision(Vec2d pos, SDL_Rect bound);
/**
* @brief returns the distance from two tiles
* @param tile index of start tile. tile index of target tile
* @return float distance between tiles
*/
float tile_dist_to_target(int start, int target);
/*
* @brief returns the number of tiles between two tiles
* @param int tile_1 tile index of first tile. int tile_2 tile index of second tile
* @return int number of tiles between tile_1 and tile_2
*/
int tile_to_tile_dist(int tile_1, int tile_2);
/**
* @brief gets the x and y position of a tile
* @param int tile index 
* @return Vec2d x and y position of tile
*/
Vec2d tile_get_pos(int index);

/**
* @brief used for damaging destructable tiles and returns true if destroyed
* @param Vec2d position of caller, caller's bound box, and face direction
* @return 1 if position and face direction match destructable tile
*/
int tile_forage(Vec2d pos, SDL_Rect bound, int face_dir);

int *Tile_Map_Get(int &tile_count, int &row, int &col);

#define MIN(a,b) (a < b ? a : b)
#define DISTANCE_CENTER(a)(abs(a - (TOTAL_TILES/2 + TILE_ROWS/2)))

//couldn't put enum dir here, defined them manually

#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3

#define TILE_CAN_MOVE_LEFT(a)  (a % TILE_ROWS !=0 ? true: false)
#define TILE_CAN_MOVE_RIGHT(a) (( a % (TILE_ROWS-1) !=0 || a == 0) ? true: false)
#define TILE_CAN_MOVE_DOWN(a) (a < TOTAL_TILES - TILE_ROWS ? true : false )
#define TILE_CAN_MOVE_UP(a) (a >= TILE_ROWS ? true : false)
//takes vec2d
#define TILE_CENTER_X(a) (a.x + TILE_WIDTH/2)
#define TILE_CENTER_Y(a) (a.y + TILE_HEIGHT/2)

#endif

