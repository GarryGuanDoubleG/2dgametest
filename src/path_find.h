#ifndef _PATH_FIND_H_
#define _PATH_FIND_H_
#include "Tile.h"
#include "vector.h"
/*
* @brief Path structure that uses an A* search algorithm to find the shorest path
* contains nodes that point to parent and next nodes
*/
typedef struct Path_S
{
	Path_S *parent; /**< pointer to parent node that moved to this current tile*/
	Path_S *next; /**< pointer to parent node that moved to this current tile*/
	int tile_index; /**< the tile index that current node is on*/
	float h_val;//distance to target tile
	float g_val;//all movement costs are the same because you can't move diagonally
	float f_val;/**<int stores the sum of h and g val*/
}Path;

/**
*@brief deallocates path node from memory
*@param double pointer to check if path node was properly freed
*/
void path_free_node(Path **path);
/*
* @brief frees all nodes of a path;
* @param takes reference to the path
*/
void path_free(Path *&path);
/**
* @brief getPath uses an A* search algorithm to find shorest path between two entities
* @param size: maximum size of open & closed list
		 start: Pointer to Vec2d position of entity finding path
		 start_bound: SDL_Rect bounding box to determine which tile entity is currently on
		 target_bound: bounding box to find which tile target entity is on
		 target: pointer to Vec2d of entity to travel to
		 curr_path: NULL if no current path, pointer to current path which is to be freed from memory
* @return pointer to first tile to move to with pointers to next nodes to move to
*/
Path* getPath(int size, Vec2d *start, SDL_Rect start_bound, SDL_Rect target_bound, Vec2d *target, Path *curr_path);
//100 tiles * value of each tile(10)
#define MAX_PATH_SIZE 200
#endif

