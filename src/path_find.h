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
void path_free_node(Path **path);
void path_free(Path *&path);
Path* getPath(int size, Vec2d *start, SDL_Rect start_bound, SDL_Rect target_bound, Vec2d *target, Path *curr_path);
//100 tiles * value of each tile(10)
#define MAX_PATH_SIZE 200
#endif

/*
int i;
	int option = curr;
	int move = -1;
	float min = 999;
	int heuristic; // distance from tile to target tile
	int count;
	int type;
	Path left;
	Path up;
	Path down;
	Path right;
	Path *temp_node1; // comparisons
	Path *temp_node2;
	Path *new_node = (Path*)malloc(sizeof(Path));

	if(new_node)
	{
		memset(new_node, 0, sizeof(Path));	
	}
	else
	{
		slog("Malloc didn't work, path node");
		return NULL;
	}
	
	if(parent)
	{
		new_node->parent = parent;
		new_node->g_val = parent->g_val;
		new_node->f_val = parent->f_val;
		
		new_node->tile_index = curr;
		parent->next = new_node;
	}
	else
	{
		new_node->tile_index = curr;
		new_node->h_val = 0;
		new_node->g_val = 0;
		new_node->f_val = 0;
		new_node->parent = NULL;
		add_closed_list(size, new_node); 
	}
	if(parent && tile_to_tile_dist(parent->tile_index, target) <= 1)
	{
		new_node->tile_index = target;
		new_node->parent = parent;
		return new_node;
	}
	new_node->tile_index = curr;

	if(curr == target) // hit base case
	{
		new_node->tile_index = target;
		return new_node;
	}

	type = tile_get_type(curr - TOTAL_TILES_X);
	int in_c_list = !check_closed_list(size, curr - TOTAL_TILES_X);
	int can_move = TILE_CAN_MOVE_UP(curr);
	if(TILE_CAN_MOVE_UP(curr) && tile_get_type(curr - TOTAL_TILES_X) != TILE_TREE
							  && !check_closed_list(size, curr - TOTAL_TILES_X))
	{
		option = curr - TOTAL_TILES_X;

	//	if(add_open_list(option, size, open_list))
		up.h_val = tile_dist_to_target(option, target) * 10;
		up.g_val = parent ? parent->g_val + 10 : 10;
		up.f_val = up.h_val + up.g_val;
		add_open_list(size, &up);
	/*	min = MIN(up.f_val, min);
		new_node->f_val = min == up.f_val ? up.f_val : new_node->f_val;
		new_node->g_val = min == up.f_val ? up.g_val : new_node->g_val;
		new_node->tile_index = min == up.f_val ? option : new_node->tile_index;*/
//	}
	/*type = tile_get_type(curr - TOTAL_TILES_X);
	int in_c_list = !check_closed_list(size, curr - 1);
	int can_move = TILE_CAN_MOVE_LEFT(curr);
	if(TILE_CAN_MOVE_LEFT(curr) && tile_get_type(curr -1) != TILE_TREE && !in_c_list )
	{
		option = curr -1;

		left.h_val = tile_dist_to_target(option, target) * 10;
		left.g_val = parent ? parent->g_val + 10 : 10;
		left.f_val = left.h_val + left.g_val;
		add_open_list(size, &left);
		/*new_node->f_val = min == left.f_val ? left.f_val : new_node->f_val;
		new_node->g_val = min == left.f_val ? left.g_val : new_node->g_val;
		new_node->tile_index = min == left.f_val ? option : new_node->tile_index;*/
/*	}


	type = tile_get_type(curr + 1);
	in_c_list = !check_closed_list(size, curr + 1);
	can_move = TILE_CAN_MOVE_RIGHT(curr);

	if(TILE_CAN_MOVE_RIGHT(curr) && tile_get_type(curr + 1) != TILE_TREE  && !in_c_list )
	{
		option = curr + 1;
		
		right.h_val = tile_dist_to_target(option, target);
		right.g_val = parent ? parent->g_val + 10 : 10;
		right.f_val = right.h_val + right.g_val;
		min = MIN(right.f_val, min);
		add_open_list(size, &right);
		/*new_node->f_val = min == right.f_val ? right.f_val : new_node->f_val;
		new_node->g_val = min == right.f_val ? right.g_val : new_node->g_val;
		new_node->tile_index = min == right.f_val ? option : new_node->tile_index;*/

	/*}

	type = tile_get_type(curr + TOTAL_TILES_X);
	in_c_list = !check_closed_list(size, curr +TOTAL_TILES_X);
	can_move = TILE_CAN_MOVE_DOWN(curr);

	if(TILE_CAN_MOVE_DOWN(curr) && tile_get_type(curr + TOTAL_TILES_X) != TILE_TREE 
								&& !in_c_list)
	{
		option = curr + TOTAL_TILES_X;
		down.h_val = tile_dist_to_target(option, target);
		down.g_val = parent ? parent->g_val + 10 : 10;
		down.f_val = down.h_val + down.g_val;
		min = MIN(down.f_val, min);
		add_open_list(size, &down);
	}	
	
	temp_node1 = get_priority_OL();

	if(min == 999) // no moves
	{
		return NULL;
	}
	if(new_node->g_val >= max_g_val)
	{
		return new_node;
	}

	if(new_node->next == NULL && tile_to_tile_dist(new_node->tile_index, target) > 1)
	{
		count = 0;
		//if no next search, go through open list
		for( i = 0; i < size - 1; i++)
		{
			if(open_list[i+1] == 9999)
			{
				count = i;
				break;
			}
		}
		while(i -1 >= 0 && !new_node->next)
		{
			if(open_list[i] != 9999)
			{
				new_node->next = getOptions(open_list[i], size, target, max_g_val, new_node);
			}
				--i;
		}
	}
	return new_node;
}*/