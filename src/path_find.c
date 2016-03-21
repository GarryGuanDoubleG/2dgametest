#include "path_find.h"

void getOptions(int curr, int size, int closed_list[], int open_list[])
{
	int i;
	int option = curr;
	for(i = 0; i < size; i++)
	{
		if(option % TOTAL_TILES_X != 0)
		{
			option = curr -1;
		}
	}
}

void getPath(int size, Vec2d *start, Vec2d *target)
{
	const int list_size = size * size;
	int start_index  = tile_get_tile_number(*start);
	int target_index = tile_get_tile_number(*target);
	int closed_list[MAX_PATH_SIZE];
	int open_list[MAX_PATH_SIZE];
	int i;

	for(i =0; i < MAX_PATH_SIZE; i++)
	{
		closed_list[i] = -1;
		open_list[i] = -1;
	}
	closed_list[0] = start_index;
	tile_heuristic *tile_heur = tile_get_heuristic( size, start_index, target_index);
	//get move set()
	if(tile_heur)
	{
		tile_list_heuristic_free(tile_heur);
	}
}