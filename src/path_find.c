#include "path_find.h"

//returns false if option is in list
//true if added to list
static Path closed_list[MAX_PATH_SIZE];
static Path open_list[MAX_PATH_SIZE];
static Path * temp_path_nodes; // temp path for moving 



void path_free_node(Path **path)
{
	Path *head = *path;
	if(path){
		 head = *path;
		*path = (*path)->next;
		memset(head, 0, sizeof(Path));
		//free(head);
	}
	else
	{
		slog("no path node");
	}
}

void path_free(Path *&path)
{
	if(!path)
	{
		return;
	}
	Path * head = path;
	while ((path = head) != NULL) 
	{ // set curr to head, stop if list empty.
		head = head->next;          // advance head to next element.
		memset(path, 0, sizeof(Path));
		path = NULL;
	}
}

int check_closed_list(int size, int tile_index)
{
	int i = 0;
	for(i = 0; i < size - 1; i++)
	{
		if(closed_list[i].tile_index == tile_index)
			return true;
	}
	return false;
}

void sort_open_list(int size, int target)
{
	int done = false;
	int i;
	Path temp;
	while(!done){
		done = true;
		for(i = 0; i < size - 1; i++)
		{
			if(open_list[i].tile_index == target)
			{
				open_list[0] = open_list[i];
				return;
			}
			if(open_list[i].tile_index == open_list[i+1].tile_index)
			{
				done = true;
				break;
			}
			 if(open_list[i + 1].parent == open_list[i].parent 
				 && open_list[i+1].f_val < open_list[i].f_val)
			 {
					temp = open_list[i + 1];
					open_list[i + 1] = open_list[i];
					open_list[i] = temp;
					done = false;
			 }

			 if(open_list[i].tile_index == -1 && open_list[i+1].tile_index != -1)
			 {
				 temp = open_list[i + 1];
				 open_list[i + 1] = open_list[i];
				 open_list[i] = temp;
				 done = false;
			 }
			 else
			 {
				 break;
			 }

		}
	}
}

void add_closed_list(int size, Path path)
{
	int i = 0;
	int found = false;
	for(i = 0; i < size - 1; i++)
	{
		if(closed_list[i].tile_index == path.tile_index)
		{
			closed_list[i] = path;
			found = true;
		}	
		if(closed_list[i].tile_index == closed_list[i+1].tile_index)
		{

			break;
		}
	}
	if(!found)
	{
		for(i = 0; i < size - 1; i++)
		{
			if(closed_list[i].f_val >= 999)
			{
				closed_list[i] = path;
				break;
			}
		
		}

		for(i = 0; i < size - 1; i++)
		{
			if(open_list[i].tile_index == path.tile_index)
			{
				open_list[i].tile_index = -1;
				open_list[i].f_val = 999;
			}		
		}
	}
}

Path * get_priority_OL()
{
	return open_list[0].f_val < 999 ? &open_list[0] : NULL;
}


void add_open_list(int size, Path new_path)
{
	int i = 0;
	int done = false;
	Path temp;
	//because I call this often, bubble sort is a good option
	if(new_path.tile_index < 0)
	{
		slog("path < 0");
	}

	while(!done){
		done = true;
		for(i = 0; i < size - 1; i++)
		{
		 if(open_list[i].tile_index == open_list[i+1].tile_index) break;
		 if(open_list[i + 1].f_val < open_list[i].f_val
			 &&open_list[i].tile_index != open_list[i+1].tile_index)
			{
				temp = open_list[i + 1];
				open_list[i + 1] = open_list[i];
				open_list[i] = temp;
				done = false;
			}
		}
	}	

	for(i = 0; i < size; i++)
	{
		if(open_list[i].tile_index == open_list[i+1].tile_index) break;
		if(open_list[i].tile_index == new_path.tile_index)
		{
			if(new_path.g_val < open_list[i].g_val)
			{
				open_list[i].parent = new_path.parent;
				open_list[i].g_val = new_path.g_val;
			}
			return;
		}
	}

	i = 0;
	while(i < size)
	{
		if(open_list[i].f_val >= 999 || open_list[i].tile_index == -1)
		{
			open_list[i] = new_path;
			break;
		}
		++i;
	}
}

