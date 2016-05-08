#include <jansson.h>

#include "level.h"
#include "load.h"
#include "save.h"
#include "simple_logger.h"
#include "json_parse.h"

void Level_Load()
{

}

void Level_Save()
{
	int tile_count;
	int *tile_map;

	int row;
	int col;

	Dict *dict_tile_map;
	Dict *save;

	tile_map = Tile_Map_Get(tile_count, row, col);
	
	save = Dict_New_Hash();
	
	dict_tile_map = Dict_New_Array( sizeof(int) );
	Dict_Array_Cpy(dict_tile_map, 0, tile_map, tile_count);
	
	//save tile dimensions
	Dict_Hash_Insert(save, "TILE_ROWS", Dict_New_int(row));
	Dict_Hash_Insert(save, "TILE_COLUMNS", Dict_New_int(col));

	//store tile map
	Dict_Hash_Insert(save, "TILE_MAP", dict_tile_map);

	GArray *temp;

	temp = (GArray *)g_hash_table_lookup((GHashTable*)save->keyValue, "TILE_MAP");

	save_dict_as_json(save, "save.def");
}