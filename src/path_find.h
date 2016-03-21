#ifndef _PATH_FIND_H_
#define _PATH_FIND_H_
#include "Tile.h"
#include "vector.h"
typedef struct Path_S
{
	Path_S *next;
	int move;
}Path;

void getPath(const int size, Vec2d *start, Vec2d *target);

#define MAX_PATH_SIZE 100
#endif