int *get_moves(int tile)
{
	static int options[4];
	options[0] = TILE_CAN_MOVE_UP(tile) ? tile - TOTAL_TILES_X : -1;
	options[1] = TILE_CAN_MOVE_LEFT(tile) ? tile -1 : -1;
	options[2] = TILE_CAN_MOVE_RIGHT(tile) ? tile + 1 : -1;
	options[3] = TILE_CAN_MOVE_DOWN(tile) ? tile + TOTAL_TILES_X : -1;

	return options;
}
Path *get_path(int curr, int size, int target, int max_g_val)
{
	int i;
	int *options;
	int temp_index = 1;
	// could use chunking as well
	Path * parent_paths = (Path*)malloc(sizeof(Path) * size);
	memset(parent_paths, 0, sizeof(Path) * size);
	Path * path = (Path*)malloc(sizeof(Path) * size);
	memset(path, 0, sizeof(Path) * size);
	//bfs a * approach
	if(curr == target || tile_to_tile_dist(curr, target) == 1)
	{
		path->tile_index = target;
		return path;
	}

	path->tile_index = curr;
	path->next = path->parent = NULL;
	path->h_val = tile_to_tile_dist(curr, target);
	path->g_val = 0;
	path->f_val = path->h_val + path->g_val;

	add_closed_list(size, *path);
	options = get_moves(curr);
	for( i = 0; i < 4; i++)
	{
		Path new_node;

		if(tile_get_type(options[i]) != TILE_TREE && options[i] != -1)
		{
			new_node.tile_index = options[i];
			new_node.parent = path;
			new_node.g_val = new_node.parent->g_val + 1;
			new_node.h_val = tile_to_tile_dist(options[i], target);
			new_node.f_val = new_node.g_val + new_node.h_val;
			add_open_list(size, new_node);
		}
	}

	while(open_list[0].f_val != 999)
	{
		Path new_node = {&parent_paths[temp_index++], NULL, -1, 999,999,999};
		*new_node.parent = open_list[0];
		//checks if we found target tile
		if(open_list[0].tile_index == target)
		{
			int j = 0;
			parent_paths->parent = open_list[0].parent;
			while(parent_paths->parent)
			{	
				parent_paths->parent->next = parent_paths;
				parent_paths = parent_paths->parent;
			}
			return path;
		}
		//if not, get options
		options = get_moves(open_list[0].tile_index);
		//add curernt tile to closed list
		add_closed_list(size, open_list[0]);
		//add options to open_list
		for( i = 0; i < 4; i++)
		{ 
			if(tile_get_type(options[i]) != TILE_TREE 
			&& options[i] != -1  
			&& !check_closed_list(size, options[i]) )
			{
				new_node.h_val = (tile_to_tile_dist( options[i], target));
				new_node.g_val = new_node.parent->g_val + 1;
				new_node.f_val = new_node.parent->g_val + new_node.parent->h_val;
				new_node.tile_index = options[i];
				add_open_list(size, new_node);
				sort_open_list(size,target);
			}
		}
		sort_open_list(size,target);
	}
	//if no path, return what we found
	return path;
}

void slog_path(Path *path)
{
	Path * head = path;
	Path * temp = path;
	if(!path)
	{
		slog("No path");
		return;
	}

	slog("START\nSTART\nSTART");
	slog("Start pos is: %i ", path->tile_index);

	while ((temp = head) != NULL) 
	{ // set curr to head, stop if list empty.
		if(!head->next)
		{
			break;
		}	
		head = head->next;          // advance head to next element.
		slog("Next: %i", head->tile_index);
	}
	slog("END\nEND\nEND");
	slog("Path head is %", path->tile_index);
}


Path* getPath(int size, Vec2d *start, SDL_Rect start_bound, SDL_Rect target_bound, Vec2d *target, Path *curr_path)
{
	const int list_size = size * size;
	int start_index  = tile_get_tile_number(*start, start_bound);
	int target_index = tile_get_tile_number(*target, target_bound);
	int i;
	Path init = {NULL, NULL, -1, 999, 999, 999};
	Path *path = NULL;

	if(curr_path)
	{
		path_free(curr_path);
	}

	for(i =0; i < MAX_PATH_SIZE; i++)
	{
		closed_list[i].tile_index = init.tile_index;
		closed_list[i].f_val = init.f_val; //high value to make sorting easier
		open_list[i].tile_index = init.tile_index;
		open_list[i].f_val = init.f_val;
	}

	//tile_heuristic *tile_heur = tile_get_heuristic( size, start_index, target_index);
	path = get_path(start_index, MAX_PATH_SIZE, target_index, MAX_PATH_SIZE);
	slog_path(path);
	/*if(tile_heur)
	{
		tile_list_heuristic_free(tile_heur);
	}*/

	return path;
}