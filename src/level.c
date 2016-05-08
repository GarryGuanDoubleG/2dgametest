#include <jansson.h>
#include "level.h"
#include "load.h"
#include "save.h"
#include "simple_logger.h"
#include "json_parse.h"
#include <stdio.h>
#include <stdlib.h>

void Level_Load_New()
{
  tile_init_system(Bool_True); // allocate memory for tiles and generate map
  Player_Init(); //creates player entity
}

void Level_Load(int load)
{
	Dict * last_save;
	Dict * value;

	int player_tile;

	int *tile_map;
	int row;
	int col;

	if(load == Bool_False)
	{
		Level_Load_New();
		return;
	}

	last_save = load_dict_from_file("save.def");
	
	value = dict_get_hash_value(last_save, "TILE_ROWS");
	if(!value) return;
	row = atoi((char *)value->keyValue);

	value = dict_get_hash_value(last_save, "TILE_COLUMNS");
	if(!value) return;
	col = atoi((char *)value->keyValue);

	value = dict_get_hash_value(last_save, "TILE_MAP");
	if(!value) return;
	tile_map = (int *)value->keyValue;

	tile_load_from_def(tile_map); // allocate memory for tiles and generate map

	//player data should be stored in a hash
	//but only starting tile is being saved;
	value = dict_get_hash_value(last_save, "PLAYER_TILE");
	if(!value) return;
	Player_Load_from_Def(value); //creates player entity
}

void Level_Save()
{
	int tile_count;
	int *tile_map;

	int row;
	int col;

	int player_tile;
	entity * ent_player;

	Dict *dict_tile_map;
	Dict *save;

	ent_player = Entity_Get_Player();
	player_tile = tile_get_tile_number(player->position, player->boundBox);

	tile_map = Tile_Map_Get(tile_count, row, col); 

	save = Dict_New_Hash();
	
	dict_tile_map = Dict_New_Array( sizeof(int) );
	Dict_Array_Cpy(dict_tile_map, 0, tile_map, tile_count);
	
	//save tile dimensions and map
	Dict_Hash_Insert(save, "TILE_ROWS", Dict_New_int(row));
	Dict_Hash_Insert(save, "TILE_COLUMNS", Dict_New_int(col));
	Dict_Hash_Insert(save, "TILE_MAP", dict_tile_map);
	
	Dict_Hash_Insert(save, "PLAYER_TILE", Dict_New_int(player_tile));

	save_dict_as_json(save, "save.def");
}