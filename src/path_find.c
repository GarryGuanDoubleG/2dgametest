#include "path_find.h"

//returns false if option is in list
//true if added to list
static Path closed_list[MAX_PATH_SIZE];
static Path open_list[MAX_PATH_SIZE];

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
void sort_open_list(int size)
{
	int done = false;
	int i;
	Path temp;
	while(!done){
		done = true;
		for(i = 0; i < size - 1; i++)
		{
			if(open_list[i].tile_index == open_list[i+1].tile_index)
			{
				done = true;
				break;
			}
			 if(open_list[i + 1].parent == open_list[i].parent 
				 && open_list[i+1].g_val < open_list[i].g_val)
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
			if(closed_list[i].f_val == -1)
			{
				closed_list[i] = path;
			}
			if(open_list[i].tile_index == path.tile_index)
			{
				open_list[i].tile_index = open_list[i].f_val = -1;
				sort_open_list(size);
			}
		}
	}
}

Path * get_priority_OL()
{
	return open_list[0].f_val != -1 ? &open_list[0] : NULL;
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
		sort_open_list(size);
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
		if(open_list[i].f_val == -1 || open_list[i].f_val == -1)
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
Path *get_path(int curr, int size, int target, int max_g_val, Path *parent)
{
	int i;
	int *options;
	// could use chunking as well
	Path * path = (Path*)malloc(sizeof(Path) * size);
	memset(path, 0, sizeof(Path) * size);
	//bfs a * approach
	path->tile_index = curr;
	path->next = path->parent = NULL;
	if(curr == target || tile_to_tile_dist(curr, target) == 1)
	{
		return path;
	}
	add_closed_list(size, *path);
	options = get_moves(curr);
	for( i = 0; i < 4; i++)
	{
		Path new_node;
		if(tile_get_type(options[i]) != TILE_TREE && options[i] != -1)
		{
			new_node.tile_index = options[i];
			new_node.parent = path;
			add_open_list(size, new_node);
		}
	}

	while(open_list[0].f_val != -1)
	{
		Path new_node = { &open_list[0], NULL, -1, -1, -1, -1};

		if(open_list[0].tile_index == target)
		{
			int j = 0;
			Path prev = open_list[0];
			path[0] = open_list[0];
			if(open_list[0].tile_index == target)
			{
				while(path[j++].parent)
				{
					path[j].next = &path[j-1];
				}
			}
			return path;
		}
		options = get_moves(open_list[0].tile_index);
		add_closed_list(size, open_list[0]);
		for( i = 0; i < 4; i++)
		{ 
			if(tile_get_type(options[i]) != TILE_TREE 
			&& options[i] != -1  
			&& !check_closed_list(size, options[i]) )
			{
				new_node.h_val = (tile_to_tile_dist( options[i], target));
				new_node.g_val = new_node.parent->g_val + 1;
				new_node.g_val = new_node.parent->g_val + new_node.parent->h_val;
				new_node.tile_index = options[i];

				add_open_list(size, new_node);
				sort_open_list(size);
			}
		}
	}
	return path;
}
/*
Path * getOptions(int curr, int size, int target, int max_g_val, Path *parent)
{
	int min = 999;
	int new_tile_index;
	Path adjacent_node;
	Path * temp_node;
	Path * new_node = (Path*)malloc(sizeof(Path));
	memset(new_node, 0, sizeof(Path));

	if(parent)
	{
		if(parent->g_val >= max_g_val)
		{
			return NULL;
		}
		new_node->parent = parent;
		new_node->g_val = parent->g_val + 10; 
	}
	else //must be first node
	{
		new_node->g_val = 0;
	}

	if((new_node->h_val = tile_to_tile_dist(curr, target)* 10) <= 10)
	{
		slog("found");
		new._node->tile_index = target;
		return new_node;
	}
	new_node->f_val = new_node->h_val + new_node->g_val;
	new_node->tile_index = curr;

	add_closed_list(size, new_node);
	new_tile_index = curr - TOTAL_TILES_X;
	//above tile moveable
	if(TILE_CAN_MOVE_UP(curr) 
	&& tile_get_type(curr - TOTAL_TILES_X) != TILE_TREE
	&& !check_closed_list(size, curr - TOTAL_TILES_X))
	{
		adjacent_node.g_val = new_node->g_val + 10;
		adjacent_node.h_val = tile_to_tile_dist( target, new_tile_index )* 10;
		adjacent_node.f_val = adjacent_node.g_val + adjacent_node.h_val;
		adjacent_node.tile_index = new_tile_index;
		adjacent_node.parent = new_node;
		add_open_list(size, &adjacent_node);
	}

	new_tile_index = curr - 1;
	if(TILE_CAN_MOVE_LEFT(curr) && tile_get_type(new_tile_index) != TILE_TREE
							  && !check_closed_list(size, new_tile_index))
	{
		adjacent_node.g_val = new_node->g_val + 10;
		adjacent_node.h_val = tile_to_tile_dist( target, new_tile_index ) * 10;
		adjacent_node.f_val = adjacent_node.g_val + adjacent_node.h_val;
		adjacent_node.tile_index = new_tile_index;
		adjacent_node.parent = new_node;

		add_open_list(size, &adjacent_node);
	}

	new_tile_index = curr + 1;
	if(TILE_CAN_MOVE_RIGHT(curr) && tile_get_type(new_tile_index) != TILE_TREE
							  && !check_closed_list(size, new_tile_index))
	{
		adjacent_node.g_val = new_node->g_val + 10;
		adjacent_node.h_val = tile_to_tile_dist( target, new_tile_index ) * 10;
		adjacent_node.f_val = adjacent_node.g_val + adjacent_node.h_val;
		adjacent_node.tile_index = new_tile_index;
		adjacent_node.parent = new_node;

		add_open_list(size, &adjacent_node);
	}
	new_tile_index = curr + TOTAL_TILES_X;
	if(TILE_CAN_MOVE_DOWN(curr) && tile_get_type(new_tile_index) != TILE_TREE
							  && !check_closed_list(size, new_tile_index))
	{
		adjacent_node.g_val = new_node->g_val + 10;
		adjacent_node.h_val = tile_to_tile_dist( target, new_tile_index ) * 10;
		adjacent_node.f_val = adjacent_node.g_val + adjacent_node.h_val;
		adjacent_node.tile_index = new_tile_index;
		adjacent_node.parent = new_node;

		add_open_list(size, &adjacent_node);
	}

	temp_node = get_priority_OL();
	if(!temp_node || temp_node->f_val == -1)
	{
		slog("Open List is Empty");
  		return new_node;
	}
	add_closed_list(size, temp_node);
	new_node->next = getOptions(temp_node->tile_index, size, target, max_g_val, temp_node->parent);

	return new_node;
}
*/
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
	Path init = {NULL, NULL, -1, -1, -1, -1};
	Path *path = NULL;

	if(curr_path)
	{
		path_free(curr_path);
	}

	for(i =0; i < MAX_PATH_SIZE; i++)
	{
		closed_list[i].tile_index = closed_list[i].f_val = init.f_val; //high value to make sorting easier
		open_list[i].tile_index = open_list[i].f_val = init.f_val;
	}

	//tile_heuristic *tile_heur = tile_get_heuristic( size, start_index, target_index);
	path = get_path(start_index, MAX_PATH_SIZE, target_index, MAX_PATH_SIZE, NULL);
	slog_path(path);
	/*if(tile_heur)
	{
		tile_list_heuristic_free(tile_heur);
	}*/

	return path;
}