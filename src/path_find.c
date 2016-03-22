#include "path_find.h"

//returns false if option is in list
//true if added to list

void path_free_node(Path *path)
{
	if(path){
		Path *head = path;
		path = path->next;
		free(head);
	}
	else
	{
		slog("no path node");
	}
}

void path_free(Path *path)
{
	Path * head = path;
	while ((path = head) != NULL) 
	{ // set curr to head, stop if list empty.
		head = head->next;          // advance head to next element.
		free(path);                // delete saved pointer.
	}
}
int check_closed_list(int option, int size, int closed_list[])
{
	int i;
	for(i = 0; i < size - 1; i++)
	{
		if(closed_list[i] == option)
		{
			return true;
		}
		if(closed_list[i] == 9999 && closed_list[i + 1] == 9999)
		{
			return false;
		}
	}
	return false;
}
int	add_open_list(int option, int size, int open_list[])
{
	int i;
	int done = false;
	int temp;
	//because I call this often, bubble sort is a good option
	while(!done){
		done = true;
		for(i = 0; i < size - 1; i++)
		{
			if(open_list[i + 1] == open_list[i] && open_list[i] == 9999)
			{
				done = true;
				break;
			}
			else if(open_list[i + 1] < open_list[i])
			{
				temp = open_list[i + 1];
				open_list[i + 1] = open_list[i];
				open_list[i] = temp;
				done = false;
			}
		}
	}
	
	while(i++ < size)
	{
		if(open_list[i] == option)
		{
			return false;
		}
		else
		{
			if(open_list[i] == 9999)
			{
				open_list[i] = option;
				return true;
			}
		}
	}
	return false;
}
int add_closed_list(int tile, int size, int open_list[], int closed_list[])
{
	
	int i;
	int done = false;
	int temp;
	//because I call this often, bubble sort is a good option
	while(!done){
		done = true;
		for(i = 0; i < size - 1; i++)
		{
			if(closed_list[i + 1] == closed_list[i] && closed_list[i] == 9999)
			{
				done = true;
				break;
			}
			else if(closed_list[i + 1] < closed_list[i])
			{
				temp = closed_list[i + 1];
				closed_list[i + 1] = closed_list[i];
				closed_list[i] = temp;
				done = false;
			}
		}
	}
	
	while(i++ < size)
	{
		if(closed_list[i] == tile)
		{
			return false;
		}
		else
		{
			if(closed_list[i] == 9999)
			{
				closed_list[i] = tile;
				return true;
			}
		}
	}
	return false;
}
Path * getOptions(int curr, int size, int target, int max_g_val, int closed_list[], int open_list[], Path *parent)
{

	int option = curr;
	int move = -1;
	int min = 999;
	int heuristic; // distance from tile to target tile
	int count;
	Path left;
	Path up;
	Path down;
	Path right;
	Path chosen_one;
	Path *new_node = (Path*)malloc(sizeof(Path));
	memset(new_node, 0, sizeof(Path));	
	
	if(parent)
	{
		new_node->parent = parent;
		new_node->g_val = parent->g_val;
		new_node->f_val = parent->f_val;

		parent->next = new_node;
	}
	else
	{
		new_node->tile_index = curr;
		new_node->parent = NULL;
	}

	if(curr == target) // hit base case
	{
		return NULL;
	}


	if(TILE_CAN_MOVE_UP(curr) && tile_get_type(curr + TOTAL_TILES_X) != TILE_TREE
							  && !check_closed_list(curr-1, size, closed_list))
	{
		option = curr - TOTAL_TILES_X;
		if( add_open_list(option, size, open_list))
		{
			up.h_val = tile_dist_to_target(option, target);
			up.g_val = parent ? parent->g_val + 10 : 10;
			up.f_val = up.h_val + up.g_val;
			min = MIN(up.f_val, min);
			new_node->f_val = min == up.f_val ? up.f_val : new_node->f_val;
			new_node->g_val = min == up.f_val ? up.g_val : new_node->g_val;
			new_node->tile_index = min == up.f_val ? option : new_node->tile_index;
		}
	}
	if(TILE_CAN_MOVE_LEFT(curr) && tile_get_type(curr -1) != TILE_TREE && !check_closed_list(curr-1, size, closed_list) )
	{
		option = curr -1;
		if( add_open_list(option, size, open_list))
		{
			left.h_val = tile_dist_to_target(option, target);
			left.g_val = parent ? parent->g_val + 10 : 10;
			left.f_val = left.h_val + left.g_val;
			min = MIN(left.f_val, min);
			new_node->f_val = min == left.f_val ? left.f_val : new_node->f_val;
			new_node->g_val = min == left.f_val ? left.g_val : new_node->g_val;
			new_node->tile_index = min == left.f_val ? option : new_node->tile_index;
		}
	}
	
	if(TILE_CAN_MOVE_RIGHT(curr) && tile_get_type(curr + 1) != TILE_TREE && !check_closed_list(curr + 1, size, closed_list))
	{
		option = curr + 1;
		if(add_open_list(option, size, open_list))
		{
			right.h_val = tile_dist_to_target(option, target);
			right.g_val = parent ? parent->g_val + 10 : 10;
			right.f_val = right.h_val + right.g_val;
			min = MIN(right.f_val, min);
			new_node->f_val = min == right.f_val ? right.f_val : new_node->f_val;
			new_node->g_val = min == right.f_val ? right.g_val : new_node->g_val;
			new_node->tile_index = min == right.f_val ? option : new_node->tile_index;
		}
	}

	if(TILE_CAN_MOVE_DOWN(curr) && tile_get_type(curr + TOTAL_TILES_X) != TILE_TREE 
								&& !check_closed_list(curr + TOTAL_TILES_X, size, closed_list))
	{
		option = curr + TOTAL_TILES_X;
		if( add_open_list(option, size, open_list))
		{
			down.h_val = tile_dist_to_target(option, target);
			down.g_val = parent ? parent->g_val + 10 : 10;
			down.f_val = down.h_val + down.g_val;
			min = MIN(down.f_val, min);

			new_node->f_val = min == down.f_val ? down.f_val : new_node->f_val;
			new_node->g_val = min == down.f_val ? down.g_val : new_node->g_val;
			new_node->tile_index = min == down.f_val ? option : new_node->tile_index;
		}
	}

	new_node->f_val = min;
	new_node->parent = parent ? parent : NULL;
	new_node->next = NULL;

	if(min == 999)
	{
		return NULL;
	}
	if(new_node->g_val >= max_g_val)
	{
		return new_node;
	}
	
	new_node->next = getOptions(new_node->tile_index, size, target, max_g_val, closed_list, open_list, &chosen_one);
	return new_node;
}
void slog_path(Path *path)
{
	if(!path)
	{
		slog("No path");
		return;
	}

	Path * temp = path;
	slog("Start pos is: %i ", path->tile_index);
	while(temp->next)
	{
		temp = temp->next;
		slog("Next: %i ", temp->tile_index);
	}
}
Path* getPath(int size, Vec2d *start, Vec2d *target, Path *curr_path)
{
	const int list_size = size * size;
	int start_index  = tile_get_tile_number(*start);
	int target_index = tile_get_tile_number(*target);
	int closed_list[MAX_PATH_SIZE];
	int open_list[MAX_PATH_SIZE];
	int i;

	if(curr_path)
	{
		path_free(curr_path);
	}


	for(i =0; i < MAX_PATH_SIZE; i++)
	{
		closed_list[i] = 9999; //high value to make sorting easier
		open_list[i] = 9999;
	}
	closed_list[0] = start_index;
	//tile_heuristic *tile_heur = tile_get_heuristic( size, start_index, target_index);
	Path * path = getOptions(start_index, MAX_PATH_SIZE, target_index, MAX_PATH_SIZE, closed_list, open_list, NULL);
	slog_path(path);
	/*if(tile_heur)
	{
		tile_list_heuristic_free(tile_heur);
	}*/

	return path;
}