#include "pathfinding.h"

static int g_closed_list[90];
static int g_open_list[90];
static int g_moves[10];

void aStar_find_path()
{

}

void aStar_search(int aggro_range, Vec2d start, Vec2d goal, Path &path_self)
{
	slog("start pos x:%f y:%f goal pos: x%f y%f", start.x, start.y, goal.x, goal.y);
	path_self.tile_path_list = tile_get_heuristic(aggro_range *2, tile_get_tile_number(start), tile_get_tile_number(goal), path_self.tile_path_list );
	path_node node_start = {tile_get_tile_number(start), NULL, 0, Vec2dDistance(start, goal)};

}

