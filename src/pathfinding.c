#include "pathfinding.h"

static int g_closed_list[90];
static int g_open_list[90];
static int g_moves[10];

void aStar_find_path()
{

}

void aStar_search(Vec2d start, Vec2d goal)
{
	path_node start = {tile_get_tile_number(start), NULL, 0, Vec2dDistance(start, goal)};

}

