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
	int **tile_path_list;
}path;

void aStar_search(Vec2d start, Vec2d goal);

#endif