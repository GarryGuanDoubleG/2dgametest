#ifndef _PATHFINDING_H_
#define _PATHFINDING_H_
#include "Tile.h"
#include "vector.h"


typedef struct path_node_S
{
	int tile_number;
	path_node_S* parent;
	int distance;	
	int distance_to_node;
}path_node;

typedef struct PATH_S
{
	void (*push)( path_node node);
	void (*pop)();
	tile_heuristic *tile_path_list;
}Path;

void aStar_search(int aggro_range, Vec2d start, Vec2d goal, Path &path_self);

#endif