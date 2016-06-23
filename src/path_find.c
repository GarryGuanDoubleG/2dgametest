#include "path_find.h"

//returns false if option is in list
//true if added to list
static Path closed_list[MAX_PATH_SIZE];
static Path open_list[MAX_PATH_SIZE];

static Path * g_path_heads;
static Path * g_path_nodes;

extern const int g_entity_max;

static int g_max_path_heads;
static int g_max_path_nodes;
static const int g_path_length = 25;


void Path_Close_System()
{
	memset(g_path_heads, 0, sizeof(Path) * g_entity_max);
	memset(g_path_nodes, 0, sizeof(Path) * g_entity_max * g_path_length);

	g_path_heads = NULL;
	g_path_nodes = NULL;
}

void Path_Init_System()
{
	g_max_path_heads = g_entity_max;
	g_max_path_nodes = g_entity_max * g_path_length;

	//each entity has a path head
	g_path_heads = (Path *) malloc(sizeof(Path) * g_max_path_heads);
	//each path can store up to 25 path nodes
	g_path_nodes = (Path *) malloc(sizeof(Path) * g_max_path_nodes);

	memset(g_path_heads, 0, sizeof(Path) * g_entity_max);
	memset(g_path_nodes, 0, sizeof(Path) * g_entity_max * g_path_length);

	atexit(Path_Close_System);
}

Path * Path_New_Head()
{
	int i;

	for(i = 0; i < g_max_path_heads; i++)
	{
		if(g_path_heads[i].parent == g_path_heads[i].next == NULL)
		{
			return &g_path_heads[i];
		}
	}

	return NULL;
}

Path * Path_New_Node()
{
	int i;

	for(i = 0; i < g_max_path_heads; i++)
	{
		if(g_path_nodes[i].parent == g_path_nodes[i].next == NULL)
		{
			return &g_path_nodes[i];
		}
	}
}


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

void add_closed_list(Path path)
{
	int i = 0;
	int found = false;

	for(i = 0; i < MAX_PATH_SIZE - 1; i++)
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
		for(i = 0; i < MAX_PATH_SIZE - 1; i++)
		{
			if(closed_list[i].f_val >= 999)
			{
				closed_list[i] = path;
				break;
			}
		
		}

		for(i = 0; i < MAX_PATH_SIZE - 1; i++)
		{
			if(open_list[i].tile_index == path.tile_index)
			{
				open_list[i].tile_index = -1;
				open_list[i].f_val = 999;
			}		
		}
	}
}

int open_list_empty()
{
	int i;
	for(i = 0; i < MAX_PATH_SIZE; i++)
	{
		if(open_list[i].tile_index != -1)
		{
			return 0;
		}
	}

	return 1;
}

Path * get_priority_OL()
{
	return open_list[0].f_val < 999 ? &open_list[0] : NULL;
}


void add_open_list(Path new_path)
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

//needs improvement
int *get_moves(int tile)
{
	static int options[4];
	options[0] = TILE_CAN_MOVE_UP(tile) ? tile - TILE_ROWS : -1;
	options[1] = TILE_CAN_MOVE_LEFT(tile) ? tile -1 : -1;
	options[2] = TILE_CAN_MOVE_RIGHT(tile) ? tile + 1 : -1;
	options[3] = TILE_CAN_MOVE_DOWN(tile) ? tile + TILE_ROWS : -1;

	return options;
}
Path *Find_New_Path(int start_tile, int target, int range)
{
	int i;
	int p_index;
	int *options;
	int distance;

	static Path p_nodes[MAX_PATH_SIZE]; //Temp nodes used for finding path
	
	Path curr_node;
	Path * start_path;

	start_path = Path_New_Head();
	start_path->tile_index = start_tile;
	start_path->priority = 0;

	memset(p_nodes, 0, sizeof(p_nodes));
	
	//current index of p_node
	p_index = 0;
	curr_node = p_nodes[p_index];

	//bfs a * approach
	//if next to target, set path to target
	distance = tile_to_tile_dist(start_tile, target);

	if(distance <= 1)
	{
		start_path->parent = NULL;
		start_path->next = NULL;

		start_path->tile_index = target;
		return start_path;
	}

	curr_node.tile_index = start_tile;
	curr_node.priority	= distance;

	add_open_list(curr_node);

	while(!open_list_empty())
	{
		
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


Path* getPath(Vec2d *self_pos, Vec2d *target_pos, SDL_Rect self_bound, SDL_Rect target_bound, int range)
{
	int i;
	int start_index;
	int target_index;
	
	const int list_size = range * range;

	Path *path;

	start_index = Tile_Get_Index(*self_pos, self_bound);
	target_index = Tile_Get_Index(*target_pos, target_bound);

	path = Find_New_Path(start_index, MAX_PATH_SIZE, target_index, list_size);

	return path;
}