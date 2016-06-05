#include <time.h>
#include <math.h>
#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "types.h"
#include "Tile.h"
#include "level.h"
#include "load.h"
#include "save.h"
#include "simple_logger.h"
#include "json_parse.h"

const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;
const int TILE_ROWS = 100;
const int TILE_COLUMNS = 100;
const int TOTAL_TILES = TILE_ROWS * TILE_COLUMNS ;

//array handle on tile types and coordinates on game
Tile *tile_list = NULL;
Destructable_Tile *dest_tile_list = NULL;
//array of tile scores for procedural forest gen

//have file in / o
//sprite and tile assets

GHashTable *g_tile_sprites;

// should refactor to use file input and output and store in ghash
int * tile_list_scores = NULL; 
int G_Editor_Mode = Bool_False;

void tile_editor_mode_set();
void tile_load_assets();
/**
* @brief loads tile sprites to be loaded into game and calls tile set to set the tile map
  Allocates memory for each tile in map and each destructable tile on the map
*/
void tile_init_system(int mode)
{
	if(TOTAL_TILES == 0){
		printf("Why is total tiles 0?");
		return;
	}

	tile_list		 = (Tile*) malloc(sizeof(Tile)*TOTAL_TILES);
	dest_tile_list	 = (Destructable_Tile*) malloc(sizeof(Destructable_Tile)*TOTAL_TILES);
	tile_list_scores = (int*) malloc(sizeof(int) * TOTAL_TILES);

	memset(tile_list,0, sizeof(Tile) * TOTAL_TILES);
	memset(dest_tile_list,0, sizeof(Destructable_Tile) * TOTAL_TILES);
	memset(tile_list_scores, 0, sizeof(int) * TOTAL_TILES);

	tile_load_assets();

	if(mode == 1)
	{
		tile_new_game_set();
	}
	if(mode == 2)
	{
		tile_editor_mode_set();
	}

	atexit(tile_close_system);
}

void tile_load_from_def(int *tile_map)
{
	int i;
	int x = 0,y = 0;
	tile_init_system(Bool_False);

	//set tile list and destructable tiles based on loaded tile map
	//reset hit values for destructable tiles
	for(i = 0; i < TOTAL_TILES; i++)
	{
		tile_list[i].mType = tile_map[i];
		dest_tile_list[i].mType = tile_map[i];

		tile_list[i].mBox	   = New_SDL_Rect(x, y, TILE_WIDTH, TILE_HEIGHT);
		dest_tile_list[i].mBox = tile_list[i].mBox;

		dest_tile_list[i].hits = 5;

		x += TILE_WIDTH;		
		if(x >= TILE_ROWS * TILE_WIDTH)
		{
			x = 0;
			y += TILE_HEIGHT;
		}
	}
	//not very efficient but free tile_map
	memset(tile_map, 0, sizeof(int) * TOTAL_TILES);
}

/*
* @brief loads the tile sprite from filepath into memory
* @param cstring for filepath to open
* @return pointer to Sprite
*/

