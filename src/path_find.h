#ifndef _PATH_FIND_H_
#define _PATH_FIND_H_
#include "Tile.h"
#include "vector.h"
typedef struct Path_S
{
	Path_S *parent;
	Path_S *next;
	int tile_index;
	float h_val;//distance to target tile
	float g_val;//all movement costs are the same because you can't move diagonally
	float f_val;
}Path;
void path_free_node(Path *path);
void path_free(Path *path);
Path* getPath(int size, Vec2d *start, Vec2d *target, Path *curr_path);
#define MAX_PATH_SIZE 100
#endif
