#ifndef _PATHFINDING_H_
#define _PATHFINDING_H_

#include "Tile.h"
#include "vector.h"

typedef struct path_node_S
{
	struct path_node_s *next;
	int tile_to_move;
	int direction;	
}path_node;

typedef struct PATH_S
{
	void (*push)( path_node node);
	void (*pop)();
	int head;
	int tail;
}path;

void DFS_find_path();

#endif