Sprite * tile_load(char *filename)
{
	Sprite * sprite = Sprite_Load(filename, TILE_WIDTH, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
	return sprite;
}

/*
* @brief used for provide a huerestic on which tile is preferable to create a road tile
* Tiles that are adjacent to roads have higher scores and are less desirable
* tiles that are roads are extremely undesirable to become a road.
* @param move is the tile being evaluated for becoming a road tile
* @return int score of the move tile
*/
void get_moves(int * moves, int size, int move)
{

	if(!TILE_CAN_MOVE_LEFT(move))
	{
		moves[0] = -1;//cannot move to this tile if false
	}
	if(!TILE_CAN_MOVE_RIGHT(move))
	{
		moves[1] = -1;
	}
	if(!TILE_CAN_MOVE_UP(move))
	{
		moves[2] = -1;
	}
	if(!TILE_CAN_MOVE_DOWN(move))
	{
		moves[3] = -1;
	}
}

/**
* @brief based on a manicured drunken man's walk
* evaluates all adjacent nondiagonal tiles from the current tile and choose the lowest scoring 
* tile to become a road tile
* @param int array for 4 adjacent tiles to current tile
* @return the tile index to move to
*/
int tile_forest_walk(int moves[])
{
	int min = 999;
	int new_node = -1;
	int i;
	int size = 4;
	int move = -1;

	for(i = 0; i < size; i++)//4 is number of moves
	{
		int a = 0, b = 0;
		int random_num;
		int temp = min;
		if(moves[i] == -1)
		{
			continue;
		}		
		min = MIN(min, tile_list_scores[moves[i]]);
		if(temp > min)
		{
			new_node = moves[i];
			continue;
		}
		a = DISTANCE_CENTER(new_node);
		b = DISTANCE_CENTER(moves[i]);
		//tiles closer to the center have a higher chance of being the next move
		if(a < b)
		{
			//give smaller num 66% chance of being new move
			random_num = rand() % 9;
			new_node = random_num <= 5 ? new_node : moves[i];
		}
		else if ( b < a)
		{
			random_num = rand() % 9;
			new_node = random_num <=5 ? moves[i] : new_node;
		}
		//if both tiles are equally closer to the center, both have an equal chance of being next move
		else
		{
			random_num = rand() % 1;
			new_node = random_num == 0 ? moves[i] : new_node;
		}
	}
	return new_node; // return new tile index
}
/*
* @brief initiates the forest generation, setting the max number of tiles(lifespan) that can be set to road
* sets the possible moves and send it to tile_forest_walk to be evaluated
* @param int tile index to start the forest walk. 
*/
void tile_forest_gen(int start)
{
	int i = start;
	int done = false;
	int lifespan = TOTAL_TILES / 20;
	//-1 means can't move to that node
	tile_list[i].mType = TILE_ROAD;
	dest_tile_list[i].mType = TILE_ROAD;

	while( 0 < lifespan--)
	{
		int moves[4] = {i-1, i+1, i-TILE_ROWS, i + TILE_ROWS}; // left,right,up,down a tile on map
		int j;

		get_moves(moves, 4, i);//4 directions
		i = tile_forest_walk(moves);
		//update scores here
		//road tiles have heur score +10
		//adjacent tiles to road have score +3
		tile_list_scores[i] += 10;
		moves[0] = i - 1;
		moves[1] = i + 1;
		moves[2] = i - TILE_ROWS;
		moves[3] = i + TILE_ROWS;
		get_moves(moves, 4, i);//4 directions
		for(j = 0; j < 4; j++)
		{
			if(moves[j] == -1)
			{
				continue;
			}
			tile_list_scores[moves[j]] += 3; 
		}
		tile_list[i].mType = TILE_ROAD;
		dest_tile_list[i].mType = TILE_ROAD;
	}
}
/**
* @brief starts a manicured drunken man's walk across each edge of the map
* start location is a randomly chosen tile on the edge
* drunken man's walk moves in random directions towards the center
* each tile the man walks on becomes a road tile
*/

void tile_forest_gen()
{
	int start;

	start = 0;
	slog("Starting FGEN 0 TILE: %i", start);
	tile_forest_gen(start);
	//start of top edge of forest
	start = rand() % TILE_ROWS -1; //rand num from 0 to end of first row
	slog("Starting FGEN 2 TILE: %i", start);
	tile_forest_gen(start);
	//start on left edge of forest
	start = rand() % TILE_COLUMNS + 1;
	start = start >= TILE_COLUMNS ? TILE_COLUMNS - 1 : start;
	start = start * TILE_ROWS;
	slog("Starting FGEN 3 TILE: %i", start);
	tile_forest_gen(start);
	//right edge
	start = rand() % TILE_COLUMNS +1;
	start = start >= TILE_COLUMNS ? TILE_COLUMNS - 1 : start;
	start = (start *TILE_ROWS) - 1;
	slog("Starting FGEN 4 TILE: %i", start);

	tile_forest_gen(start);
	//bottom edge
	start = rand() % TILE_ROWS -1;
	start += TOTAL_TILES - TILE_ROWS;
	slog("Starting FGEN 5 TILE: %i", start);
	tile_forest_gen(start);
	slog("Finished Gen");
}
/*
* @brief used for displaying all the tile indexes of the map and its destructable tile type
*/
void slog_dest_tree_list()
{
	int i;
	for(i = 0; i < TOTAL_TILES; i++)
	{
		slog( "Index:%i TYPE:%i", i, dest_tile_list[i].mType);
	}
}

/**
* @brief sets the whole map to have grass tiles and tree destructable tiles
* future plans for procedurally setting the map include applying several layers of
* perlin noise to create natural rock and river scenary.
*/
void tile_new_game_set(){
	int i;
	int x = 0,y = 0;
	Tile * tile;
	//place the grass tile for all tiles on the map
	for( i = 0; i < TOTAL_TILES; i++)
	{

		tile = &tile_list[i];
		tile->mBox.x = x;
		tile->mBox.y = y;
		tile->mBox.w = TILE_WIDTH;
		tile->mBox.h = TILE_HEIGHT;
		tile->mType = TILE_TREE;//filler

		//set destructable tile to be a tree by default
		dest_tile_list[i].mBox = tile->mBox;
		dest_tile_list[i].mType = TILE_TREE;
		dest_tile_list[i].hits = 5;
		x += TILE_WIDTH;		
		if(x >= TILE_ROWS * TILE_WIDTH)
		{
			x = 0;
			y += TILE_HEIGHT;
		} 
	}
	tile_forest_gen(); // procedurally generate forest	
}

void tile_editor_mode_set()
{
	int i;
	int x = 0,y = 0;

	Tile * tile;
	Destructable_Tile * dest_tile;

	G_Editor_Mode = Bool_True;

	for( i = 0; i < TOTAL_TILES; i++)
	{
		tile = &tile_list[i];
		dest_tile = &dest_tile_list[i];

		tile->mBox = New_SDL_Rect(x, y, TILE_WIDTH, TILE_HEIGHT);
		tile->mType = TILE_GRASS;//filler

		dest_tile->mBox = tile->mBox;
		dest_tile->mType = TILE_GRASS;
		dest_tile->hits = 5;

		x += TILE_WIDTH;	

		if(x >= TILE_ROWS * TILE_WIDTH)
		{
			x = 0;
			y += TILE_HEIGHT;
		} 
	}
}
/**
* @brief frees the tile type to be reset
* @param pointer to tile to free
*/
void tile_free(Tile *tile){
	tile->mType = 0;
}

/**
* @brief returns the type of a tile given its tile index
* @param int tile index used to get tile type
*/
int tile_get_type(int index)
{
	if(index < 0 || index > TOTAL_TILES)
	{
		return -1;
	}
	return dest_tile_list[index].mType;
}

void tile_editor_set_type(int tile_index, int new_type)
{
	if(tile_index < TOTAL_TILES && tile_index >= 0 )
	{
		tile_list[tile_index].mType = new_type;
		dest_tile_list[tile_index].mType = new_type;
	}
}
/**
* @brief calculates the distance between tiles by counting how many steps to move before one reaches the other 
* @param indexes of both tiles in tile_list and calculate the tiles in between
* @return number of tiles between another tile
*/
//needs refactoring
int tile_to_tile_dist(int tile_1, int tile_2)
{
	int move_left = 0, move_right = 0, move_down = 0, move_up = 0;
	int start = tile_1;
	Vec2d t1_pos = tile_get_pos(tile_1);
	Vec2d t2_pos = tile_get_pos(tile_2);

	if(t2_pos.x < t1_pos.x)
	{
		while(tile_1 % (TILE_ROWS) != 0 && tile_get_pos(tile_2).x <  tile_get_pos(tile_1).x )
		{
			tile_1--;
			move_left++;
		}
		tile_1 = start;
	}
	else if(t2_pos.x > t1_pos.x)
	{
		while(tile_1 %(TILE_ROWS-1) != 0 && tile_get_pos(tile_2).x >  tile_get_pos(tile_1).x )
		{
			tile_1++;
			move_right++;
		}
		tile_1 = start;
	}
	if(t2_pos.y < t1_pos.y)
	{
		while(tile_1 > (TILE_ROWS) && tile_get_pos(tile_2).y <  tile_get_pos(tile_1).y)
		{
			tile_1 -= TILE_ROWS;
			move_up++;
		}
		tile_1 = start;
	}
	if(t2_pos.y > t1_pos.y)
	while(tile_1 < (TOTAL_TILES - TILE_ROWS) && tile_get_pos(tile_2).y >  tile_get_pos(tile_1).y)
	{
		tile_1 += TILE_ROWS;
		move_down++;
	}
	
	return move_left + move_right + move_up + move_down;
}
/**
* @brief draws the tiles on the screen using tile type as reference for which sprite to draw
*/
void tile_draw(){
	int i;

	Vec2d draw_pos;
	Vec2d cam_pos;
	Vec2d editor_offset;

	SDL_Rect camera;
	SDL_Rect tile_box;
	SDL_Rect dest;

	SDL_Renderer * renderer = Graphics_Get_Renderer();
	
	editor_offset = Camera_Get_Editor_Offset();
	cam_pos = Camera_GetPosition();
	camera = Camera_Get_Camera();	

	Sprite *sprite;
	Line key;

	for( i = 0; i < TOTAL_TILES; i++)
	{
		Tile * tile = &tile_list[i];		

		if(tile == NULL){
			slog("Tile is null while rendering");
			return ;
		}

		Vec2dSet(draw_pos, tile->mBox.x, tile->mBox.y);
		
		if(G_Editor_Mode == Bool_True)
		{	
			tile_box = New_SDL_Rect(tile->mBox.x + editor_offset.x, 
							 tile->mBox.y + editor_offset.y, 
							 TILE_WIDTH, TILE_HEIGHT);
			Vec2dAdd(draw_pos, editor_offset, draw_pos);
			Vec2dAdd(draw_pos, editor_offset, draw_pos);
		}
		else
		{
			tile_box = New_SDL_Rect(tile->mBox.x, tile->mBox.y,
								    TILE_WIDTH,   TILE_HEIGHT);
		}
		if(rect_collide(camera, tile_box))
		{
			//draw grass
			itoa(TILE_GRASS, key, 10);
			sprite = (Sprite *) g_hash_table_lookup(g_tile_sprites, key);
			Sprite_Draw(sprite, 0, draw_pos);

			itoa(tile->mType, key, 10);
			sprite = (Sprite *)g_hash_table_lookup(g_tile_sprites, key);
			Sprite_Draw(sprite, 0, draw_pos);
		}
	}
}
/**
* @brief clears tile_list and sets values to 0(free)
* frees the sprites used by the tile
*/
void tile_close_system(){
	slog("close system");
	memset(tile_list, 0, sizeof(Tile) * TOTAL_TILES);
	memset(dest_tile_list, 0, sizeof(Destructable_Tile) * TOTAL_TILES);
	memset(tile_list_scores, 0, sizeof(int) * TOTAL_TILES);
	slog("finished closing tiles");
/*
	SDL_DestroyTexture(tile_sprite_grass->image);`
	SDL_DestroyTexture(tile_sprite_tree->image);
*/
}
 
/**
* @brief finds a road tile and gives it a twenty percent chance for an AI to start on
* returns the first tile in the tile list by default
* @return  copy of the tile a monster would start on 
*/

Tile tile_start()
{
	int i;
	for( i = 0; i < TOTAL_TILES; i++)
	{
		if(tile_list[i].mType == TILE_ROAD || tile_list[i].mType == TILE_GRASS )
		{			
			if(rand() % 20 <= 1 && i != 0)
			{
				return tile_list[i];
			}
		}
	}

	return tile_list[0];
}

/**
* @brief checks to see if a position and bound collide with a tile
* @param Vec2d position of Entity, SDL_Rect bounding box to check collision
* @return 1(true) if position & bound collide with a tree
*/
int tile_collision(Vec2d pos, SDL_Rect bound)
{
	int i;
	Rect_f player_pos = {pos.x + bound.x, pos.y + bound.y, bound.w, bound.h};
	Rect_f tile_bound;

	for( i = 0; i < TOTAL_TILES; i++)
	{
		//offset to make tile collision more lenient
		tile_bound.x = tile_list[i].mBox.x + TILE_WIDTH * .2f;
		tile_bound.y = tile_list[i].mBox.y + TILE_HEIGHT * .2f;
		tile_bound.w = tile_list[i].mBox.w * .6f;
		tile_bound.h = tile_list[i].mBox.h * .8f;

		if(rect_collide(player_pos, tile_bound))
		{
			if(tile_list[i].mType != TILE_GRASS && tile_list[i].mType != TILE_ROAD)
			{
				return true;
			}		
		}
	}
	return false;
}

/**
* @brief player has an ability to destroy(forage) a tree for wood
* check if Entity is facing a tree and collides with it
* @param position of forager, SDL_Rect bounding box for collision against tree, int face direction to check if facing tree
* @return tree if can forage
*/

int tile_forage(Vec2d pos, SDL_Rect bound, int face_dir)
{
	int i;
	int curr_tile;
	int is_tree = false;
	int tree_index;
	Rect_f player_pos = {pos.x + bound.x+bound.w/2, pos.y + bound.y + bound.h/2, bound.w/2, bound.h/2};
	Rect_f tile_bound;

	for( i = 0; i < TOTAL_TILES; i++)
	{
		tile_bound.x = tile_list[i].mBox.x;
		tile_bound.y = tile_list[i].mBox.y;
		tile_bound.w = tile_list[i].mBox.w;
		tile_bound.h = tile_list[i].mBox.h;
		
		if(rect_collide(player_pos, tile_bound))
		{
			curr_tile = i;
			break;
		}
	}

	switch(face_dir)
	{
		case UP:
			slog("Up");
			if(curr_tile < TILE_ROWS)
			{
				return false;
			}
			if (dest_tile_list[curr_tile - TILE_ROWS].mType == TILE_TREE)
			{
				is_tree = true;
				tree_index = curr_tile - TILE_ROWS;
			}
			break;
		case DOWN:
			slog("Down");
			if(curr_tile > TOTAL_TILES - TILE_ROWS)
			{
				return false;
			}
			if(dest_tile_list[curr_tile + TILE_ROWS].mType == TILE_TREE)
			{
				is_tree = true;
				tree_index = curr_tile + TILE_ROWS;
			}
			break;
		case LEFT:
			slog("Left");
			if(curr_tile % TILE_ROWS == 0)
			{
				return false;
			}
			if (dest_tile_list[curr_tile - 1].mType == TILE_TREE)
			{
				is_tree = true;
				tree_index = curr_tile - 1;
			}
			break;
		case RIGHT:
			slog("Right");
			if(curr_tile % (TILE_ROWS -1 ) == 0)
			{
				return false;
			}
			if(dest_tile_list[curr_tile + 1].mType == TILE_TREE)
			{
				tree_index = curr_tile + 1;
				is_tree = true;
			}
			break;
		default:
			break;
	}

	if(is_tree)
	{
		dest_tile_list[tree_index].hits -= 1;
		if( dest_tile_list[tree_index].hits == 0)
		{
			dest_tile_list[tree_index].mType = NULL;
			if(tile_list[tree_index].mType == TILE_TREE)
			{
				tile_list[tree_index].mType = TILE_ROAD;
			}

			inventory_add("wood");
		}
	}
	slog("Is tree is %s", is_tree ? "TRUE" : "False");
	return is_tree;
}

/**
* @brief finds the tile index of an Entity based on the center of its body
* @param Vec2d pos - x and y position of Entity, SDL_Rect bound - used for collision and finding center
* @return tile index of Entity
*/
int tile_get_tile_number(Vec2d pos, SDL_Rect bound)
{
	int i;
	Vec2d bound_pos = {pos.x + bound.x + bound.w/2, pos.y + bound.y + bound.h/2};
	Rect_f tile_box;

	for(i = 0; i < TOTAL_TILES; i++)
	{
		tile_box.x = tile_list[i].mBox.x;
		tile_box.y = tile_list[i].mBox.y;
		tile_box.w = tile_list[i].mBox.w;
		tile_box.h = tile_list[i].mBox.h;

		if(bound_pos.x >= tile_box.x && bound_pos.x <= tile_box.x + tile_box.w &&
			bound_pos.y >= tile_box.y && bound_pos.y <= tile_box.y + tile_box.h )
		{
			return i;
		}
	}

	return -1;
}

/**
* @brief physical distance between two tiles
* currently used for generating heuristics for pathing
* @param tile index of two tiles
* @return float distance between tile
*/
float tile_dist_to_target(int start, int target)
{
	Vec2d vec_start = { tile_list[start].mBox.x, tile_list[start].mBox.y };
	Vec2d vec_target = { tile_list[target].mBox.x, tile_list[target].mBox.y };
	return Vec2dDistance(vec_start, vec_target);
}
/**
* @brief get the x and why position of a tile based on tile index in tile list
* @param tile index of desired tile 
* @return x and y position in a Vec2d of tile
*/
Vec2d tile_get_pos(int index)
{
	Vec2d tile_pos = {tile_list[index].mBox.x, tile_list[index].mBox.y };

	return tile_pos;	
}

int *Tile_Map_Get(int &tile_count, int &row, int& col)
{
	static int tile_map[TOTAL_TILES];
	int i;

	tile_count = TOTAL_TILES;
	row = TILE_ROWS;
	col = TILE_COLUMNS;

	for(i = 0; i < TOTAL_TILES; i++)
	{
		tile_map[i] = tile_list[i].mType;
	}

	return tile_map;
}

void tile_load_tiles(Dict *value)
{
	int i;
	Line key;

	for(i = 0; i < value->item_count; i++)
	{
		Dict * tile;
		Sprite *sprite;

		char *filepath;
		char * type;

		tile = dict_get_hash_nth(key, value, i);

		if(!tile) 
		{
			slog("Could not load tile asset");
			return;
		}
		//get tile info to load sprite

		filepath = (char *)(dict_get_hash_value(tile, "filepath")->keyValue);
		type	 = (char *)(dict_get_hash_value(tile, "type")->keyValue);

		//load and store sprite
		sprite = Sprite_Load(filepath, TILE_WIDTH, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
		
		if(!sprite) 
		{
			slog("Could not load tile sprite");
			return;
		}

		g_hash_table_insert(g_tile_sprites, g_strdup(type), sprite);
	}
}
/**
* @brief loads tile assets from a def file
*/
void tile_load_assets()
{
	Dict *assets;
	Dict *value;

	int i, j;

	Line key;

	assets = load_dict_from_file("def/tiles.def");
	value = dict_get_hash_value(assets, "tiles");

	g_tile_sprites = g_hash_table_new_full(g_str_hash,
							  g_str_equal,
							  (GDestroyNotify)dict_g_string_free,
							  (GDestroyNotify)dict_destroy);

	if(!value || value->data_type != DICT_HASH) 
	{
		slog("Could not load tile set");
		return;
	}

	tile_load_tiles(value);

	value = dict_get_hash_value(assets, "destructables");

	if(!value || value->data_type != DICT_HASH)
	{
		slog("Could not load destructables");
		return;
	}
	
	tile_load_tiles(value);
}
