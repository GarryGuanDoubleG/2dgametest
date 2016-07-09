#include "path_find.h"

//returns false if option is in list
//true if added to list
static Path *g_closed_list[MAX_PATH_SIZE];
static Path *g_open_list[MAX_PATH_SIZE];

static int g_cl_count;//number of elements in closed list
static int g_ol_count;//number of elements in open list

static Path * g_path_heads;
static Path * g_path_nodes;

extern const int g_entity_max;

static int g_max_path_heads;
static int g_max_path_nodes;
static const int g_path_length = 25;

int in_g_closed_list(int tile_index);

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

	memset(g_closed_list, 0, sizeof(Path*) * MAX_PATH_SIZE);
	memset(g_open_list, 0, sizeof(Path*) * MAX_PATH_SIZE);

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


void Path_Free_Node(Path **path)
{
	Path *head = *path;
	if(path){
		 head = *path;
		*path = (*path)->next;
		memset(head, 0, sizeof(Path));
		head = NULL;
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

/****************************************************************************
*                                   A Star									*
*****************************************************************************/
int in_g_open_list(int tile_pos)
{
	int i;

	for( i = 0; i < MAX_PATH_SIZE; i++)
	{
		//reached end of list
		if(!g_open_list[i])
		{
			return 0;
		}
		if(g_open_list[i]->tile_index == tile_pos 
			&& g_open_list[i]->priority != -1)
		{
			return 1;
		}
	}
	return 0;
}
int g_open_list_empty()
{
	return g_ol_count == 0 ? 1 : 0;
}

int Get_Heuristic(int start, int curr_pos, int target)
{
	int dist_to_target	= tile_to_tile_dist(curr_pos, target);
	int dist_to_start	= tile_to_tile_dist(curr_pos, start);

	return dist_to_target + dist_to_start;
}

void Reset_Lists()
{
	g_cl_count = 0;
	g_ol_count = 0;

	memset(g_open_list,	0, sizeof(Path*) * MAX_PATH_SIZE);
	memset(g_closed_list, 0, sizeof(Path*) * MAX_PATH_SIZE);
}

Path *open_list_get()
{
	int i = 0;

	while(i < MAX_PATH_SIZE)
	{
		if(!g_open_list[i])
			return NULL;

		if(g_open_list[i]->priority >= 0)
		{
			g_open_list[i]->priority = -1;
			break;
		}
		++i;
	}

	return g_open_list[i];
}

void add_g_open_list(Path *new_path)
{
	int i;

	int added;
	int new_priority;
	int new_tile_index;

	new_priority = new_path->priority;
	new_tile_index = new_path->tile_index;

	for(i = 0; i < MAX_PATH_SIZE - 1; i++)
	{
		//already checked this index
		if(in_g_closed_list(new_tile_index) ||
			in_g_open_list(new_tile_index))
		{
			//need to re-parent
			break;
		}
		if(!g_open_list[i])
		{
			g_open_list[i] = new_path;
			break;
		}
		if(new_priority >= g_open_list[i]->priority)
			continue;
		//found index with worse priority
		//shift elements below, down 1
		//insert new path
		memmove(&g_open_list[i + 1], &g_open_list[i], (MAX_PATH_SIZE - i - 1) *sizeof(Path *));
		g_open_list[i] = new_path;
		break;
	}
	++g_ol_count;
}

int in_g_closed_list(int tile_index)
{
	int i;

	for(i = 0; i < g_cl_count; i++)
	{
		if(!g_closed_list[i]) //reached end of list
			return 0;
		if(g_closed_list[i]->tile_index == tile_index)
			return 1;
	}

	return 0;
}

void add_g_closed_list(Path *new_path)
{
	int i;
	int j;
	int new_priority;
	int new_tile_index;

	new_priority = new_path->priority;
	new_tile_index = new_path->tile_index;

	if(in_g_closed_list(new_tile_index))
	{			
		return;
	}
	
	g_closed_list[g_cl_count++] = new_path;
	g_ol_count--;
}

//needs improvement
int *Get_Moves(int tile)
{
	static int options[4];
	options[0] = TILE_CAN_MOVE_UP(tile) && tile_moveable(tile - TILE_ROWS) ? tile - TILE_ROWS : -1;
	options[1] = TILE_CAN_MOVE_LEFT(tile) && tile_moveable(tile - 1 ) ? tile -1 : -1;
	options[2] = TILE_CAN_MOVE_RIGHT(tile) && tile_moveable(tile + 1) ? tile + 1 : -1;
	options[3] = TILE_CAN_MOVE_DOWN(tile) && tile_moveable(tile + TILE_ROWS) ? tile + TILE_ROWS : -1;

	return options;
}

//finds available nodes and singly links them
Path * Assign_Path(Path *path)
{
	int i;

	Path * curr_node = path;
	Path * temp = NULL;
	
	for(i = 0; i < g_max_path_nodes; i++)
	{
		if(g_path_nodes[i].inuse)
			continue;
		g_path_nodes[i].tile_index = curr_node->tile_index;
	
		if(temp)
			g_path_nodes[i].next = temp;
		temp = &g_path_nodes[i];
		
		if(curr_node->parent == NULL)
			break;
		curr_node = curr_node->parent;
	}
	return temp;
}
Path *Find_New_Path(int start, int target, int range)
{
	int i;
	int p_index;
	int *moves;

	Path *curr_node;
	Path *path;

	static Path p_nodes[MAX_PATH_SIZE]; //Temp nodes used for finding path
	static int	move_cost[MAX_PATH_SIZE];

	Reset_Lists();

	memset(p_nodes,		0,	sizeof(Path) *MAX_PATH_SIZE);
	memset(move_cost,	0,	sizeof(int) *MAX_PATH_SIZE);
  
	//current index of p_node
	p_index		= 0;
	path		= NULL;
	curr_node	= &p_nodes[p_index];
	
	curr_node->tile_index	= start;
	curr_node->priority		= tile_to_tile_dist(start, target);
	curr_node->next			= curr_node->parent = NULL;

	add_g_open_list(curr_node);
	
	while(!g_open_list_empty())
	{
		//get next node to check
		curr_node	= open_list_get();
		//no possible path, break
		if(!curr_node)
			break;
		//directional options from current node
		moves		= Get_Moves(curr_node->tile_index);

		//found target or checked too many path nodes
		if(curr_node->tile_index == target ||
			(g_ol_count + g_cl_count) >= range)
		{
			path = Assign_Path(curr_node);
			break;
		}

		for(i = 0; i < 4; i++)
		{
			//get next available node to point at
			Path *node = NULL;

			if(moves[i] == -1)
				continue;
			node = &p_nodes[++p_index];
			node->tile_index = moves[i];
			node->priority	= Get_Heuristic(start, node->tile_index, target);
			node->parent	= curr_node;

			add_g_open_list(node);
		}

		add_g_closed_list(curr_node);
	}
	//if no path, return what we found
 	return path;
}
Path* getPath(Vec2d self_pos, Vec2d target_pos, SDL_Rect self_bound, SDL_Rect target_bound, int range)
{
	int i;
	int start;
	int target;
	
	//maximum tiles to check before quitting path search
	const int path_size = range * range;

	start = Tile_Get_Index(self_pos, self_bound);
	target= Tile_Get_Index(target_pos, target_bound);

	return Find_New_Path(start, target, path_size);
}